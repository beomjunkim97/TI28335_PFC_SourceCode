#define _TIMER_

#include "DSP2833x_Device.h"         // Headerfile Include File
#include "DSP2833x_Examples.h"       // Examples Include File
#include "Timer.h"

#define ABS(x) ((x>=0)? (x) : -(x))  //절대값의 정의

#include "DSP2833x_GlobalPrototypes.h"
//#include "FPU.h"
#include <math.h>



void Timer_Init(void)
{
    DINT;
    IER = 0x0000;               // Disable CPU interrupts and clear all CPU interrupt flags
    IFR = 0x0000;

    InitPieCtrl();              //Initialize the PIE control registers
    InitPieVectTable();         //Initialize the PIE vector table with pointers

    InitCpuTimers();
    //Timer 설정  (CpuTimer0  , 시스템클럭[MHz],  인터럽트주기[usec])
    ConfigCpuTimer(&CpuTimer0, 150, 50); // 10000이면 10[ms]마다 한번씩 인터럽트 수행, 50=50usec마다 timer0 수행
    StartCpuTimer0();

    IER |= M_INT1;  // Disable CPU interrupts

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;   //INTx.7이 TIMER0가 있는 열
 
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
}
