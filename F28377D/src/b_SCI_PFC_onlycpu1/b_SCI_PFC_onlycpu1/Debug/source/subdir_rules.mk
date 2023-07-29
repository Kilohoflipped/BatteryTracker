################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source/%.obj: ../source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"E:/Software/TI/CCS 1240/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="E:/Kilo/Project/BatteryTracker/F28377D/src/b_SCI_PFC_onlycpu1/b_SCI_PFC_onlycpu1/include" --include_path="E:/Software/TI/CCS 1240/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --define=CPU1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

source/%.obj: ../source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"E:/Software/TI/CCS 1240/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="E:/Kilo/Project/BatteryTracker/F28377D/src/b_SCI_PFC_onlycpu1/b_SCI_PFC_onlycpu1/include" --include_path="E:/Software/TI/CCS 1240/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --define=CPU1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


