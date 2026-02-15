#define _QEP_


#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "Qep.h"

// Initialize eQEP1
void Init_eQEP1(void){//*EQep1의 레지스터를 설정하는 함수. 28335는 2개의 EQep 모듈을 가지고 있다.

    //*EQep 디코더 설정 레지스터*//
	// Setup eQEP1 Decoder Control 
	EQep1Regs.QDECCTL.bit.QSRC = 2;	// UP count mode for frequency measurement
	//*0: Quadrature count mode //QEPA(엔코더 A슬릿 신호)와 QEPB(엔코더 A슬릿 신호)를 합쳐서 2bit의 조합을 구성. QEPA와 QEPB의 high or low에 따라 00 01 10 11의 네가지 상태로 분류.
	                            //정방향: 00 -> 10 -> 11 -> 01. 정방향일 때 각 상태 변화마다 QPOSCNT는 1씩 증가하고, 역방향일 경우 각 상태 변화마다 QPOSCNT는 1씩 감소한다.(4체배)
	//*1: Direction-count mode  //모터의 회전방향에 따라 QEPA 당 position counter(QPOSCNT)를 증가 또는 감소시키는 모드. 설정에 따라 QEPA의 rising edge에서만 QPOSCNT를 증가시키거나(1체배), rising/falling edge 모두 QPOSCNT를 증가시킬 수 있다.(2체배)
	//*2: UP count mode for frequency measurement  //모터의 회전방향 상관없이 QEPA 당 position-counter(QPOSCNT)를 1씩 증가시키는 모드. 설정에 따라 QEPA의 rising edge에서만 QPOSCNT를 증가시키거나(1체배), rising/falling edge 모두 QPOSCNT를 증가시킬 수 있다.(2체배)
	//*3: DOWN count mode for frequency measurement  //모터의 회전방향 상관없이 QEPA 당 position-counter(QPOSCNT)를 1씩 감소시키는 모드. 설정에 따라 QEPA의 rising edge에서만 QPOSCNT를 증가시키거나(1체배), rising/falling edge 모두 QPOSCNT를 증가시킬 수 있다.(2체배)

	EQep1Regs.QDECCTL.bit.XCR = 1;
	//*QCLK의 채배를 결정하는 비트. QCLK는 QPOSCNT(position-counter: 회전각도를 측정하기 위해 엔코더 슬릿 펄스를 계수하는 32bit 레지스터), UPEVNT(Unit Position Event: QCTMR을 초기화하는 비트)애서 이용된다.
    //*0: 2x resolution: Count the rising/falling edge  //A, B슬릿의  rising/falling(4채배) edge에서 QCLK 클럭 펄스가 발생한다.
    //*1: 1x resolution: Count the rising edge only  //A, B슬릿의  rising(2채배) edge에서 QCLK 클럭 펄스가 발생한다.

	EQep1Regs.QDECCTL.bit.SOEN = 0;		// Disable position-compare sync output
	//*max count에 동기해서 어떤 이벤트가 발생하도록 할 것인가 결정. 필요없음
	//*0: Disable position-compare sync output
	//*1: Enable position-compare sync output



	//*unit timer의 주기를 결정하는 32bit 레지스터.*//
	// Setup eQEP1 Unit Timer Period(1ms)
	EQep1Regs.QUPRD = 300000; 			// SYSCLKOUT(=150MHz)/1000 = 1ms period
	//*unit timer는 포화 시 여러 레지스터들의 값을 저장용 레지스터로 옮기고 인터럽트를 발생시킨다.
	//*unit timer period = QUPRD / SYSCLKOUT(150MHz)
	


	//*position counter를 초기화하는 레지스터*// position counter에 사용하는 엔코더 슬릿 수를 나눠주면 회전각도를 측정할 수 있다.
	// Setup eQEP1 Position Counter Initialization
	EQep1Regs.QPOSINIT = 0x00000000;
	//*32bit의 정수형으로 입력해야 한다. 0x가 앞에 붙는 숫자는 16진수이며 16진수에서는 한 단위가 2진수의 네 단위를 표현할 수 있다.
	


	//*position counter의 최대값을 초기화하는 레지스터.*//
	// Setup eQEP1 Maximum Position Counter
	EQep1Regs.QPOSMAX = 0xffffffff;
	//*32bit의 정수형으로 입력해야 한다.

	

	//*EQep 설정 레지스터*//
	// Setup eQEP1 Control
	// Emulation Control Bits
	// position counter, watchdog counter, unit timer, capture timer is unaffected by emulation suspend 
	EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;
	//*0: 에뮬레이터 중지 시 Position counter, Watchdog counter, Unit timer stops, Capture Timer 즉시 중단한다.
	//*1:
	//*2: 에뮬레이터 중지에 영향받지 않는다.
	//*3: FREE_SOFT_2와 같다.
	
	EQep1Regs.QEPCTL.bit.PCRM = 1;	// Position counter reset on the index position
	//*0: Position counter reset on an index event
	//*1: Position counter reset on the maximum position
	//*2: Position counter reset on the first index event
	//*3: Position counter reset on a unit time event

	EQep1Regs.QEPCTL.bit.UTE = 1; 	// Enable unit timer
	//*0: eQEP unit timer 사용하지 않음.
	//*1: unit timer 사용.

	EQep1Regs.QEPCTL.bit.WDE = 0;	// Disable the eQEP2 watchdog timer
	//*0: eQEP watchdog timer 사용하지 않음.
	//*1: watchdog timer 사용.
	
	// Position counter, capture timer, capture period values is latched
	// into QPOSLAT, QCTMRLAT and QCPRDLAT register on unit time out  
	EQep1Regs.QEPCTL.bit.QCLM = 1;	// Latch on unit time out
	//*0: CPU가 [Position counter, capture timer, capture period values]의 값을 읽을 때 각각 LAT 레지스터(값을 저장하는 레지스터)에 저장된다.
	//*1: unit timer 주기마다 [Position counter(32bit), capture timer(16bit), capture period values(16bit)]가 각각 LAT 레지스터(값을 저장하는 레지스터)에 저장된다.

	EQep1Regs.QEPCTL.bit.QPEN = 1; 	// Enable eQEP position counter
	//*0: eQEP peripheral 내부 작동 플래그와 읽기 전용 레지스터를 재설정.
	//*1: eQEP Position counter 사용.

    EQep1Regs.QEPCTL.bit.IEI = 10;
    //*0: 아무 것도 하지 않음.
    //*1: 아무 것도 하지 않음.
    //*2: 엔코더 인덱스 펄스(z펄스)의 rising edge에서 position counter 초기화. QEPI 펄스(Index Pulse)는  1회전 당 1개만 발생하는 회전자의 기준 위치를 설정하기 위한 신호
    //*3: 엔코더 인덱스 펄스(z펄스)의 falling edge에서 position counter 초기화.


	
	//*EQep 위치 비교 설정 레지스터*//
	// Setup eQEP2 Position-compare control
	EQep1Regs.QPOSCTL.bit.PCE = 0;	// Disable position compare unit
	//*0: eQEP position compare unit 사용하지 않음.
	//*1: Position compare unit 사용.
	


	//*EQep 캡쳐 설정 레지스터*//
	// Setup eQEP2 Capture Control
	EQep1Regs.QCAPCTL.bit.UPPS = 0;	// Unit position event prescaler(UPEVENT = QCLK/(2^1))
	//*Unit position event 분주비. UPEVENT은  QCLK에서 분주된다.
	//*UPEVENT는 Capture Timer(QCTMR)를 초기화시킨다. QCTMR은 시스템 클럭(SYSCLKOUT)에서 분주된 일정한 클럭(CAPCLK)마다 1씩 더해지는 타이머이다. QCTMR 값으로 엔코더 펄스 간 주기를 측정할 수 있다.
	//*4bit로 할당된 레지스터로 0 ~ 15까지 입력할 수 있다. (UPEVENT = QCLK / 2^(UPPS))

	EQep1Regs.QCAPCTL.bit.CCPS = 4;	// eQEP2 capture timer clock prescaler(CAPCLK = SYSCLKOUT(=150MHz)/16)
	//*capture timer clock 분주비. CAPCLK은 시스템 클럭(SYSCLKOUT)에서 분주된다.
	//*CAPCLK마다 Capture Timer(QCTMR)는 1씩 증가한다.
	//*3bit로 할당된 레지스터로 0 ~ 7까지 입력할 수 있다. (CAPCLK = QCLK / 2^(CCPS))

	EQep1Regs.QCAPCTL.bit.CEN = 1; 	// Enable eQEP2 capture unit
	//*0: eQEP capture unit 사용하지 않음.
	//*1: capture unit 사용.



	//*EQep 인터럽트 사용 레지스터*// EQep 인터럽트 발생 조건을 설정하는 레지스터
	// Setup eQEP2 Interrupt Enable
	EQep1Regs.QEINT.bit.UTO = 1;	// Unit time out interrupt enable
	//*0: eQEP unit timer 주기마다 인터럽트 사용하지 않음.
	//*1: unit timer 주기마다 인터럽트 사용.



   	// Setup GPIO and interrupt service routine for eQEP1
   	EALLOW;

   	//*GPIO B그룹 풀업 설정 레지스터*// 기본값 0일 때 GPIO는 풀업 상태
 	GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;   // Enable pull-up on GPIO24 (EQEP1A)
    GpioCtrlRegs.GPBPUD.bit.GPIO51 = 0;   // Enable pull-up on GPIO25 (EQEP1B)
    GpioCtrlRegs.GPBPUD.bit.GPIO53 = 0;   // Enable pull-up on GPIO25 (EQEP1I)

    

    //*GPIO B그룹 입력 자격 레지스터2*// GPIO 입력을 시스템 클럭의 몇 배마다 받을지 결정하는 레지스터. 기본값 0일 때 시스템 클럭마다 GPIO 입력
    GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 0; // Sync to SYSCLKOUT GPIO24 (EQEP1A)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 0; // Sync to SYSCLKOUT GPIO25 (EQEP1B)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO53 = 0; // Sync to SYSCLKOUT GPIO25 (EQEP1I)

    

    //*GPIO B그룹 멀티플렉서 레지스터2*// 해당 GPIO를 EQEP1의 주변기기로 사용
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 1;  // Configure GPIO24 as EQEP1A
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 1;  // Configure GPIO25 as EQEP1B
    GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 1;  // Configure GPIO25 as EQEP1I



    PieVectTable.EQEP1_INT = &EQEP1_INT_ISR;  // for eQEP1 interrupt
	//EQEP1_INT_ISR
    //*함수 EQEP1_INT_ISR의 주소값을 PieVectTable.EQEP1_INT에 저장하여, EQEP1 인터럽트 발생 시  EQEP1_INT_ISR()가 실행되도록 한다.

	EDIS;

    // Enable CPU INT5 for eQEP2
	IER |= M_INT5;
	//*EQEP1 인터럽트가 속한 CPU 인터럽트 5를 사용할 수 있게함.

    // Enable eQEP1 INT in the PIE: Group 5 interrupt 1
	PieCtrlRegs.PIEIER5.bit.INTx1 = 1;
	//*EQEP1 인터럽트가 속한 주변기기 인터럽트 1을 사용할 수 있게함.
}


// Initialize eQEP2
void Init_eQEP2(void){
	// Setup eQEP2 Decoder Control 
	EQep2Regs.QDECCTL.bit.QSRC = 2;		// Direction count mode
	EQep2Regs.QDECCTL.bit.XCR = 1;
	EQep2Regs.QDECCTL.bit.SOEN = 0;		// Disable position-compare sync output
	
	// Setup eQEP2 Unit Timer Period(1ms)
	EQep2Regs.QUPRD = 150000; 			// SYSCLKOUT(=150MHz)/1000 = 1ms period
	
	// Setup eQEP2 Position Counter Initialization
	EQep2Regs.QPOSINIT = 0x00000000;
	
	// Setup eQEP2 Maximum Position Counter
	EQep2Regs.QPOSMAX = 0xffffffff;
	
	// Setup eQEP2 Control
	// Emulation Control Bits
	// position counter, watchdog counter, unit timer, capture timer is unaffected by emulation suspend 
	EQep2Regs.QEPCTL.bit.FREE_SOFT = 2;
	
	// 00 : position counter is reset on an index event
	// 01 :                  is reset on the maximum position
	// 10 :                  is reset on the first index event
	// 11 :                  is reset on a unit timer event 
	EQep2Regs.QEPCTL.bit.PCRM = 0;	// Position counter reset on the index position
	EQep2Regs.QEPCTL.bit.UTE = 1; 	// Enable unit timer
	EQep2Regs.QEPCTL.bit.WDE = 0;	// Disable the eQEP2 watchdog timer
	
	// Position counter, capture timer, capture period values is latched
	// into QPOSLAT, QCTMRLAT and QCPRDLAT register on unit time out  
	EQep2Regs.QEPCTL.bit.QCLM = 1;	// Latch on unit time out
	EQep2Regs.QEPCTL.bit.QPEN = 1; 	// Enable eQEP position counter
	
	// Setup eQEP2 Position-compare control
	EQep2Regs.QPOSCTL.bit.PCE = 0;	// Disable position compare unit
	
	// Setup eQEP2 Capture Control
	EQep2Regs.QCAPCTL.bit.UPPS = 0;	// Unit position event prescaler(UPEVENT = QCLK/(2^1))
	EQep2Regs.QCAPCTL.bit.CCPS = 4;	// eQEP2 capture timer clock prescaler(CAPCLK = SYSCLKOUT(=150MHz)/16)
	EQep2Regs.QCAPCTL.bit.CEN = 1; 	// Enable eQEP2 capture unit
	
	// Setup eQEP2 Interrupt Enable
	EQep2Regs.QEINT.bit.UTO = 1;	// Unit time out interrupt enable

	EQep2Regs.QEPCTL.bit.IEI = 10;
   	
   	// Setup GPIO and interrupt service routine for eQEP2
   	EALLOW;
 	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;   // Enable pull-up on GPIO24 (EQEP2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;   // Enable pull-up on GPIO25 (EQEP2B)
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;    // Enable pull-up on GPIO26 (EQEP2I)
    
    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0; // Sync to SYSCLKOUT GPIO24 (EQEP2A)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0; // Sync to SYSCLKOUT GPIO25 (EQEP2B)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 0; // Sync to SYSCLKOUT GPIO25 (EQEP2I)
        
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;  // Configure GPIO24 as EQEP2A
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;  // Configure GPIO25 as EQEP2B
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 2;  // Configure GPIO25 as EQEP2I

    PieVectTable.EQEP2_INT = &EQEP2_INT_ISR;  // for eQEP2 interrupt
	//EQEP2_INT_ISR
	EDIS;

    // Enable CPU INT5 for eQEP2
	IER |= M_INT5;

    // Enable eQEP2 INT in the PIE: Group 5 interrupt 2
	PieCtrlRegs.PIEIER5.bit.INTx2 = 1;
}
