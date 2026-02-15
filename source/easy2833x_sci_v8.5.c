/***************************************************************
    easy2833x_sci_v8.5.c
	2006.02.02 : 	first release
	2008.11.25 : 	change file name (suffix '_sci_v7.1')
					8Byte writing (for 'long double' variable)
					16Byte reading
					enable pull-ups and async
	2009.07.07 : 	remove my own register definition. And follow TI's header files
	2011.02.20 :    8 Byte reading
	2015.12.15 :    8 Byte writing with auto address increasement
					block data (max. 512 word) transfer function
	by Daewoong Chung (Á¤´ë¿õ)
****************************************************************/
#include "easy2833x_sci_v8.5.h"
#include "RingBuff.h"

/////////////////////////////////////////////////////////////////////////////////////////
// NOTICE :
// Select DSP header file
/////////////////////////////////////////////////////////////////////////////////////////
#include "DSP2833x_Device.h"

/////////////////////////////////////////////////////////////////
// NOTICE :
// When you run the code with flash, uncomment below and run the ISRs fast on the RAM
/////////////////////////////////////////////////////////////////
//#pragma CODE_SECTION(easy_RXINT_ISR, "ramfuncs");
//#pragma CODE_SECTION(easy_TXINT_ISR, "ramfuncs");

/////////////////////////////////////////////////////////////////
// NOTICE : Please change below CPU_CLK, LSP_CLK, BAUDRATE
//          according to your system
//          Don't change any other variables and source
/////////////////////////////////////////////////////////////////
#define CPU_CLK		150000000L				// 150MHz
//#define CPU_CLK		120000000L			// 120MHz
//#define CPU_CLK		100000000L			// 100MHz
//#define CPU_CLK		75000000L			// 75MHz

#define	LSP_CLK		(CPU_CLK/4)

//#define	BAUDRATE	1200L
//#define	BAUDRATE	2400L
//#define	BAUDRATE	4800L
//#define	BAUDRATE	9600L
//#define	BAUDRATE	19200L	
//#define	BAUDRATE	38400L
//#define	BAUDRATE	57600L
//#define	BAUDRATE	86400L
#define	BAUDRATE	115200L
//#define	BAUDRATE	153600L
//#define	BAUDRATE	192000L
//#define	BAUDRATE	230400L
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


// TXEMPTY >> TXRDY @ easyDSP v7.4
#define	TX_START	{	if(SciaRegs.SCICTL2.bit.TXRDY) {	\
								SciaRegs.SCICTL2.all = BIT1 | BIT0;	\
								SciaRegs.SCITXBUF = ExtractRing();}	\
						}

#define	TX_STOP	{SciaRegs.SCICTL2.all = BIT1;}	// enable only Rx int

// easyDSP commands & states
#define STAT_INIT		0
#define STAT_ADDR		1
#define STAT_DATA2B		2
#define STAT_DATA4B		3
#define STAT_WRITE		4
#define STAT_DATA8B		5				// v7.1
#define STAT_DATA_BLOCK	6				// v8.5

#define CMD_ADDR			0xE7
#define	CMD_READ2B			0xDB
#define CMD_READ4B			0xC3
#define CMD_READ8B			0x8B		// v8
#define CMD_READ16B			0x28		// v7.1
#define	CMD_DATA2B			0xBD
#define	CMD_DATA4B			0x99
#define	CMD_DATA8B			0x64		// v7.1
#define CMD_DATA_BLOCK		0x55		// v8.5
#define	CMD_WRITE			0x7E
#define	CMD_FB_READ			0x0D
#define	CMD_FB_WRITE_OK		0x0D
#define CMD_FB_WRITE_NG		0x3C

// for internal use
unsigned int ezDSP_DSP = 0, ezDSP_Version_SCI = 730, ezDSP_SW_Reset = 0;
unsigned int ezDSP_uRead16BPossible = 1, ezDSP_uRead8BPossible = 1;	// v8
float ezDSP_fFloat = 0;
unsigned int ezDSP_uBlockTransfer = 1; // v8.5

void easyDSP_SCI_Init()
{
	int i;

	//SCI CLOCK ENABLE
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.SCIAENCLK=1;
	EDIS;

	EALLOW;
	// This will enable the pullups for the specified pins. as in boot rom code
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)
	// This will select asynch (no qualification) for the selected pins. as in boot rom code
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
	// This specifies which of the possible GPIO pins will be SCI functional pins.
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation
    EDIS;

	// SCI Registers Settings
	SciaRegs.SCIFFTX.all = 0xa000;		// FIFO reset
	SciaRegs.SCIFFCT.all = 0x4000;		// Clear ABD
 	
	SciaRegs.SCICCR.all = 0x7;			// 1 stop & no parity & 8bit char, no loopback, idle-line
	SciaRegs.SCICTL1.all = 0x43;		// enable RX-ERR, TX, RX INT

	i = (int)(((float)LSP_CLK/(BAUDRATE*8.) - 1) + 0.5);
	SciaRegs.SCIHBAUD = i >> 8;
	SciaRegs.SCILBAUD = i & 0xFF;

	SciaRegs.SCICTL2.all = 0x3;			// enable RX/BK INT, TX INT
	SciaRegs.SCICTL1.all = 0x63;		// Relinquish SCI from Reset

    // Reassign ISR for easyDSP. Don't use SCIRXINTA_ISR & SCITXINTA_ISR found in DSP28_DefaultIsr.c.
	EALLOW;
	PieVectTable.SCIRXINTA = &easy_RXINT_ISR;
	PieVectTable.SCITXINTA = &easy_TXINT_ISR;
	EDIS;

	// Enable the PIE block
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   
	// Enable SCI-A RX/TX INT in the PIE: Group 9 interrupt 1,2
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;
    // Enable CPU INT9 for SCI-A
	IER |= M_INT9;
	// Enable Global interrupt INTM
	EINT;	

	// init ring buffer
	ResetRing();
}

// error counter
unsigned int ezDSP_uBRKDTCount = 0, ezDSP_uFECount = 0, ezDSP_uOECount = 0, ezDSP_uPECount = 0;
unsigned int ezDSP_uWrongISRCount = 0;

// for easyDSP
unsigned char ezDSP_ucRx = 0;
unsigned int ezDSP_uState = STAT_INIT, ezDSP_uData = 0, ezDSP_uChksum = 0;
unsigned long ezDSP_ulData = 0, ezDSP_ulAddr = 0;
unsigned int ezDSP_uAddrRdCnt = 0, ezDSP_uDataRdCnt = 0;
unsigned long long ezDSP_ullData = 0;
unsigned int ezDSP_uBlockSize = 0, ezDSP_uBlockIndex = 0, ezDSP_uChkSumCalculated = 0; // v8.5
interrupt void easy_RXINT_ISR()
{
#if	0	// Interrupt Nesting Example : allow all the other interrupts

	IER |= M_INT9;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
	__asm("  NOP");
	EINT;
#endif



	Uint16 uIndex;

	// check RX Error
	if(SciaRegs.SCIRXST.bit.RXERROR) {
		if(SciaRegs.SCIRXST.bit.BRKDT)	ezDSP_uBRKDTCount++;	// Break Down
		if(SciaRegs.SCIRXST.bit.FE) 	ezDSP_uFECount++;		// FE
		if(SciaRegs.SCIRXST.bit.OE) 	ezDSP_uOECount++;		// OE
		if(SciaRegs.SCIRXST.bit.PE)		ezDSP_uPECount++;		// PE

		// 'Break down' stops further Rx operation.
		// software reset is necessary to clear its status bit and proceed further rx operation
		SciaRegs.SCICTL1.bit.SWRESET = 0;
		SciaRegs.SCICTL1.bit.SWRESET = 1;

		PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;	
		return;
	}

	// Receive Char
	if(SciaRegs.SCIRXST.bit.RXRDY) {
		ezDSP_ucRx = SciaRegs.SCIRXBUF.bit.RXDT;

		// loop back for test
		//SciaRegs.SCITXBUF = ezDSP_ucRx; 
		//PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;	
		//return;
	}
	else {
		ezDSP_uWrongISRCount++;
		PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;	
		return;
	}

	////////////////////////////////////////////
	// Parsing by state
	////////////////////////////////////////////
	
	if(ezDSP_uState == STAT_INIT) {
		if(ezDSP_ucRx == CMD_ADDR) {
			ezDSP_uState = STAT_ADDR;
			ezDSP_uAddrRdCnt = 0;
		}
		////////////////////////////////////////////////////
		// below command supports auto address increase
		////////////////////////////////////////////////////
		else if(ezDSP_ucRx == CMD_READ2B) {
			ezDSP_ulAddr++;	// auto increment
			ezDSP_uData = *(unsigned int*)ezDSP_ulAddr;
			
			AddRing(ezDSP_uData >> 8);	// MSB
			AddRing(ezDSP_uData);		// LSB
			AddRing(CMD_FB_READ);
			
			ezDSP_uState = STAT_INIT;
			TX_START;
		}
		else if(ezDSP_ucRx == CMD_READ16B) {
			ezDSP_ulAddr += 8;
			for(uIndex = 0; uIndex < 8; uIndex++) {
				// Since this is not for variable, addresss is increased sequentially
				ezDSP_uData = *(unsigned int*)(ezDSP_ulAddr + uIndex);
				AddRing(ezDSP_uData >> 8);		// MSB
				AddRing(ezDSP_uData);			// LSB
			}
			AddRing(CMD_FB_READ);
	
			ezDSP_uState = STAT_INIT;
			TX_START;
		}
		else if(ezDSP_ucRx == CMD_DATA2B) {
			ezDSP_ulAddr++;	// auto increment

			ezDSP_uState = STAT_DATA2B;
			ezDSP_uDataRdCnt = 0;
		}
		else if(ezDSP_ucRx == CMD_DATA4B) {
			ezDSP_ulAddr += 2;	// auto increment

			ezDSP_uState = STAT_DATA4B;
			ezDSP_uDataRdCnt = 0;
		}
		// v8.5
		else if(ezDSP_ucRx == CMD_DATA8B) {
			ezDSP_ulAddr += 4;	// auto increment

			ezDSP_uState = STAT_DATA8B;
			ezDSP_uDataRdCnt = 0;
		}
	}
	else if(ezDSP_uState == STAT_ADDR) {
		ezDSP_uAddrRdCnt++;
		if(ezDSP_uAddrRdCnt == 1) {
			ezDSP_ulAddr = ezDSP_ucRx; 			// MSB
			ezDSP_ulAddr <<= 16; 				// MSB
		}
		else if(ezDSP_uAddrRdCnt == 2)
			ezDSP_ulAddr |= (ezDSP_ucRx << 8);

		else if(ezDSP_uAddrRdCnt == 3)
			ezDSP_ulAddr |= ezDSP_ucRx;			// LSB

		else if(ezDSP_uAddrRdCnt == 4) {
			if(ezDSP_ucRx == CMD_READ2B) {
				ezDSP_uData = *(unsigned int*)ezDSP_ulAddr;
				
				AddRing(ezDSP_uData >> 8);	// MSB
				AddRing(ezDSP_uData);		// LSB
				AddRing(CMD_FB_READ);
				
				ezDSP_uState = STAT_INIT;
				TX_START;
			}
			else if(ezDSP_ucRx == CMD_READ4B) {
				ezDSP_uData = *(unsigned int*)(ezDSP_ulAddr + 1);
				AddRing(ezDSP_uData >> 8);	// MSB
				AddRing(ezDSP_uData);		

				ezDSP_uData = *(unsigned int*)ezDSP_ulAddr;
				AddRing(ezDSP_uData >> 8);	
				AddRing(ezDSP_uData);		// LSB

				AddRing(CMD_FB_READ);
				
				ezDSP_uState = STAT_INIT;
				TX_START;
			}
			// v8
			else if(ezDSP_ucRx == CMD_READ8B) {
				ezDSP_uData = *(unsigned int*)(ezDSP_ulAddr + 3);
				AddRing(ezDSP_uData >> 8);	// MSB
				AddRing(ezDSP_uData);		

				ezDSP_uData = *(unsigned int*)(ezDSP_ulAddr + 2);
				AddRing(ezDSP_uData >> 8);	
				AddRing(ezDSP_uData);

				ezDSP_uData = *(unsigned int*)(ezDSP_ulAddr + 1);
				AddRing(ezDSP_uData >> 8);
				AddRing(ezDSP_uData);		

				ezDSP_uData = *(unsigned int*)ezDSP_ulAddr;
				AddRing(ezDSP_uData >> 8);	
				AddRing(ezDSP_uData);		// LSB

				AddRing(CMD_FB_READ);
				
				ezDSP_uState = STAT_INIT;
				TX_START;
			}
			else if(ezDSP_ucRx == CMD_READ16B) {
				for(uIndex = 0; uIndex < 8; uIndex++) {
					// Since this is not for variable, addresss is increased sequentially
					ezDSP_uData = *(unsigned int*)(ezDSP_ulAddr + uIndex);
					AddRing(ezDSP_uData >> 8);		// MSB
					AddRing(ezDSP_uData);			// LSB
				}
				AddRing(CMD_FB_READ);
				
				ezDSP_uState = STAT_INIT;
				TX_START;
			}
			else if(ezDSP_ucRx == CMD_DATA2B) {
				ezDSP_uState = STAT_DATA2B;
				ezDSP_uDataRdCnt = 0;
			}
			else if(ezDSP_ucRx == CMD_DATA4B) {
				ezDSP_uState = STAT_DATA4B;
				ezDSP_uDataRdCnt = 0;
			}
			else if(ezDSP_ucRx == CMD_DATA8B) {
				ezDSP_uState = STAT_DATA8B;
				ezDSP_uDataRdCnt = 0;
			}
			// v8.5
			else if(ezDSP_ucRx == CMD_DATA_BLOCK) {
				ezDSP_uState = STAT_DATA_BLOCK;
				ezDSP_uDataRdCnt = 0;
			}
			else ezDSP_uState = STAT_INIT;
		}
		else 
			ezDSP_uState = STAT_INIT;
	}
	
	else if(ezDSP_uState == STAT_DATA2B) {
		ezDSP_uDataRdCnt++;
		if(ezDSP_uDataRdCnt == 1)
			ezDSP_uData = ezDSP_ucRx << 8; 		// MSB
		else if(ezDSP_uDataRdCnt == 2)
			ezDSP_uData |= ezDSP_ucRx; 			// LSB
		else if(ezDSP_uDataRdCnt == 3) 		
			ezDSP_uChksum = ezDSP_ucRx << 8;	// MSB
		else if(ezDSP_uDataRdCnt == 4)
			ezDSP_uChksum |= ezDSP_ucRx;		// LSB
		else if(ezDSP_uDataRdCnt == 5) {
			if(ezDSP_ucRx == CMD_WRITE) {
				if(ezDSP_uChksum == ((ezDSP_ulAddr + ezDSP_uData) & 0xFFFF)) {
					*(unsigned int*)ezDSP_ulAddr = ezDSP_uData;
					AddRing(CMD_FB_WRITE_OK);
					ezDSP_uState = STAT_INIT;				
				}
				else {
					AddRing(CMD_FB_WRITE_NG);
					ezDSP_uState = STAT_INIT;
				}
				TX_START;
			}
			else
				ezDSP_uState = STAT_INIT;
		}
		else
			ezDSP_uState = STAT_INIT;
	}
	else if(ezDSP_uState == STAT_DATA4B) {
		ezDSP_uDataRdCnt++;
		if(ezDSP_uDataRdCnt == 1) {
			ezDSP_ulData = ezDSP_ucRx; 		// MSB
			ezDSP_ulData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 2) {
			ezDSP_ulData |= ezDSP_ucRx;
			ezDSP_ulData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 3) {
			ezDSP_ulData |= ezDSP_ucRx;
			ezDSP_ulData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 4) {
			ezDSP_ulData |= ezDSP_ucRx;
		}
		else if(ezDSP_uDataRdCnt == 5) 		
			ezDSP_uChksum = ezDSP_ucRx << 8;	// MSB
		else if(ezDSP_uDataRdCnt == 6)
			ezDSP_uChksum |= ezDSP_ucRx;		// LSB
		else if(ezDSP_uDataRdCnt == 7) {
			if(ezDSP_ucRx == CMD_WRITE) {
				if(ezDSP_uChksum == ((ezDSP_ulAddr + ezDSP_ulData) & 0xFFFF)) {
					*(unsigned long*)ezDSP_ulAddr = ezDSP_ulData;
					AddRing(CMD_FB_WRITE_OK);
					ezDSP_uState = STAT_INIT;				
				}
				else {
					AddRing(CMD_FB_WRITE_NG);
					ezDSP_uState = STAT_INIT;
				}
				TX_START;
			}
			else
				ezDSP_uState = STAT_INIT;
		}
		else
			ezDSP_uState = STAT_INIT;
	}
	// new
	else if(ezDSP_uState == STAT_DATA8B) {
		ezDSP_uDataRdCnt++;
		if(ezDSP_uDataRdCnt == 1) {
			ezDSP_ullData = ezDSP_ucRx; 		// MSB
			ezDSP_ullData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 2) {
			ezDSP_ullData |= ezDSP_ucRx;
			ezDSP_ullData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 3) {
			ezDSP_ullData |= ezDSP_ucRx;
			ezDSP_ullData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 4) {
			ezDSP_ullData |= ezDSP_ucRx;
			ezDSP_ullData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 5) {
			ezDSP_ullData |= ezDSP_ucRx;
			ezDSP_ullData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 6) {
			ezDSP_ullData |= ezDSP_ucRx;
			ezDSP_ullData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 7) {
			ezDSP_ullData |= ezDSP_ucRx;
			ezDSP_ullData <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 8) {
			ezDSP_ullData |= ezDSP_ucRx;
		}
		else if(ezDSP_uDataRdCnt == 9) 		
			ezDSP_uChksum = ezDSP_ucRx << 8;	// MSB
		else if(ezDSP_uDataRdCnt == 10)
			ezDSP_uChksum |= ezDSP_ucRx;		// LSB
		else if(ezDSP_uDataRdCnt == 11) {
			if(ezDSP_ucRx == CMD_WRITE) {
				if(ezDSP_uChksum == ((ezDSP_ulAddr + ezDSP_ullData) & 0xFFFF)) {
					*(unsigned long long*)ezDSP_ulAddr = ezDSP_ullData;
					AddRing(CMD_FB_WRITE_OK);
					ezDSP_uState = STAT_INIT;				
				}
				else {
					AddRing(CMD_FB_WRITE_NG);
					ezDSP_uState = STAT_INIT;
				}
				TX_START;
			}
			else
				ezDSP_uState = STAT_INIT;
		}
		else
			ezDSP_uState = STAT_INIT;
	}
	// v8.5 
	else if(ezDSP_uState == STAT_DATA_BLOCK) {
		ezDSP_uDataRdCnt++;
		// block size
		if(ezDSP_uDataRdCnt == 1) {
			ezDSP_uBlockSize = ezDSP_ucRx; 		// MSB. size in word
			ezDSP_uBlockSize <<= 8;
		}
		else if(ezDSP_uDataRdCnt == 2) {
			ezDSP_uBlockSize |= ezDSP_ucRx;		// LSB
			ezDSP_uChkSumCalculated = ezDSP_uBlockSize + ezDSP_ulAddr;
			ezDSP_uBlockIndex = 0;
		}
		// data transfer : ezDSP_uBlockSize = in words
		else if((ezDSP_uDataRdCnt >= 3) && (ezDSP_uDataRdCnt < (3 + ezDSP_uBlockSize*2))) {
			if(ezDSP_uBlockIndex == 0) {
				ezDSP_uData =  ezDSP_ucRx;
				ezDSP_uBlockIndex = 1;
			}
			else if(ezDSP_uBlockIndex == 1 ) {
				ezDSP_uData <<= 8;
				ezDSP_uData |=  ezDSP_ucRx;
				ezDSP_uBlockIndex = 0;

				// transfer
				*(unsigned int*)ezDSP_ulAddr = ezDSP_uData;
				ezDSP_uChkSumCalculated += ezDSP_uData;
				ezDSP_ulAddr++;
			}
		}
		else if(ezDSP_uDataRdCnt == (3 + ezDSP_uBlockSize*2 + 0)) 	ezDSP_uChksum = ezDSP_ucRx << 8;	// MSB
		else if(ezDSP_uDataRdCnt == (3 + ezDSP_uBlockSize*2 + 1))	ezDSP_uChksum |= ezDSP_ucRx;		// LSB
		else if(ezDSP_uDataRdCnt == (3 + ezDSP_uBlockSize*2 + 2)) {
			if(ezDSP_ucRx == CMD_WRITE) {
				if(ezDSP_uChksum == ezDSP_uChkSumCalculated)	AddRing(CMD_FB_WRITE_OK);
				else 											AddRing(CMD_FB_WRITE_NG);
				ezDSP_uState = STAT_INIT;
				TX_START;
			}
			else
				ezDSP_uState = STAT_INIT;
		}
		else
			ezDSP_uState = STAT_INIT;
	}
	else {
		ezDSP_uState = STAT_INIT;
	}


	// Acknowledge this interrupt to recieve more interrupts from group 9
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}
                                              
interrupt void easy_TXINT_ISR(void)
{
#if	0
    IER |= M_INT9;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
	__asm("  NOP");
	EINT;
#endif

	// buffer => sio
	if(!IsRingEmpty()) {
		if(SciaRegs.SCICTL2.bit.TXRDY)	// check TXRDY
			SciaRegs.SCITXBUF = ExtractRing();
		else 
			ezDSP_uWrongISRCount++;
	}
	else {
		TX_STOP;
	}

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;	
}
