################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
UART_soc.o: /home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/uart/soc/am335x/UART_soc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc-7.2.1" -c -mcpu=cortex-a8 -march=armv7-a -mtune=cortex-a8 -marm -mfloat-abi=hard -Dam3359 -I"/home/dmitrii/ti/ccs930/ccs/workspace/Test_Network" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include/newlib-nano" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/ti/posix/gcc" -I"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/arm-none-eabi/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

build-716490159:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-716490159-inproc

build-716490159-inproc: ../app.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/home/dmitrii/ti/ccs930/xdctools_3_60_02_34_core/xs" --xdcpath="/home/dmitrii/ti/bios_6_76_03_01/packages;/home/dmitrii/ti/pdk_am335x_1_0_17/packages;/home/dmitrii/ti/edma3_lld_2_12_05_30E/packages;/home/dmitrii/ti/ndk_3_61_01_01/packages;" xdc.tools.configuro -o configPkg -t gnu.targets.arm.A8F -p ti.platforms.beaglebone -r release -c "/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-716490159 ../app.cfg
configPkg/compiler.opt: build-716490159
configPkg/: build-716490159

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc-7.2.1" -c -mcpu=cortex-a8 -march=armv7-a -mtune=cortex-a8 -marm -mfloat-abi=hard -Dam3359 -I"/home/dmitrii/ti/ccs930/ccs/workspace/Test_Network" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include/newlib-nano" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/ti/posix/gcc" -I"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/arm-none-eabi/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

nimu_osal.o: /home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/transport/ndk/nimu/example/src/nimu_osal.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc-7.2.1" -c -mcpu=cortex-a8 -march=armv7-a -mtune=cortex-a8 -marm -mfloat-abi=hard -Dam3359 -I"/home/dmitrii/ti/ccs930/ccs/workspace/Test_Network" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include/newlib-nano" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/ti/posix/gcc" -I"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/arm-none-eabi/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


