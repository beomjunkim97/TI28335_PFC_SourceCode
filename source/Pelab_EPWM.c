#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "DSP2833x_EPwm.h"
#include "DSP2833x_GlobalPrototypes.h"
//#include "FPU.h"
#include <math.h>

//---------------------------------------------------------------------------
// InitEPwm: 
//---------------------------------------------------------------------------
// This function initializes the ePWM(s) to a known state.
//

void InitTB(struct EPWM_REGS * EPwmRegs)//Time Base Module Initializing
{
	//TB(Time Base) Module Clock Frequency
	//TBCLK=CPUCLKOUT/(HSPCLKDIV*CLKDIV)=150MHz
    //DIV = 분주, HSPCLKDIV = High speed time pre-scale, CLKDIV = Timebase clock pre-scale
	EPwmRegs->TBCTL.bit.HSPCLKDIV = TB_DIV1; //TB_DIV1=0x0, TB_DIV2=0x1, TB_DIV4=0x2
	EPwmRegs->TBCTL.bit.CLKDIV = TB_DIV1; //TB_DIV1=0x0, TB_DIV2=0x1, TB_DIV4=0x2
	
	//Carrier Period and Phase Setting
	EPwmRegs->TBPRD = 7500; //10kHz Carrier using TBCLK(150Mhz)
	EPwmRegs->TBPHS.half.TBPHS = 0; //Set Phase register to zero
	EPwmRegs->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;	//TB_COUNT_UP, TB_COUNT_DOWN, TB_COUNT_UPDOWN
	//HSPCLKDIV=TB_DIV2, CLKDIV=TB_DIV1이라면 TBCLK=75MHz -> TBPRD=1250 이라면 30kHz 출력 가능 , TBPRD = 3750 일때 10kHz 출력 가능
	//HSPCLKDIV=TB_DIV1, CLKDIV=TB_DIV1이라면 TBCLK=150MHz -> TBPRD=7500 이라면 10kHz 출력 가능

	//Synchronizing EPWM module
	EPwmRegs->TBCTL.bit.PHSEN = TB_DISABLE; //Carrier Phase Synchronizing //TB_DISABLE=0x0, PHSEN=0으로 설정하면 해당 ePWM은 마스터가 된다 마스터가 된 ePWM에 동기화
	EPwmRegs->TBCTL.bit.PRDLD = TB_SHADOW;
	EPwmRegs->TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; //TB_CTR_ZERO=0x1, SYNCOSEL=1이면 COUNTER=0에 동기화
}

void InitCC(struct EPWM_REGS * EPwmRegs)//Counter Compare Module Initializing
{
	//Counter for Event Trigger
	EPwmRegs->CMPA.half.CMPA = CMP1; //Reference Value of PWMA
	EPwmRegs->CMPB = CMP1; //Reference Value of PWMB (Same to PWMA but, PWM is

	//Activating Shadowing Mode or Immediate Mode
	EPwmRegs->CMPCTL.bit.SHDWAMODE = CC_SHADOW; //Activating Shadowing in CMPA
	EPwmRegs->CMPCTL.bit.SHDWBMODE = CC_SHADOW; //Activating Shadowing in CMPB

	//Time Setting for Updating CC
	EPwmRegs->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; //Time for Updating CC Register
	EPwmRegs->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; //Time for Updating CC Register
}

void InitAQ(struct EPWM_REGS * EPwmRegs) //Action Qualifier Module Initializing
{
	//Generate PWM Signal
	EPwmRegs->AQCTLA.bit.ZRO = AQ_CLEAR; //Output of EPWMA (When the carrier is zero)
	EPwmRegs->AQCTLA.bit.CAU = AQ_SET; //Output of EPWMA (When the carrier is bigger than the reference)
	EPwmRegs->AQCTLA.bit.CAD = AQ_CLEAR; //Output of EPWMA (When the carrier is smaller than the reference)

	//Opposite Action against CMPA   DC Motor Control Project 변경
	EPwmRegs->AQCTLB.bit.ZRO = AQ_SET; //Output of EPWMB (When the carrier is zero)
	EPwmRegs->AQCTLB.bit.CBU = AQ_CLEAR; //Output of EPWMB (When the carrier is bigger than the reference)
	EPwmRegs->AQCTLB.bit.CBD = AQ_SET; //Output of EPWMB (When the carrier is smaller than the reference)
}

void InitDB(struct EPWM_REGS * EPwmRegs)//Dead Band Module Initializing
{
    //IN_MODE 레지스터 : Dead Band 블록으로 입력되는 신호를 결정하는 레지스터
    //Edge Select of Dead Band
    //0x0=DBA_ALL : Both Falling Edge and Rising Edge Delay of EPWMxA
    //0x1=DBB_RED_DBA_FED : Rising Edge Delay of EPWMxB, Falling Edge Dealy of EPWMxA
    //0x2=DBA_RED_DBB_FED : Rising Edge Delay of EPWMxA, Falling Edge Delay of EPWMxB
    //0x3=DBB_ALL : Both Falling Edge and Rising Edge Delay of EPWMxB
    //Dead Band Delay Setting
    EPwmRegs->DBCTL.bit.IN_MODE = DBA_ALL;

    //OUT_MODE 레지스터 : Dead band 블록을 통과한 후에 상승 엣지와 하강 엣지 지연이 발생한 신호를 어떤 신호로 출력할지 결정하는 레지스터
	//Enable Dead Band Module
	//0x0=DB_DISABLE : Both EPWMxA and EPWMxB are not using DB
	//0x1=DBA_ENABLE : Only EPWMxA is using DB
	//0x2=DBB_ENABLE : Only EPWMxB is using DB
	//0x3=DB_FULL_ENABLE : Both EPWMxA and EPWMxB are using DB
    EPwmRegs->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //DB_DISABLE, DBA_ENABLE, DBB_ENABLE, DB_FULL_ENABLE

	//Pole Select of Output of PWM
	//0x0=DB_ACTV_HI : Neither EPWMxA nor EPWMxB is inverterd
	//0x1=DB_ACTV_LOC : EPWMxA is inverted
	//0x2=DB_ACTV_HIC : EPWMxB is inverted
	//0x3=DB_ACTV_LO : Both EPWMxA and EPWMxB are inverted
	EPwmRegs->DBCTL.bit.POLSEL = DB_ACTV_HIC;

	//Set the Period of the Dead Band
	// 하강 지연 시간 = DBFED * T(TBCLK) , 상승 지연 시간 = DBRED * T(TBCLK)
    EPwmRegs->DBFED = 7500 * PWM_DeadTime_Ratio * 2; //Falling Edge Delay[CPU_CLK*x]
	EPwmRegs->DBRED = 7500 * PWM_DeadTime_Ratio * 2; //Rising Edge Delay[CPU_CLK*x]
}

void InitET(struct EPWM_REGS * EPwmRegs)//Event Trigger Module Initializing
{
	//Enable Interrupt at Event Trigger
	EPwmRegs->ETSEL.bit.INTEN = 0; // Disable EPWMx_INT
	EPwmRegs->ETSEL.bit.INTSEL = ET_CTR_ZERO; // 1:ZRO, 2:PRD, 4:CAU, 5:CAD, 6:CBU, 7:CBD
	EPwmRegs->ETPS.bit.INTPRD = ET_1ST; // Generate pulse on 1st event 

	//	PIE : Peripheral Interrupts setting
	IER |= M_INT3;	// for ePWMINT
	PieCtrlRegs.PIEIER3.bit.INTx1 = 1; //EPWM1 Interrupt in PIE Vector Table 3.1
	
	//Start of Conversion A ADC at Event Trigger
	EPwmRegs->ETSEL.bit.SOCAEN = 1;	// Enable SOCA
	EPwmRegs->ETSEL.bit.SOCASEL = ET_CTR_ZERO;
	EPwmRegs->ETPS.bit.SOCAPRD = ET_1ST;

	//Start of Conversion B ADC at Event Trigger
	EPwmRegs->ETSEL.bit.SOCBEN = 1;	// Enable SOCB
	EPwmRegs->ETSEL.bit.SOCBSEL = ET_CTR_PRD;
	EPwmRegs->ETPS.bit.SOCBPRD = ET_1ST;
}

void InitTZ(struct EPWM_REGS * EPwmRegs)//Event Trigger Module Initializing
{
	//All Trip-zone Registers are EALLOW protected
	EALLOW;

	//Trip Zone Enable Initializing Using Hi-Impedance Mode
	//One-shot is cleared by external signal as clear register is set
	//EPwmRegs->TZEINT.bit.OST = TZ_ENABLE; //Enable Trip zone One-shot INT.
	//Cycle by Cycle is cleared by next counter when it is 0
	//EPwmRegs->TZEINT.bit.CBC = TZ_ENABLE; //Enable Trip zone Cycle by Cycle INT.

	//Indicates a trip event has occured
	EPwmRegs->TZFLG.bit.OST = TZ_ENABLE;
	//EPwmRegs->TZFLG.bit.CBC = TZ_ENABLE;

	//Trip Zone Control Register (Action for Trip Event)
	EPwmRegs->TZCTL.bit.TZA = TZ_FORCE_LO; // TZ_HIZ : High Impedance Mode
	EPwmRegs->TZCTL.bit.TZB = TZ_FORCE_LO; // TZ_HIZ : High Impedance Mode

	/*
	//One shot mode
	EPwm1Regs.TZFRC.bit.OST=1; //OST 레지스터를 통해 one shot 모드의 활성-비활성 상태 설정
	EPwm1Regs.TZSEL.bit.OSHT1=1; //One shot 모드를 PWM1에서 활성화
	EPwm1Regs.TZCTL.bit.TZA=1; //One shot 모드에서 PWMA 출력을 High 상태로 설정
	EPwm1Regs.TZCTL.bit.TZB=2; //One shot 모드에서 PWMB 출력을 Low 상태로 설정

	//Continuous mode
	EPwm1Regs.TZFRC.bit.CBC = 0 to 1; //CBC 레지스터를 통해 Continuous 모드의 활성-비활성 상태 설정
	EPwm1Regs.TZSEL.bit.CBC1=1; //Continuous 모드를 PWM1에서 활성화
    EPwm1Regs.TZCTL.bit.TZA=1; //Continuous 모드에서 PWMA 출력을 High 상태로 설정
    EPwm1Regs.TZCTL.bit.TZB=2; //Continuous 모드에서 PWMB 출력을 Low 상태로 설정
	*/
	
	EDIS;
}
//===========================================================================
// End of file.
//===========================================================================
