################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./source/Adc.obj" \
"./source/DSP2833x_ADC_cal.obj" \
"./source/DSP2833x_CSMPasswords.obj" \
"./source/DSP2833x_CodeStartBranch.obj" \
"./source/DSP2833x_CpuTimers.obj" \
"./source/DSP2833x_DBGIER.obj" \
"./source/DSP2833x_DMA.obj" \
"./source/DSP2833x_DefaultIsr.obj" \
"./source/DSP2833x_DisInt.obj" \
"./source/DSP2833x_ECan.obj" \
"./source/DSP2833x_ECap.obj" \
"./source/DSP2833x_EPwm.obj" \
"./source/DSP2833x_EQep.obj" \
"./source/DSP2833x_GlobalVariableDefs.obj" \
"./source/DSP2833x_Gpio.obj" \
"./source/DSP2833x_I2C.obj" \
"./source/DSP2833x_Mcbsp.obj" \
"./source/DSP2833x_MemCopy.obj" \
"./source/DSP2833x_PieCtrl.obj" \
"./source/DSP2833x_PieVect.obj" \
"./source/DSP2833x_Sci.obj" \
"./source/DSP2833x_Spi.obj" \
"./source/DSP2833x_SysCtrl.obj" \
"./source/DSP2833x_Xintf.obj" \
"./source/DSP2833x_usDelay.obj" \
"./source/Pelab_EPWM.obj" \
"./source/Pelab_Func.obj" \
"./source/Qep.obj" \
"./source/RFFT_adc_f32.obj" \
"./source/RFFT_adc_f32u.obj" \
"./source/RFFT_f32.obj" \
"./source/RFFT_f32_mag.obj" \
"./source/RFFT_f32_phase.obj" \
"./source/RFFT_f32_sincostable.obj" \
"./source/RFFT_f32s_mag.obj" \
"./source/RFFT_f32u.obj" \
"./source/RingBuff.obj" \
"./source/Timer.obj" \
"./source/dac_easyDSP.obj" \
"./source/easy2833x_sci_v8.5.obj" \
"./source/led.obj" \
"./source/main.obj" \
"../cmd/28335_RAM_lnk.cmd" \
"../cmd/DSP2833x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include cmd/subdir_vars.mk
-include source/subdir_vars.mk
-include cmd/subdir_rules.mk
-include source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
PFC800_V1.out 

EXE_OUTPUTS__QUOTED += \
"PFC800_V1.out" 


# All Target
all: PFC800_V1.out

# Tool invocations
PFC800_V1.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O2 --display_error_number --diag_warning=225 --diag_wrap=off -z -m"PFC800_V1.map" --stack_size=0x300 --warn_sections -i"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/lib" -i"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --reread_libs --display_error_number --diag_wrap=off --xml_link_info="PFC800_V1_linkInfo.xml" --rom_model -o "PFC800_V1.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "source\Adc.obj" "source\DSP2833x_ADC_cal.obj" "source\DSP2833x_CSMPasswords.obj" "source\DSP2833x_CodeStartBranch.obj" "source\DSP2833x_CpuTimers.obj" "source\DSP2833x_DBGIER.obj" "source\DSP2833x_DMA.obj" "source\DSP2833x_DefaultIsr.obj" "source\DSP2833x_DisInt.obj" "source\DSP2833x_ECan.obj" "source\DSP2833x_ECap.obj" "source\DSP2833x_EPwm.obj" "source\DSP2833x_EQep.obj" "source\DSP2833x_GlobalVariableDefs.obj" "source\DSP2833x_Gpio.obj" "source\DSP2833x_I2C.obj" "source\DSP2833x_Mcbsp.obj" "source\DSP2833x_MemCopy.obj" "source\DSP2833x_PieCtrl.obj" "source\DSP2833x_PieVect.obj" "source\DSP2833x_Sci.obj" "source\DSP2833x_Spi.obj" "source\DSP2833x_SysCtrl.obj" "source\DSP2833x_Xintf.obj" "source\DSP2833x_usDelay.obj" "source\Pelab_EPWM.obj" "source\Pelab_Func.obj" "source\Qep.obj" "source\RFFT_adc_f32.obj" "source\RFFT_adc_f32u.obj" "source\RFFT_f32.obj" "source\RFFT_f32_mag.obj" "source\RFFT_f32_phase.obj" "source\RFFT_f32_sincostable.obj" "source\RFFT_f32s_mag.obj" "source\RFFT_f32u.obj" 
	-$(RM) "source\RingBuff.obj" "source\Timer.obj" "source\dac_easyDSP.obj" "source\easy2833x_sci_v8.5.obj" "source\led.obj" "source\main.obj" 
	-$(RM) "source\Adc.d" "source\DSP2833x_CpuTimers.d" "source\DSP2833x_DMA.d" "source\DSP2833x_DefaultIsr.d" "source\DSP2833x_ECan.d" "source\DSP2833x_ECap.d" "source\DSP2833x_EPwm.d" "source\DSP2833x_EQep.d" "source\DSP2833x_GlobalVariableDefs.d" "source\DSP2833x_Gpio.d" "source\DSP2833x_I2C.d" "source\DSP2833x_Mcbsp.d" "source\DSP2833x_MemCopy.d" "source\DSP2833x_PieCtrl.d" "source\DSP2833x_PieVect.d" "source\DSP2833x_Sci.d" "source\DSP2833x_Spi.d" "source\DSP2833x_SysCtrl.d" "source\DSP2833x_Xintf.d" "source\Pelab_EPWM.d" "source\Pelab_Func.d" "source\Qep.d" "source\RFFT_f32_sincostable.d" "source\RingBuff.d" "source\Timer.d" "source\dac_easyDSP.d" "source\easy2833x_sci_v8.5.d" "source\led.d" "source\main.d" 
	-$(RM) "source\DSP2833x_ADC_cal.d" "source\DSP2833x_CSMPasswords.d" "source\DSP2833x_CodeStartBranch.d" "source\DSP2833x_DBGIER.d" "source\DSP2833x_DisInt.d" "source\DSP2833x_usDelay.d" "source\RFFT_adc_f32.d" "source\RFFT_adc_f32u.d" "source\RFFT_f32.d" "source\RFFT_f32_mag.d" "source\RFFT_f32_phase.d" "source\RFFT_f32s_mag.d" "source\RFFT_f32u.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

