#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "float.h"
#include "math.h"
#include "dac_easyDSP.h"
#include "Timer.h"
#include "Qep.h"
#include "DSP2833x_Adc.h"
#include "DSP2833x_GlobalPrototypes.h"


void main(void)
{
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EINT; // Enable Global interrupt INT7M
    ERTM; // Enable Global realtime interrupt DBGM

    InitSysCtrl();
    dac_setup();

    InitEPwm1Gpio();
    InitEPwm2Gpio();
    InitEPwm3Gpio();
    //InitEPwm4Gpio();
    InitTzGpio();

//    InitECanGpio();

    Timer_Init(); //Timer(CPU clock, ���ͷ�Ʈ) �ʱ�ȭ
    InitAdc(); //ADC Register �ʱ�ȭ
    Init_Code(); //easy_DSP���� Code

    easyDSP_SCI_Init(); //easy_DSP�� SCI���� Code

    InitEPWM();
    InitGpio();
    PreChg_MC_Open;
    PWM_CUT();

//    Init_eQEP1();
//    Init_eQEP2();
//    InitECan();

    InitParameter();
    EnableInterrupts();

    Set_DA_Code(30, 20, 16, 21);
    Set_DA_Offset(800.,0.,0.,0.); 
    Set_DA_Scale(40.,20.,10.,20.); 

    while(1)
    {

    }

}

//
