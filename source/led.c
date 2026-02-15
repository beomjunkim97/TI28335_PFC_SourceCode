#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "LED.h"

#define _LED_

void delay_long(long int delay_cnt)
{
	while(delay_cnt--);
}


void led_setup()
{
    EALLOW;
	GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0;

	GpioCtrlRegs.GPCDIR.bit.GPIO79 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO78 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO77 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO76 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO75 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO74 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO73 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO72 = 1;

	EDIS;
//	LED0_OFF;
//	LED1_OFF;
//	LED2_OFF;
//	LED3_OFF;
//	LED4_OFF;
//	LED5_OFF;
//	LED6_OFF;
//	LED7_OFF;
}

