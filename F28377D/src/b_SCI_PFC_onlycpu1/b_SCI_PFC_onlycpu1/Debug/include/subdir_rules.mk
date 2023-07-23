################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
include/YK.obj: ../include/YK.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"c:/ti/ccsv6/tools/compiler/c2000_6.2.11/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="G:/workspace_v6_0-1/DTU_CPU1/include" --include_path="c:/ti/ccsv6/tools/compiler/c2000_6.2.11/include" --include_path="G:/workspace_v6_0-1/DTU_CPU1" -g --define=CPU1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="include/YK.pp" --obj_directory="include" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/YX.obj: ../include/YX.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"c:/ti/ccsv6/tools/compiler/c2000_6.2.11/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="G:/workspace_v6_0-1/DTU_CPU1/include" --include_path="c:/ti/ccsv6/tools/compiler/c2000_6.2.11/include" --include_path="G:/workspace_v6_0-1/DTU_CPU1" -g --define=CPU1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="include/YX.pp" --obj_directory="include" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


