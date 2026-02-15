// TI File $Revision: /main/11 $
// Checkin $Date: May 12, 2008   14:30:08 $
//###########################################################################
//
// FILE:   DSP2833x_GlobalPrototypes.h
//
// TITLE:  Global prototypes for DSP2833x Examples
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 1, 2008 $
//###########################################################################

#ifndef DSP2833x_GLOBALPROTOTYPES_H
#define DSP2833x_GLOBALPROTOTYPES_H
#define LED_ON GpioDataRegs.GPACLEAR.bit.GPIO30 = 1; 
#define LED_OFF GpioDataRegs.GPASET.bit.GPIO30 = 1;
#define LED_TOGGLE GpioDataRegs.GPATOGGLE.bit.GPIO30 = 1;
#define GATINGON EPwm1Regs.TZCLR.bit.OST = TZ_ENABLE; \
				EPwm2Regs.TZCLR.bit.OST = TZ_ENABLE; \
				EPwm3Regs.TZCLR.bit.OST = TZ_ENABLE; 
#define GATINGOFF EPwm1Regs.TZFRC.bit.OST = 1; EPwm2Regs.TZFRC.bit.OST = 1; EPwm3Regs.TZFRC.bit.OST = 1;


	
#ifdef __cplusplus
extern "C" {
#endif

/*---- shared global function prototypes -----------------------------------*/


/*--------------------------------------------------------------------------------*/
/*------2025 800V PFC---------------------------------------*/
/*---------------------------------------*/

/*------Define---------------------------------*/


#define SoftStart_Slope 30.0 / 20000  // Volt/sec. Divide by 20k(Hz)
#define PreChgRatio 0.88

//Protection at AD Calc.
#define Thershold_GridOverVoltage_Cuttoff 550
#define Thershold_DCOverVoltage_Cuttoff 920
#define Thershold_OverCurrent_Cuttoff 55  //at Peak+
#define Thershold_StartUp_GridVoltage 100


#define Current_Reference_UpperLimit 80 / 3 //at Peak : Iqe
#define Current_Reference_LowerLimit -50 / 3

#define PWM_DeadTime_Ratio 0.035


//Control loop Parameter
#define T_samp            50.e-006
#define T_samp_V          50.e-005
#define GridFreq 60.

//PLL
#define Wc_PLL 200.
//LPF Gain
#define La (2. - Wc_PLL * T_samp) / (2. + Wc_PLL * T_samp)  //PLL LPF
#define Lb (Wc_PLL * T_samp) / (2. + Wc_PLL * T_samp)  //PLL LPF

#define PLL_Antiwindup_Lower 2. * PI * 40
#define PLL_Antiwindup_Upper 2. * PI * 70

//Filter Param
#define BandWidth_Grid_current -3000.
#define React_L    1550.e-006
#define React_R    35.e-003

#define Grid_R    240.e-003
#define Grid_L   400.e-006


//DCLINK Param
#define BandWidth_DClink_Voltage 100.
#define DCLink_C    1530.e-006
#define DCLink_ESR    43.e-003


#define PreChg_MC_Open GpioDataRegs.GPBSET.bit.GPIO32 = 1; GpioDataRegs.GPBSET.bit.GPIO33 = 1;
#define PreChg_MC_Close GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1; GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;

/*------Variable---------------------------------*/
extern int flag_SystemOK, flag_PLLDone, flag_PreChgDone;
extern int OPMode_Code, Fault_Code;
extern int BtnCountA, BtnCountB;


/* ADC Sensing */
extern float VaGain, VaADCOffset, VbGain, VbADCOffset, VcGain, VcADCOffset;
extern float IaGain, IaADCOffset, IcGain, IcADCOffset, IbGain, IbADCOffset;
extern float VdcGain, VdcADCOffset;

extern float Va_grid, Vb_grid, Vc_grid;
extern float Ia_grid, Ib_grid, Ic_grid;
extern float Vdc;

/* PLL */
extern float Vds_grid, Vqs_grid, Vde_grid, Vqe_grid;
extern float theta_grid;
extern float Em, Zeta, Wn, Wc;
extern float Vde_LPF, Vde_old, Vde_err;
extern float Kp_PLL, Ki_PLL, Vde_err_int, Wde_ref;
extern float W_rated, Wde_ref_fb;

extern float Vqe_old, Vqe_LPF;

//-------------------------- Current Controller -------------------------------------------------
/* CurrentControl */
extern float Ide_ref, Iqe_ref, id_err, iq_err, id_err_int, iq_err_int;
extern float Vde_fb, Vqe_fb, Vde_ff, Vqe_ff;
extern float Iqs_Grid, Ids_Grid, Iqe_Grid, Ide_Grid;
extern float Iqe_Grid_err, Ide_Grid_err, Iqe_Grid_err_int, Ide_Grid_err_int, Iqe_Grid_ref, Ide_Grid_ref;
extern float Cur_Ki_cc, Cur_Kp_cc;
extern float CurrentController_AntiWindUp_LimitVal_Upper, CurrentController_AntiWindUp_LimitVal_Lower;
extern float Ia_grid, Ib_grid, Ic_grid;

//-------------------------- DCLINK Controller -------------------------------------------------
extern float User_Vout_Target;
extern float Vout;
extern float Vout_ref, Vout_err, Vout_err_int;
extern float Vout_Ki_cc, Vout_Kp_cc;
extern float Vdc_LPF;
extern float Vdc_LPF_old;
extern float VDC_LPF_1;
extern float VDC_LPF_2;

/* Modulation */
extern float Vde_ref, Vqe_ref, Vds_ref, Vqs_ref;
extern float Vas_ref, Vbs_ref, Vcs_ref;
extern float Van_ref, Vbn_ref, Vcn_ref;
extern float VOut_max, VOut_mid, VOut_min, Vsn;
extern Uint16 CMP1_Ref, CMP2_Ref, CMP3_Ref;

//Cal Avg Voltage
extern int CalculatingAvg_Count;
extern float Avg_Vqe_LPF;
extern float Avg_Vde_LPF;
extern float Avg_Vdc;
extern float PreAvg_Vqe_LPF;
extern float PreAvg_Vde_LPF;
extern float PreAvg_Vdc;
extern float CalculatingAvg_Vqe_LPF;
extern float CalculatingAvg_Vde_LPF;
extern float CalculatingAvg_Vdc;
extern int V_Count;



/*------Function---------------------------------*/
extern void ControlRoutine(void);

extern void SysFaultCheck(void);
extern void PreChargeCap(void);

extern void SoftStartUp(void);
extern void NormalPFCOperation(void);
extern void EmergencyStop(void);

extern void CalculateADC(void);
extern void CurrentControl_PI(void);
extern void VoltageControl_PI(void);
extern void Modulation(void);

extern void Cal_Average_Voltages(void);

extern void KJW(void);


/*--------------------------------------------------------------------------------*/
//Default setting
extern void InitAdc(void);
extern void InitParameter(void);
extern void easyDSP_SCI_Init(void);
extern void CalculateADCOffset(void);
extern void CalculateADCScale(void);
extern void CalibrateADC(void);
extern void Init_Code(void);
extern void Init_eQEP1(void);
extern void Init_eQEP2(void);
extern void InitEPWM(void);
extern void OutputDAC(void);

extern void DMAInitialize(void);
// DMA Channel 1
extern void DMACH1AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH1BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH1TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH1WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH1ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH1(void);
// DMA Channel 2
extern void DMACH2AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH2BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH2TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH2WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH2ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH2(void);
// DMA Channel 3
extern void DMACH3AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH3BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH3TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH3WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH3ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH3(void);
// DMA Channel 4
extern void DMACH4AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH4BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH4TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH4WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH4ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH4(void);
// DMA Channel 5
extern void DMACH5AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH5BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep);
extern void DMACH5TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH5WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH5ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH5(void);
// DMA Channel 6
extern void DMACH6AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source);
extern void DMACH6BurstConfig(Uint16 bsize,Uint16 srcbstep, int16 desbstep);
extern void DMACH6TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep);
extern void DMACH6WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep);
extern void DMACH6ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte);
extern void StartDMACH6(void);

extern void InitPeripherals(void);
#if DSP28_ECANA
extern void InitECan(void);
extern void InitECana(void);
extern void InitECanGpio(void);
extern void InitECanaGpio(void);
#endif // endif DSP28_ECANA
#if DSP28_ECANB
extern void InitECanb(void);
extern void InitECanbGpio(void);
#endif // endif DSP28_ECANB
extern void InitECap(void);
extern void InitECapGpio(void);
extern void InitECap1Gpio(void);
extern void InitECap2Gpio(void);
#if DSP28_ECAP3
extern void InitECap3Gpio(void);
#endif // endif DSP28_ECAP3
#if DSP28_ECAP4
extern void InitECap4Gpio(void);
#endif // endif DSP28_ECAP4
#if DSP28_ECAP5
extern void InitECap5Gpio(void);
#endif // endif DSP28_ECAP5
#if DSP28_ECAP6
extern void InitECap6Gpio(void);
#endif // endif DSP28_ECAP6
extern void InitEPwm(void);
extern void InitEPwmGpio(void);
extern void InitEPwm1Gpio(void);
extern void InitEPwm2Gpio(void);
extern void InitEPwm3Gpio(void);
#if DSP28_EPWM4
extern void InitEPwm4Gpio(void);
#endif // endif DSP28_EPWM4
#if DSP28_EPWM5
extern void InitEPwm5Gpio(void);
#endif // endif DSP28_EPWM5
#if DSP28_EPWM6
extern void InitEPwm6Gpio(void);
#endif // endif DSP28_EPWM6
#if DSP28_EQEP1
extern void InitEQep(void);
extern void InitEQepGpio(void);
extern void InitEQep1Gpio(void);
#endif // if DSP28_EQEP1
#if DSP28_EQEP2
extern void InitEQep2Gpio(void);
#endif // endif DSP28_EQEP2
extern void InitGpio(void);
extern void InitI2CGpio(void);

extern void InitMcbsp(void);
extern void InitMcbspa(void);
extern void delay_loop(void);
extern void InitMcbspaGpio(void);
extern void InitMcbspa8bit(void);
extern void InitMcbspa12bit(void);
extern void InitMcbspa16bit(void);
extern void InitMcbspa20bit(void);
extern void InitMcbspa24bit(void);
extern void InitMcbspa32bit(void);
#if DSP28_MCBSPB
extern void InitMcbspb(void);
extern void InitMcbspbGpio(void);
extern void InitMcbspb8bit(void);
extern void InitMcbspb12bit(void);
extern void InitMcbspb16bit(void);
extern void InitMcbspb20bit(void);
extern void InitMcbspb24bit(void);
extern void InitMcbspb32bit(void);
#endif // endif DSP28_MCBSPB

extern void InitPieCtrl(void);
extern void InitPieVectTable(void);

extern void InitSci(void);
extern void InitSciGpio(void);
extern void InitSciaGpio(void);
#if DSP28_SCIB
extern void InitScibGpio(void);
#endif // endif DSP28_SCIB
#if DSP28_SCIC
extern void InitScicGpio(void);
#endif
extern void InitSpi(void);
extern void InitSpiGpio(void);
extern void InitSpiaGpio(void);
extern void InitSysCtrl(void);
extern void InitTzGpio(void);
extern void InitXIntrupt(void);
extern void XintfInit(void);
extern void InitXintf16Gpio();
extern void InitXintf32Gpio();
extern void InitPll(Uint16 pllcr, Uint16 clkindiv);
extern void InitPeripheralClocks(void);
extern void EnableInterrupts(void);
extern void DSP28x_usDelay(Uint32 Count);
extern void ADC_cal (void);
#define KickDog ServiceDog     // For compatiblity with previous versions
extern void ServiceDog(void);
extern void DisableDog(void);
extern Uint16 CsmUnlock(void);

// DSP28_DBGIER.asm
extern void SetDBGIER(Uint16 dbgier);

//                 CAUTION
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results
extern void InitFlash(void);

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);


//---------------------------------------------------------------------------
// External symbols created by the linker cmd file
// DSP28 examples will use these to relocate code from one LOAD location
// in either Flash or XINTF to a different RUN location in internal
// RAM
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

extern Uint16 XintffuncsLoadStart;
extern Uint16 XintffuncsLoadEnd;
extern Uint16 XintffuncsRunStart;


/* For SC & VC Call */
#define		SC_CALL			10
#define		VC_CALL			10


// System Mode
#define CALIBRATION		0				// Calculate CalGain, CalOffset     
#define CALOFFSET		1				// Calculate Sensing Offset value   
#define CHECKeCAN		2				// Cell to main communication check 
#define CHECKVCS		3				// Check main VCS status            
#define PRECHARGE		4				// Perform DC-Link precharging      
#define READY			5				// Wait for main command            
#define NORMAL			6				// Normal operating status
#define FAULT			9				// System fault

#define OPEN			0
#define	CLOSE			1

#define NUM_CALOFFSET	10000
#define PI 3.14159265
#define	SQRT2 1.414213562
#define INV_SQRT2 0.70710678
#define INV_SQRT3 0.57735026
#define SQRT3 1.7320508

#define ABS(x) ((x>=0)? (x) : -(x))  //���밪�� ����


typedef struct {
	float scale[16];
	int	  offset[16];
	float tmp[16];
	int conv_val[16];
}	ADC;


	/*------------------------------*/
	/*  Protection Variables        */
	/*------------------------------*/

/* ADC Calibration */
	extern float yH;
	extern float yL;
	extern float CalGain;
	extern float CalOffset;
	extern float CalGainError;
	extern float CalOffsetError;
	extern long Cal_Offset_Chk;

/* ADC Offset Calculate */
	extern float A1OffsetSum;
    extern float A2OffsetSum;
    extern float A3OffsetSum;
    extern float A4OffsetSum;

    extern float A1OffsetCal;
    extern float A2OffsetCal;
    extern float A3OffsetCal;
    extern float A4OffsetCal;

/* Limit value */
	extern long CntSequenceTime;

/* easy_dsp */
	extern void *data_ptr[99];

	extern int DA_Code_ch0;
	extern int DA_Code_ch1;
	extern int DA_Code_ch2;
	extern int DA_Code_ch3;

	extern float DA_Data_ch0;
	extern float DA_Data_ch1;
	extern float DA_Data_ch2;
	extern float DA_Data_ch3;

	extern float DA_ScaleIn_ch0;
	extern float DA_ScaleIn_ch1;
	extern float DA_ScaleIn_ch2;
	extern float DA_ScaleIn_ch3;

	extern float DA_Offset_ch0;
	extern float DA_Offset_ch1;
	extern float DA_Offset_ch2;
	extern float DA_Offset_ch3;

	extern float DA_Scale_ch0;
	extern float DA_Scale_ch1;
	extern float DA_Scale_ch2;
	extern float DA_Scale_ch3;

	extern float DA_Scale[4];

	extern int  dac_data0, dac_data1, dac_data2, dac_data3;
	extern float fdummy;

/* ePWM Setting */
	extern Uint32 CMP1;
	extern Uint32 CMP2;
	extern Uint32 CMP3;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of DSP2833x_GLOBALPROTOTYPES_H

//===========================================================================
// End of file.
//===========================================================================
