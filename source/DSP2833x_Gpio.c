// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:25 $
//###########################################################################
//
// FILE:	DSP2833x_Gpio.c
//
// TITLE:	DSP2833x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 1, 2008 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example. 

void InitGpio(void)
{
	EALLOW;


	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;			// 0 : GPIO27, 1 : ECAP4, 2 : EQEP2S, 3 : MFSXB
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;			// 0 : GPIO32, 1 : SDAA, 2 : EPWMSYNCI, 3 : ADCSOCAO
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;			// 0 : GPIO33, 1 : SCLA, 2 : EPWMSYNCO, 3 : ADCSOCBO
	GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;			// 0 : GPIO48, 1 : ECAP5, 2 : XD31
	GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;			// 0 : GPIO49, 1 : ECAP6, 2 : XD30

	GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;				// 0 : input, 1 : output
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;				// 0 : input, 1 : output
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;				// 0 : input, 1 : output
	GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;				// 0 : input, 1 : output
	GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;				// 0 : input, 1 : output

/*
	GpioDataRegs.GPASET.bit.GPIO27 = 1;
    GpioDataRegs.GPBSET.bit.GPIO33 = 1;
    GpioDataRegs.GPBSET.bit.GPIO48 = 1;
    GpioDataRegs.GPBSET.bit.GPIO49 = 1;
   */
	GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1;

	EDIS;
}


/*
void InitGpio(void)
{
   EALLOW;
   
   // Each GPIO pin can be: 
   // a) a GPIO input/output
   // b) peripheral function 1
   // c) peripheral function 2
   // d) peripheral function 3
   // By default, all are GPIO Inputs 
   GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
   GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
   GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO39
   GpioCtrlRegs.GPBMUX2.all = 0x0000;     // GPIO functionality GPIO48-GPIO63
   GpioCtrlRegs.GPCMUX1.all = 0x0000;     // GPIO functionality GPIO64-GPIO79
   GpioCtrlRegs.GPCMUX2.all = 0x0000;     // GPIO functionality GPIO80-GPIO95

   GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are inputs
   GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO63 are inputs   
   GpioCtrlRegs.GPCDIR.all = 0x0000;      // GPI064-GPIO95 are inputs

   // Each input can have different qualification
   // a) input synchronized to SYSCLKOUT
   // b) input qualified by a sampling window
   // c) input sent asynchronously (valid for peripheral inputs only)
   GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
   GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO39 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPBQSEL2.all = 0x0000;    // GPIO48-GPIO63 Synch to SYSCLKOUT 

   // Pull-ups can be enabled or disabled. 
   GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
   GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO63
   GpioCtrlRegs.GPCPUD.all = 0x0000;      // Pullup's enabled GPIO64-GPIO79

   //GpioCtrlRegs.GPAPUD.all = 0xFFFF;    // Pullup's disabled GPIO0-GPIO31
   //GpioCtrlRegs.GPBPUD.all = 0xFFFF;    // Pullup's disabled GPIO32-GPIO34
   //GpioCtrlRegs.GPCPUD.all = 0xFFFF     // Pullup's disabled GPIO64-GPIO79

   EDIS;

}	
*/	
//===========================================================================
// End of file.
//===========================================================================
