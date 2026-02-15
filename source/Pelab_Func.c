// TI File $Revision: Pelab_Func.c
//
//
// FILE:   Pelab_Func.c
// TITLE:  Functions for 3-Phase PFC Control.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 11, 2014 $
//###########################################################################
//
// 핵심 실행 흐름 
//   1) CalculateADC()  : ADC 읽기/스케일링/보호 판단/평균값 계산/ PLL 업데이트
//   2) UserButtonRead(): 사용자 버튼(시작/정지/비상정지) 읽고 OPMode 전환
//   3) ControlRoutine(): PLL/Precharge 완료 전에는 대기, 이후 모드별 제어 수행
//
//###########################################################################

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "DSP2833x_GlobalPrototypes.h"
#include <math.h>

//---------------------------------------------------------------------------
// Declare each Function:
//---------------------------------------------------------------------------


/*
 * Fault_Code Define
 *
 *  11 : DC OverVoltage (ADC)
 *  12 : AC OverVoltage (ADC)
 *  13 : AC OverCurrent (ADC)
 *
 *  User Error 100~
 *  100 : Emergency Stop
 *  110 : Target Voltage is Low
 *
 *
 */
//====================================================================================
// 1) InitParameter(): 제어 파라미터 초기화
//====================================================================================
void InitParameter() 
{
	// ---------------------------
    // (1) PLL 파라미터 초기화
    // ---------------------------
	Em = 300; 
	Wn = 200.;
	Zeta = 0.707, Wc = 1. + 2. * Zeta * Wn;
	
	// Kp_PLL, Ki_PLL: PLL PI 게인
    // (Em + 0.1)로 나누는 것은 Em이 0일 때 분모 보호 목적
	Kp_PLL = (2. * Zeta * Wn) / (Em + 0.00001);
	Ki_PLL = (Wn * Wn) / ((Em + 0.00001) * Wc);
	
	// W_rated: 그리드 정격 각속도 [rad/s]
	W_rated = 2. * PI * GridFreq;

    // ---------------------------
    // (2) Controller 전류/전압 게인 초기화
    // ---------------------------
	// Current Controller
	Cur_Ki_cc = React_R * BandWidth_Grid_current;
	Cur_Kp_cc = React_L* BandWidth_Grid_current;
	//Voltage Controller
	 //Vout_Ki_cc = DCLink_ESR * 3 * BandWidth_DClink_Voltage,
	 //Vout_Kp_cc = /*1.7 **/ DCLink_C * 0.0001  * BandWidth_DClink_Voltage;
	 //Vout_Kp_cc = DCLink_C * 800 * 0.0001 * BandWidth_DClink_Voltage; // 0.01224
	 //Vout_Ki_cc = Vout_Kp_cc * BandWidth_DClink_Voltage; // 1.224
	  Vout_Kp_cc = 311 * 3 * 0.5 * 0.00125 * DCLink_C * BandWidth_DClink_Voltage; // 0.089
	  Vout_Ki_cc = Vout_Kp_cc * BandWidth_DClink_Voltage * 0.2; // 1.78

	 VDC_LPF_1 = 0.952380;                  //   1/(1+0.00005*1000);
	 VDC_LPF_2 = 0.0476190;                  // 0.00005*1000/(1+0.00005*1000)

	//Just for Dev.
	flag_SystemOK = 1;
	//PreChg_MC_Close
}

//====================================================================================
// 2) CalculateADC(): 센싱/보호/변환/PLL 업데이트
//====================================================================================
void CalculateADC() 
{
	// ---------------------------
    // (1) 3상 전압 ADC 읽기 및 Offset/Gain 적용)
    // ---------------------------
    // ADCRESULTx >> 4 : 12-bit 정렬 및 하위비트 제거 용도로 사용
	Va_grid = ((Uint16) (AdcRegs.ADCRESULT0 >> 4) - VaADCOffset) * VaGain;
	Vb_grid = ((Uint16) (AdcRegs.ADCRESULT2 >> 4) - VbADCOffset) * VbGain;
	Vc_grid = ((Uint16) (AdcRegs.ADCRESULT8 >> 4) - VcADCOffset) * VcGain;

	// ---------------------------
    // (2) 3상 전류 ADC 읽기 
    // ---------------------------
	Ia_grid = ((Uint16) (AdcRegs.ADCRESULT10 >> 4) - IaADCOffset) * IaGain;
	Ic_grid = ((Uint16) (AdcRegs.ADCRESULT6 >> 4) - IcADCOffset) * IcGain;
	Ib_grid = -(Ia_grid + Ic_grid);
	//Ib_grid = ((Uint16) (AdcRegs.ADCRESULT1 >> 4) - IbADCOffset) * IbGain;
	
	// ---------------------------
    // (3) DC 링크 전압 ADC 읽기
    // ---------------------------
	Vdc = (((Uint16) (AdcRegs.ADCRESULT13 >> 4) - VdcADCOffset) * VdcGain) * 0.7395288 + 0.6491016;
    Vdc_LPF = VDC_LPF_1 * Vdc_LPF_old + VDC_LPF_2 * Vdc;
    Vdc_LPF_old = Vdc_LPF;
    
	// abc_to_dqs: 3상 abc 좌표계→ 정지좌표계(dqs)
    // dqs_to_dqe: 정지좌표계 → 동기좌표계(dqe), theta_grid 사용
	abc_to_dqs(Ia_grid, Ib_grid, Ic_grid, &Ids_Grid, &Iqs_Grid);
	dqs_to_dqe(Ids_Grid, Iqs_Grid, theta_grid, &Ide_Grid, &Iqe_Grid);
	
	// 전류 제어기 출력 전압을 ±Vdc 범위로 제한하는 의도
	CurrentController_AntiWindUp_LimitVal_Upper = Vdc; // * 0.60;
	CurrentController_AntiWindUp_LimitVal_Lower = -1 * Vdc; // * 0.60;

	// ---------------------------
    // (6) 보호 로직: OverVoltage/OverCurrent 발생 시 PWM 차단 + fault Flag 발생 
    // ---------------------------
	if (Vdc > Thershold_DCOverVoltage_Cuttoff) 
	{
		PWM_CUT();
		flag_SystemOK = 0;
		Fault_Code = 11;
	}

	if (ABS(Va_grid) > Thershold_GridOverVoltage_Cuttoff
			|| ABS(Vb_grid) > Thershold_GridOverVoltage_Cuttoff
			|| ABS(Vc_grid) > Thershold_GridOverVoltage_Cuttoff) {
		PWM_CUT();
		flag_SystemOK = 0;
		Fault_Code = 12;
	}

	if (ABS(Ia_grid) > Thershold_OverCurrent_Cuttoff
			|| ABS(Ib_grid) > Thershold_OverCurrent_Cuttoff
			|| ABS(Ic_grid) > Thershold_OverCurrent_Cuttoff) {
		PWM_CUT();
		flag_SystemOK = 0;
		Fault_Code = 13;
	}

	Cal_Average_Voltages();
	
}

//====================================================================================
// 3) ControlRoutine(): 전체 제어 시퀀스 + 보호 로직 
//====================================================================================

void ControlRoutine() 
{
	// 이 함수가 ISR에서 20kHz로 호출됨
	// (1) 센싱/보호 수행
	CalculateADC();
	// (2) PLL 수행
	PLL();
	// (3) 외부 버튼 읽기 -> Operation Mode 전환
	UserButtonRead();

	// (4) 시스템 OK + Fault 없음일 때만 동작 진행
	if (flag_SystemOK && Fault_Code == 0) 
	{
		// (4-1) PLL/프리차지가 완료되지 않았으면 PLL/프리차지 우선 수행
		if (!flag_PreChgDone || !flag_PLLDone) 
		{
			if(!flag_PLLDone)
			{
				PLLCheck(); 	// PLL lock 완료 조건 검사(완료 시 flag_PLLDone=1)
			}
			else
			{
			    PreChargeCap(); // 프리차지 완료 조건 검사(완료 시 flag_PreChgDone=1)
			}
			OPMode_Code = 0;
		} else 
		{
			// (4-2) PLL & 프리차지 완료 후: OPMode에 따라 모드 수행
			switch (OPMode_Code) 
			{
			case 0: 
				//대기
				DELAY_US(1);
				break;
			case 1: 
				//제어기 내부 변수 초기화
				//제어기 적분기 초기화//Reset PFC Variables
				InitPFC();
				break;
			case 2:
				//소프트 스타트: DC 링크 목표 전압을 램프 함수로 상승
				SoftStartUp();
				break;
            case 3:
				//정상운전: 전압루프/전류루프/SVPWM/CMPA.CMPB 갱신
                NormalPFCOperation();
                break;
            case 4:
				//정지
                StopPFCOperation();
                break;
			default:
				// 정의되지 않은 상태는 긴급정지
				EmergencyStop();
				break;
			}
		}
	} 
	else 
	{
		//(5) 시스템 비정상 또는 Fault 발생 시 시스템 정지
	    PWM_CUT();
		EmergencyStop();
	}
}
//====================================================================================
// 4) PLLCheck(): PLL 동기 완료 여부를 "d축""q축" 전압 기반으로 판단
//====================================================================================
void PLLCheck()
{
	// Avg_Vqe_LPF, Avg_Vde_LPF는 PLL 내부에서 구한 전압 dq성분의 크기
    // 의도:
    //  - q축 전압이 일정 이상이면 그리드 입력 전압 존재
    //  - d축 전압이 거의 0이면 PLL 완료 판단
	if(PreAvg_Vqe_LPF > Thershold_StartUp_GridVoltage && Avg_Vqe_LPF > Thershold_StartUp_GridVoltage)
	{		
		if(ABS(PreAvg_Vde_LPF) < 3 && ABS(Avg_Vde_LPF) < 3)
		{
            flag_PLLDone = 1;
		}
	}
}
//====================================================================================
// 5) PreChargeCap(): DC 링크 프리차지 조건 만족 시 릴레이 및 컨택터 투입
//====================================================================================
void PreChargeCap(void) 
{
	// 그리드 전압이 충분히 들어온 상태에서만 프리차지를 허용
    if (Avg_Vqe_LPF > Thershold_StartUp_GridVoltage)
    {
		// PreAvg_Vdc가 일정 이상이면 프리차지 완료로 판단
        // (Avg_Vqe_LPF * 1.7) ≈ 상전압 -> 선간전압 변환 
        if (PreAvg_Vdc > Avg_Vqe_LPF * 1.7 * PreChgRatio)
        {
            PreChg_MC_Close ;				//프리차지 시  MC(컨택터) 닫기
            Vout_ref = Avg_Vqe_LPF * 1.7;	// DC링크 초기 충전 전압 
            flag_PreChgDone = 1;			// 프리차지 완료 Flag 발생 
        }
    }
}
//====================================================================================
// 6) InitPFC(): PFC 제어 변수 및 적분기 초기화
//====================================================================================

void InitPFC(void) 
{
	 // PI 적분기 초기화
	Iqe_Grid_err_int = 0;
	Ide_Grid_err_int = 0;
	Vout_err_int = 0;
	Vout_ref = 0;

	// DC-Link 전압 초기값은 현재 DC 전압보다 낮지 않도록 설정
    if(Avg_Vdc > Vout_ref){
        Vout_ref = Avg_Vdc;
    }
	// User_Vout_Target 목표 지령 전압 유효성 검사 
    // (1)전압지령이 계통 전압 기반 최소치보다 낮으면 fault 발생 
	if (User_Vout_Target < Avg_Vqe_LPF * 1.7) 
	{
		flag_SystemOK = 0;
		Fault_Code = 90;
	}
	// (2)전압지령이 커패시터 내압 범위를 넘어서면 fault 발생 
	else if (User_Vout_Target > 900)
	{
	    flag_SystemOK = 0;
        Fault_Code = 91;
	}
	// (3)정상지령이면 소프트스타트 단계로 진입
	else
	{
	    OPMode_Code = 2;
	}
}
//====================================================================================
// 7) SoftStartUp(): DC 링크 목표 전압을 램프 함수 기울기로 증가시켜 운전 시작
//====================================================================================
void SoftStartUp(void) 
{
	if (Vout_ref < User_Vout_Target) 
	{
		// 매 ISR마다 램프 함수 증가 
		Vout_ref  +=  SoftStart_Slope;	   
	} 
	else 
	{
		// 목표 도달 → 정상운전
		Vout_ref = User_Vout_Target;
		OPMode_Code = 3;
	}
	// 소프트스타트 중에도 제어 함수 동작 수행
	NormalPFCOperation();
}
//====================================================================================
// 8) NormalPFCOperation(): 정상 운전 루프(전압루프/전류루프/SVPWM/PWM)
//====================================================================================
void NormalPFCOperation(void) 
{
	
	SysFaultCheck_Cycle();
	if(V_Count == 10)
	{
	    V_Count = 0;
	}

	if(V_Count == 0)
	{
	    VoltageControl_PI(); 	// 전압루프(외부루프): Vdc 조절 → Iq_ref 생성
	}
    CurrentControl_PI();		// 전류루프(내부루프): Id/Iq 조절 → Vdq_ref 생성
    Modulation();				// 변조: Vdq_ref → duty/CMP 생성
    PWM_OUTPUT();				// PWM 반영
    V_Count++;
}
//====================================================================================
// 9) StopPFCOperation(), EmergencyStop(): PWM 차단 및 상태 리셋
//====================================================================================
void StopPFCOperation()
{
    PWM_CUT();
	OPMode_Code = 0;
	Vout_ref = Avg_Vqe_LPF * 1.7;
}
void EmergencyStop(void) 
{
	PWM_CUT();
	flag_SystemOK = 0;
	OPMode_Code = 0;
}
//====================================================================================
// 10) UserButtonRead(): 사용자 버튼 처리
//====================================================================================
void UserButtonRead(void)
{
	if(!GpioDataRegs.GPBDAT.bit.GPIO50){  //Emergency BTN
		flag_SystemOK = 0;
		Fault_Code = 100;
	}else{
		//For Dev Only
		flag_SystemOK = 1;
		Fault_Code = 0;
	}

	if(GpioDataRegs.GPBDAT.bit.GPIO51){
			//BTN1:Start Pressed
			BtnCountA += 1;
		}else{
			BtnCountA -= 2;
		}

	if(GpioDataRegs.GPBDAT.bit.GPIO52){
			//BTN2:Stop Pressed
			BtnCountB += 1;
		}else{
			BtnCountB -= 2;
		}

	//Processing
	//BTN1 : Start
	if (BtnCountA == 999) {
		//Pressing : Exceed Just 1 Time
		if (OPMode_Code == 0) {
			OPMode_Code = 1;
		}
	} else if (BtnCountA > 999) {
		BtnCountA = 1000;
		//Pressed
	} else if (BtnCountA <= 0) {
		BtnCountA = 0;
		//Released
	}

	//BTN2 : Stop
	if (BtnCountB == 999) {
		//Pressing : Exceed Just 1 Time
		if (OPMode_Code == 3) {
			OPMode_Code = 4;
		}
	} else if (BtnCountB > 999) {
		BtnCountB = 1000;
		//Pressed
	} else if (BtnCountB <= 0) {
		BtnCountB = 0;
		//Released
	}
}

//====================================================================================
// 11) PLL(): 그리드 전압을 dq로 변환하여 d축=0 되도록 PI 제어 수행 
//====================================================================================

void PLL()
 {
	// abc_to_dqs: 정지좌표(dqs) 성분
	// abc_to_dqe: 동기좌표(dqe) 성분
	abc_to_dqs(Va_grid, Vb_grid, Vc_grid, &Vds_grid, &Vqs_grid);
	abc_to_dqe(Va_grid, Vb_grid, Vc_grid, theta_grid, &Vde_grid, &Vqe_grid);

	// Vde, Vqe에 저역통과 필터 적용
	Vde_LPF = La * Vde_LPF + Lb * (Vde_grid + Vde_old);
	Vde_old = Vde_grid;
	Vqe_LPF = La * Vqe_LPF + Lb * (Vqe_grid + Vqe_old);
	Vqe_old = Vqe_grid;
	
	// PLL 에러: d축 전압을 0으로 만들도록 제어
	Vde_err = 0. - Vde_LPF;
	Vde_err_int += (Vde_err) * T_samp;
	
	// PI 출력:
	Wde_ref_fb = Kp_PLL * Vde_err + Ki_PLL * Vde_err_int;
	Wde_ref = Wde_ref_fb + W_rated;

	// Anti-windup: Wde_ref를 특정 범위로 제한
	if (Wde_ref > PLL_Antiwindup_Upper) {
		float over_Value = Wde_ref - PLL_Antiwindup_Upper;
		Wde_ref = PLL_Antiwindup_Upper;
		Vde_err_int -= over_Value / Ki_PLL;
	} else if (Wde_ref < PLL_Antiwindup_Lower) {
		float over_Value = Wde_ref - PLL_Antiwindup_Lower;
		Wde_ref = PLL_Antiwindup_Lower;
		Vde_err_int -= over_Value / Ki_PLL;
	}
	
	// 각도 업데이트
	theta_grid += Wde_ref * T_samp;
	
	// 각도 (-2π ~ +2π 범위 유지)
	if (theta_grid > PI) 
	{
		theta_grid -= 2. * PI;
	} 
	else if (theta_grid < -PI) 
	{
		theta_grid += 2. * PI;
	}
}

void SysFaultCheck_Cycle(void) {

}

//====================================================================================
// 12) CurrentControl_PI(): dq 전류 PI 제어 → dq 전압 지령
//====================================================================================

void CurrentControl_PI() 
{
	// 오차
	Ide_Grid_err = Ide_Grid_ref - Ide_Grid;
	Iqe_Grid_err = Iqe_Grid_ref - Iqe_Grid;
	// 적분
	Ide_Grid_err_int += Ide_Grid_err * T_samp;
	Iqe_Grid_err_int += Iqe_Grid_err * T_samp;
	// PI 연산
	Vde_fb = Cur_Kp_cc * Ide_Grid_err + Cur_Ki_cc * Ide_Grid_err_int;
	Vqe_fb = Cur_Kp_cc * Iqe_Grid_err + Cur_Ki_cc * Iqe_Grid_err_int;
	
	//Anti Wind up 연산
	//d축 연산
	if (Vde_fb > CurrentController_AntiWindUp_LimitVal_Upper) {
		float over_Value = Vde_fb - CurrentController_AntiWindUp_LimitVal_Upper;
		Vde_fb = CurrentController_AntiWindUp_LimitVal_Upper;
		Ide_Grid_err_int -= over_Value / Cur_Ki_cc;
	} else if (Vde_fb < CurrentController_AntiWindUp_LimitVal_Lower) {
		float over_Value = Vde_fb - CurrentController_AntiWindUp_LimitVal_Lower;
		Vde_fb = CurrentController_AntiWindUp_LimitVal_Lower;
		Ide_Grid_err_int -= over_Value / Cur_Ki_cc;
	}

	//Anti Wind up 연산
	//q축 연산
	if (Vqe_fb > CurrentController_AntiWindUp_LimitVal_Upper) {
		float over_Value = Vqe_fb - CurrentController_AntiWindUp_LimitVal_Upper;
		Vqe_fb = CurrentController_AntiWindUp_LimitVal_Upper;
		Iqe_Grid_err_int -= over_Value / Cur_Ki_cc;
	} else if (Vqe_fb < CurrentController_AntiWindUp_LimitVal_Lower) {
		float over_Value = Vqe_fb - CurrentController_AntiWindUp_LimitVal_Lower;
		Vqe_fb = CurrentController_AntiWindUp_LimitVal_Lower;
		Iqe_Grid_err_int -= over_Value / Cur_Ki_cc;
	}
	
	// FeedForward
	Vde_ff = Vde_LPF; 
	Vqe_ff = Vqe_LPF;
	
	// 최종 dq 전압 지령
	Vde_ref = (Vde_fb + Vde_ff);
	Vqe_ref = (Vqe_fb + Vqe_ff);
}

//====================================================================================
// 13) VoltageControl_PI(): DC 링크 전압 PI → Iq_ref 생성 (유효 전류 지령)
//====================================================================================
void VoltageControl_PI() 
{
	
	// DC 링크 전압 오차
	Vout_err = Vout_ref - Vdc;
	Vout_err_int += Vout_err * T_samp_V;
	// PI 출력	
	Iqe_Grid_ref = (Vout_Kp_cc * Vout_err + Vout_Ki_cc * Vout_err_int);
	// Iq anti-windup
	if (Iqe_Grid_ref > Current_Reference_UpperLimit) {
		float over_Value = Iqe_Grid_ref - Current_Reference_UpperLimit;
		Iqe_Grid_ref = Current_Reference_UpperLimit;
		Vout_err_int -= over_Value / Vout_Ki_cc;
	} else if (Iqe_Grid_ref < Current_Reference_LowerLimit) {
		float over_Value = Iqe_Grid_ref - Current_Reference_LowerLimit;
		Iqe_Grid_ref = Current_Reference_LowerLimit;
		Vout_err_int -= over_Value / Vout_Ki_cc;
	}

	// 역률 1 목표: d축 전류 지령 0
	Ide_Grid_ref = 0;
}

void Modulation() 
{
	// dq -> dqs -> abc
	dqe_to_dqs(Vde_ref, Vqe_ref, theta_grid, &Vds_ref, &Vqs_ref);
	dqs_to_abc(Vds_ref, Vqs_ref, &Vas_ref, &Vbs_ref, &Vcs_ref);

	//max/mid/min 계산
	Max_Mid_Min(&Vas_ref, &Vbs_ref, &Vcs_ref, &VOut_max, &VOut_mid, &VOut_min);

	// SVPWM //
	Vsn = -0.5 * (VOut_max + VOut_min);
	Van_ref = Vas_ref + Vsn, Vbn_ref = Vbs_ref + Vsn, Vcn_ref = Vcs_ref + Vsn;
	
	// 듀티 계산 //
    // CMP = TBPRD * 0.5 * (1 + Vphase/Vdc)
	CMP1_Ref = 7500 * 0.5 * (1 + (Van_ref / Vdc));  //PWM range 0~7500
	CMP2_Ref = 7500 * 0.5 * (1 + (Vbn_ref / Vdc));
	CMP3_Ref = 7500 * 0.5 * (1 + (Vcn_ref / Vdc));
}

void PWM_OUTPUT() 
{
	// Trip Zone clear 출력 허용
	PWM_ON();

	// ePWM Compare 업데이트
	EPwm1Regs.CMPA.half.CMPA = CMP1_Ref;
	EPwm1Regs.CMPB = CMP1_Ref;

	EPwm2Regs.CMPA.half.CMPA = CMP2_Ref;
	EPwm2Regs.CMPB = CMP2_Ref;

	EPwm3Regs.CMPA.half.CMPA = CMP3_Ref;
	EPwm3Regs.CMPB = CMP3_Ref;
}
//====================================================================================
// 16) PWM_ON(), PWM_CUT(): Trip Zone (OST) 클리어/강제 발생
//====================================================================================
void PWM_ON() 
{
	EALLOW;
	EPwm1Regs.TZCLR.bit.OST = 1;
	EPwm2Regs.TZCLR.bit.OST = 1;
	EPwm3Regs.TZCLR.bit.OST = 1;
	EDIS;
}

void PWM_CUT() 
{
	EALLOW;
	EPwm1Regs.TZFRC.bit.OST = 1;
	EPwm2Regs.TZFRC.bit.OST = 1;
	EPwm3Regs.TZFRC.bit.OST = 1;
	EDIS;
}
//====================================================================================
// 17) Cal_Average_Voltages(): 100샘플 평균(PreAvg/Avg) 값 계산 
//====================================================================================
void Cal_Average_Voltages() 
{
	CalculatingAvg_Count += 1;

	CalculatingAvg_Vdc += Vdc;
	CalculatingAvg_Vde_LPF += Vde_LPF;
	CalculatingAvg_Vqe_LPF += Vqe_LPF;

	if(CalculatingAvg_Count == 100){
		PreAvg_Vdc = Avg_Vdc;
		PreAvg_Vde_LPF = Avg_Vde_LPF;
		PreAvg_Vqe_LPF = Avg_Vqe_LPF;

		Avg_Vdc = CalculatingAvg_Vdc/100.;
		Avg_Vde_LPF = CalculatingAvg_Vde_LPF/100.;
		Avg_Vqe_LPF = CalculatingAvg_Vqe_LPF/100.;

		CalculatingAvg_Vdc = 0;
		CalculatingAvg_Vde_LPF = 0;
		CalculatingAvg_Vqe_LPF = 0;

		CalculatingAvg_Count = 0;
	}

}
//====================================================================================
// 18) 좌표변환 함수들 (abc <-> dq)
//====================================================================================
void abc_to_dqs(double input_A, double input_B, double input_C,
		double *output_DS, double *output_QS) {
	(*output_DS) = (2. * input_A - input_B - input_C) / 3.;
	(*output_QS) = (input_B - input_C) / sqrt(3.);
}
void dqs_to_dqe(double input_DS, double input_QS, double input_theta,
		double *output_DR, double *output_QR) {
	*output_DR = cos(input_theta) * input_DS + sin(input_theta) * input_QS;
	*output_QR = -sin(input_theta) * input_DS + cos(input_theta) * input_QS;
}
void abc_to_dqe(double input_A, double input_B, double input_C,
		double input_theta, double *output_DR, double *output_QR) {
	*output_DR = 2. / 3.
			* (input_A * cos(input_theta)
					+ input_B * cos(input_theta - 2. * PI / 3.)
					+ input_C * cos(input_theta - 4. * PI / 3.));
	*output_QR = 2. / 3.
			* ((-1.) * input_A * sin(input_theta)
					+ (-1.) * input_B * sin(input_theta - 2. * PI / 3.)
					+ (-1.) * input_C * sin(input_theta - 4. * PI / 3.));

}

void abc_to_dqze(double input_A, double input_B, double input_C,
		double input_theta, double *output_DR, double *output_QR,
		double *output_ZR) {
	*output_DR = 2. / 3.
			* (input_A * cos(input_theta)
					+ input_B * cos(input_theta - 2. * PI / 3.)
					+ input_C * cos(input_theta - 4. * PI / 3.));
	*output_QR = 2. / 3.
			* ((-1.) * input_A * sin(input_theta)
					+ (-1.) * input_B * sin(input_theta - 2. * PI / 3.)
					+ (-1.) * input_C * sin(input_theta - 4. * PI / 3.));
	*output_ZR = (input_A + input_B + input_C) / 3.;
}

//����ȯ
void dqe_to_dqs(double input_DR, double input_QR, double input_theta,
		double *output_DS, double *output_QS) {
	*output_DS = input_DR * cos(input_theta) - input_QR * sin(input_theta);
	*output_QS = input_DR * sin(input_theta) + input_QR * cos(input_theta);
}
void dqs_to_abc(double input_DS, double input_QS, double *output_A,
		double *output_B, double *output_C) {
	*output_A = input_DS;
	*output_B = -0.5 * input_DS + sqrt(3.) / 2. * input_QS;
	*output_C = -0.5 * input_DS - sqrt(3.) / 2. * input_QS;
}
void dqe_to_abc(double input_DR, double input_QR, double input_theta,
		double *output_A, double *output_B, double *output_C) {
	*output_A = cos(input_theta) * input_DR - sin(input_theta) * input_QR;
	*output_B = cos(input_theta - 2. * PI / 3.) * input_DR
			- sin(input_theta - 2. * PI / 3.) * input_QR;
	*output_C = cos(input_theta + 2. * PI / 3.) * input_DR
			- sin(input_theta + 2. * PI / 3.) * input_QR;
}

void dqze_to_abc(double input_DR, double input_QR, double input_ZR,
		double input_theta, double *output_A, double *output_B,
		double *output_C) {
	*output_A = cos(input_theta) * input_DR - sin(input_theta) * input_QR
			+ input_ZR * 0.5;
	*output_B = cos(input_theta - 2. * PI / 3.) * input_DR
			- sin(input_theta - 2. * PI / 3.) * input_QR + input_ZR * 0.5;
	*output_C = cos(input_theta + 2. * PI / 3.) * input_DR
			- sin(input_theta + 2. * PI / 3.) * input_QR + input_ZR * 0.5;
}

void Max_Mid_Min(double *input_A, double *input_B, double *input_C, double *max,
		double *mid, double *min) {
	if (*input_A <= *input_B) {
		if (*input_B <= *input_C) {
			*max = *input_C, *mid = *input_B, *min = *input_A;
		} else if (*input_C <= *input_B) {
			*max = *input_B;
			if (*input_C <= *input_A) {
				*mid = *input_A, *min = *input_C;
			} else {
				*mid = *input_C, *min = *input_A;
			}
		}

	}

	else if (*input_B <= *input_A) {
		if (*input_A <= *input_C) {
			*max = *input_C, *mid = *input_A, *min = *input_B;
		} else if (*input_C <= *input_A) {
			*max = *input_A;
			if (*input_C <= *input_B) {
				*mid = *input_B, *min = *input_C;
			} else {
				*mid = *input_C, *min = *input_B;
			}
		}
	}
}
// End of file.
//===========================================================================

