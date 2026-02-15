// TI File $Revision: /main/2 $
// Checkin $Date: March 5, 2007   12:20:33 $
//###########################################################################
//
// FILE:	DSP2833x_GlobalVariableDefs.c
//
// TITLE:	DSP2833x Global Variables and Data Section Pragmas.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.00 $
// $Release Date: September 7, 2007 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Device.h" 
#include "DSP2833x_GlobalPrototypes.h" 


//---------------------------------------------------------------------------
// Define Global Peripheral Variables:
//
//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("AdcRegsFile")
#else
#pragma DATA_SECTION(AdcRegs,"AdcRegsFile");
#endif
volatile struct ADC_REGS AdcRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("AdcMirrorFile")
#else
#pragma DATA_SECTION(AdcMirror,"AdcMirrorFile");
#endif
volatile struct ADC_RESULT_MIRROR_REGS AdcMirror;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CpuTimer0RegsFile")
#else
#pragma DATA_SECTION(CpuTimer0Regs,"CpuTimer0RegsFile");
#endif
volatile struct CPUTIMER_REGS CpuTimer0Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CpuTimer1RegsFile")
#else
#pragma DATA_SECTION(CpuTimer1Regs,"CpuTimer1RegsFile");
#endif
volatile struct CPUTIMER_REGS CpuTimer1Regs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CpuTimer2RegsFile")
#else
#pragma DATA_SECTION(CpuTimer2Regs,"CpuTimer2RegsFile");
#endif
volatile struct CPUTIMER_REGS CpuTimer2Regs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CsmPwlFile")
#else
#pragma DATA_SECTION(CsmPwl,"CsmPwlFile");
#endif
volatile struct CSM_PWL CsmPwl;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("CsmRegsFile")
#else
#pragma DATA_SECTION(CsmRegs,"CsmRegsFile");
#endif
volatile struct CSM_REGS CsmRegs;



//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("DevEmuRegsFile")
#else
#pragma DATA_SECTION(DevEmuRegs,"DevEmuRegsFile");
#endif
volatile struct DEV_EMU_REGS DevEmuRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("DmaRegsFile")
#else
#pragma DATA_SECTION(DmaRegs,"DmaRegsFile");
#endif
volatile struct DMA_REGS DmaRegs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanaRegsFile")
#else
#pragma DATA_SECTION(ECanaRegs,"ECanaRegsFile");
#endif
volatile struct ECAN_REGS ECanaRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanaMboxesFile")
#else
#pragma DATA_SECTION(ECanaMboxes,"ECanaMboxesFile");
#endif
volatile struct ECAN_MBOXES ECanaMboxes;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanaLAMRegsFile")
#else
#pragma DATA_SECTION(ECanaLAMRegs,"ECanaLAMRegsFile");
#endif
volatile struct LAM_REGS ECanaLAMRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanaMOTSRegsFile")
#else
#pragma DATA_SECTION(ECanaMOTSRegs,"ECanaMOTSRegsFile");
#endif
volatile struct MOTS_REGS ECanaMOTSRegs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanaMOTORegsFile")
#else
#pragma DATA_SECTION(ECanaMOTORegs,"ECanaMOTORegsFile");
#endif
volatile struct MOTO_REGS ECanaMOTORegs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanbRegsFile")
#else
#pragma DATA_SECTION(ECanbRegs,"ECanbRegsFile");
#endif
volatile struct ECAN_REGS ECanbRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanbMboxesFile")
#else
#pragma DATA_SECTION(ECanbMboxes,"ECanbMboxesFile");
#endif
volatile struct ECAN_MBOXES ECanbMboxes;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanbLAMRegsFile")
#else
#pragma DATA_SECTION(ECanbLAMRegs,"ECanbLAMRegsFile");
#endif
volatile struct LAM_REGS ECanbLAMRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanbMOTSRegsFile")
#else
#pragma DATA_SECTION(ECanbMOTSRegs,"ECanbMOTSRegsFile");
#endif
volatile struct MOTS_REGS ECanbMOTSRegs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECanbMOTORegsFile")
#else
#pragma DATA_SECTION(ECanbMOTORegs,"ECanbMOTORegsFile");
#endif
volatile struct MOTO_REGS ECanbMOTORegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm1RegsFile")
#else
#pragma DATA_SECTION(EPwm1Regs,"EPwm1RegsFile");
#endif
volatile struct EPWM_REGS EPwm1Regs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm2RegsFile")
#else
#pragma DATA_SECTION(EPwm2Regs,"EPwm2RegsFile");
#endif
volatile struct EPWM_REGS EPwm2Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm3RegsFile")
#else
#pragma DATA_SECTION(EPwm3Regs,"EPwm3RegsFile");
#endif
volatile struct EPWM_REGS EPwm3Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm4RegsFile")
#else
#pragma DATA_SECTION(EPwm4Regs,"EPwm4RegsFile");
#endif
volatile struct EPWM_REGS EPwm4Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm5RegsFile")
#else
#pragma DATA_SECTION(EPwm5Regs,"EPwm5RegsFile");
#endif
volatile struct EPWM_REGS EPwm5Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EPwm6RegsFile")
#else
#pragma DATA_SECTION(EPwm6Regs,"EPwm6RegsFile");
#endif
volatile struct EPWM_REGS EPwm6Regs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap1RegsFile")
#else
#pragma DATA_SECTION(ECap1Regs,"ECap1RegsFile");
#endif
volatile struct ECAP_REGS ECap1Regs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap2RegsFile")
#else
#pragma DATA_SECTION(ECap2Regs,"ECap2RegsFile");
#endif
volatile struct ECAP_REGS ECap2Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap3RegsFile")
#else
#pragma DATA_SECTION(ECap3Regs,"ECap3RegsFile");
#endif
volatile struct ECAP_REGS ECap3Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap4RegsFile")
#else
#pragma DATA_SECTION(ECap4Regs,"ECap4RegsFile");
#endif
volatile struct ECAP_REGS ECap4Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap5RegsFile")
#else
#pragma DATA_SECTION(ECap5Regs,"ECap5RegsFile");
#endif
volatile struct ECAP_REGS ECap5Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ECap6RegsFile")
#else
#pragma DATA_SECTION(ECap6Regs,"ECap6RegsFile");
#endif
volatile struct ECAP_REGS ECap6Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EQep1RegsFile")
#else
#pragma DATA_SECTION(EQep1Regs,"EQep1RegsFile");
#endif
volatile struct EQEP_REGS EQep1Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("EQep2RegsFile")
#else
#pragma DATA_SECTION(EQep2Regs,"EQep2RegsFile");
#endif
volatile struct EQEP_REGS EQep2Regs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioCtrlRegsFile")
#else
#pragma DATA_SECTION(GpioCtrlRegs,"GpioCtrlRegsFile");
#endif
volatile struct GPIO_CTRL_REGS GpioCtrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioDataRegsFile")
#else
#pragma DATA_SECTION(GpioDataRegs,"GpioDataRegsFile");
#endif
volatile struct GPIO_DATA_REGS GpioDataRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("GpioIntRegsFile")
#else
#pragma DATA_SECTION(GpioIntRegs,"GpioIntRegsFile");
#endif
volatile struct GPIO_INT_REGS GpioIntRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("I2caRegsFile")
#else
#pragma DATA_SECTION(I2caRegs,"I2caRegsFile");
#endif
volatile struct I2C_REGS I2caRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("McbspaRegsFile")
#else
#pragma DATA_SECTION(McbspaRegs,"McbspaRegsFile");
#endif
volatile struct MCBSP_REGS McbspaRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("McbspbRegsFile")
#else
#pragma DATA_SECTION(McbspbRegs,"McbspbRegsFile");
#endif
volatile struct MCBSP_REGS McbspbRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("PieCtrlRegsFile")
#else
#pragma DATA_SECTION(PieCtrlRegs,"PieCtrlRegsFile");
#endif
volatile struct PIE_CTRL_REGS PieCtrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("PieVectTableFile")
#else
#pragma DATA_SECTION(PieVectTable,"PieVectTableFile");
#endif
struct PIE_VECT_TABLE PieVectTable;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("SciaRegsFile")
#else
#pragma DATA_SECTION(SciaRegs,"SciaRegsFile");
#endif
volatile struct SCI_REGS SciaRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ScibRegsFile")
#else
#pragma DATA_SECTION(ScibRegs,"ScibRegsFile");
#endif
volatile struct SCI_REGS ScibRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("ScicRegsFile")
#else
#pragma DATA_SECTION(ScicRegs,"ScicRegsFile");
#endif
volatile struct SCI_REGS ScicRegs;


//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("SpiaRegsFile")
#else
#pragma DATA_SECTION(SpiaRegs,"SpiaRegsFile");
#endif
volatile struct SPI_REGS SpiaRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("SysCtrlRegsFile")
#else
#pragma DATA_SECTION(SysCtrlRegs,"SysCtrlRegsFile");
#endif
volatile struct SYS_CTRL_REGS SysCtrlRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("FlashRegsFile")
#else
#pragma DATA_SECTION(FlashRegs,"FlashRegsFile");
#endif
volatile struct FLASH_REGS FlashRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("XIntruptRegsFile")
#else
#pragma DATA_SECTION(XIntruptRegs,"XIntruptRegsFile");
#endif
volatile struct XINTRUPT_REGS XIntruptRegs;

//----------------------------------------
#ifdef __cplusplus
#pragma DATA_SECTION("XintfRegsFile")
#else
#pragma DATA_SECTION(XintfRegs,"XintfRegsFile");
#endif


volatile struct XINTF_REGS XintfRegs;



/*------------------------------*/
/*  Protection Variables        */
/*------------------------------*/

/* ADC Calibration*/
	float yH = 0.;
	float yL = 0.;
	float CalGain = 0.;
	float CalOffset = 0.;
	float CalGainError = 0.;
	float CalOffsetError = 0.;
	long Cal_Offset_Chk = 0;

/* ADC Offset Calculate */

	float A1OffsetSum=0.;
    float A2OffsetSum=0.;
    float A3OffsetSum=0.;
    float A4OffsetSum=0.;

    float A1OffsetCal=0.;
    float A2OffsetCal=0.;
    float A3OffsetCal=0.;
    float A4OffsetCal=0.;

	long CntSequenceTime = 0;


/* easy_dsp */
	void *data_ptr[99];

	int DA_Code_ch0 = 0;
	int DA_Code_ch1 = 0;
	int DA_Code_ch2 = 0;
	int DA_Code_ch3 = 0;

	float DA_Data_ch0 = 0;
	float DA_Data_ch1 = 0;
	float DA_Data_ch2 = 0;
	float DA_Data_ch3 = 0;

	float DA_ScaleIn_ch0 = 0;
	float DA_ScaleIn_ch1 = 0;
	float DA_ScaleIn_ch2 = 0;
	float DA_ScaleIn_ch3 = 0;

	float DA_Offset_ch0 = 0;
	float DA_Offset_ch1 = 0;
	float DA_Offset_ch2 = 0;
	float DA_Offset_ch3 = 0;

	float DA_Scale_ch0 = 0;
	float DA_Scale_ch1 = 0;
	float DA_Scale_ch2 = 0;
	float DA_Scale_ch3 = 0;

	float DA_Mid_ch0 = 0;
	float DA_Mid_ch1 = 0;
	float DA_Mid_ch2 = 0;
	float DA_Mid_ch3 = 0;

	float DA_Scale[4];

	int  dac_data0 = 0, dac_data1 = 0, dac_data2 = 0, dac_data3 = 0;
	float fdummy = 0.;


/* ePWM Setting */
	Uint32 CMP1 = 0; //450//Init Vaiue//375
	Uint32 CMP2 = 0;
	Uint32 CMP3 = 0;


/*--------------------------------------------------------------------------------*/
/*------2025 800V PFC---------------------------------------*/
/*---------------------------------------*/

/*------Variable---------------------------------*/

	//Operation Variable
	int flag_SystemOK = 0;
	int flag_PLLDone = 0;
	int flag_PreChgDone = 0;
	int OPMode_Code = 0;
	int Fault_Code = 0;

	int BtnCountA = 0;
	int BtnCountB = 0;

	//Sensor Calibration
    float VaGain = 0.2777;
    float VbGain = 0.2750;
    float VcGain = 0.2750;

    float VaADCOffset = 2070;
    float VbADCOffset = 2060;
    float VcADCOffset = 2070;

    float IaGain = -0.028;  //ADCRESULT6
    float IcGain = -0.028;  //ADCRESULT8
    float IaADCOffset = 2064;
    float IcADCOffset = 2056; // 2088
    float IbGain = -0.035;
    float IbADCOffset = 2068;

     //float VdcGain = 0.480;  //ADCRESULT9 -> dead
     //float VdcADCOffset = 2065; // ADCRESULT9
     //float VdcGain = 0.644; // ADCRESULT11 // 0.455 0.656 ->dead
     //float VdcADCOffset = 2060; // ADCRESULT11
     float VdcGain = 0.644; // ADCRESULT13
     float VdcADCOffset = 2069; // ADCRESULT13


    float Va_grid = 0.;
    float Vb_grid = 0.;
    float Vc_grid = 0.;
    float Ia_grid = 0.;
    float Ib_grid = 0.;
    float Ic_grid = 0.;
    float Vdc = 0.;


    /* PLL */
    float Vds_grid = 0.;
    float Vqs_grid = 0.;
    float Vde_grid = 0.;
    float Vqe_grid = 0.;
    float theta_grid = 0.;
    float Em, Zeta, Wn, Wc = 0.;
    float Vde_LPF = 0.;
    float Vde_old = 0.;
    float Vde_err = 0.;
    float Kp_PLL, Ki_PLL = 0.;
    float Vde_err_int = 0.;
    float Wde_ref = 0.;
    float W_rated = 0.;
    float Wde_ref_fb = 0.;

    float Vqe_old = 0.;
    float Vqe_LPF = 0.;


    //Cal Avg Voltage
    int CalculatingAvg_Count = 0;
    float Avg_Vqe_LPF = 0.;
    float Avg_Vde_LPF = 0.;
    float Avg_Vdc = 0.;
    float PreAvg_Vqe_LPF = 0.;
    float PreAvg_Vde_LPF = 0.;
    float PreAvg_Vdc = 0.;
    float CalculatingAvg_Vqe_LPF = 0.;
    float CalculatingAvg_Vde_LPF = 0.;
    float CalculatingAvg_Vdc = 0.;


    //-------------------------- Current Controller -------------------------------------------------
    /* CurrentControl */
    float Vde_fb = 0.;
    float Vqe_fb = 0.;
    float Vde_ff = 0.;
    float Vqe_ff = 0.;

    float Iqs_Grid = 0.;
    float Ids_Grid = 0.;
    float Iqe_Grid = 0.;
    float Ide_Grid = 0.;

    float Iqe_Grid_err = 0.;
    float Ide_Grid_err = 0.;
    float Iqe_Grid_err_int = 0.;
    float Ide_Grid_err_int = 0.;
    float Iqe_Grid_ref = 0.;
    float Ide_Grid_ref = 0.;

    float Cur_Ki_cc = 0.;
    float Cur_Kp_cc = 0.;

    float CurrentController_AntiWindUp_LimitVal_Upper; //Unit : [Volt]
    float CurrentController_AntiWindUp_LimitVal_Lower = 0.0;

    //-------------------------- DCLINK Controller -------------------------------------------------
    float User_Vout_Target = 0.;
    float Vout_ref = 0.;
    float Vout_err = 0.;
    float Vout_err_int = 0.;
    float Vout_Ki_cc = 0.;
    float Vout_Kp_cc = 0.;
    float Vdc_LPF = 0.;
    float Vdc_LPF_old  = 0.;
    float VDC_LPF_1 = 0.;
    float VDC_LPF_2 = 0.;
    int V_Count = 0;


    //-------------------------- Inverter Modulation -------------------------------------------------
    float t_cnt = 0.;

    /* PWM */
    float carrierCNT = 0. ;
    float carrierWave = 0.;

    /* Modulation */
    float Vde_ref = 0.;
    float Vqe_ref = 0.;
    float Vds_ref = 0.;
    float Vqs_ref = 0.;

    float Vas_ref = 0.;
    float Vbs_ref = 0.;
    float Vcs_ref = 0;
    float Van_ref = 0.;
    float Vbn_ref = 0.;
    float Vcn_ref = 0.;

    float VOut_max = 0.;
    float VOut_mid = 0.;
    float VOut_min = 0.;
    float Vsn = 0.;

    Uint16 CMP1_Ref = 0;
    Uint16 CMP2_Ref = 0;
    Uint16 CMP3_Ref = 0;

/*--------------------------------------------------------------------------------*/


//===========================================================================
// End of file.
//===========================================================================
