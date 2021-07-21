################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
GPIO_log.o: /home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/gpio/test/led_blink/src/GPIO_log.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc-7.2.1" -c -mcpu=cortex-a8 -march=armv7-a -mtune=cortex-a8 -marm -mfloat-abi=hard -Dam3359 -DUSE_BIOS -DSOC_AM335x -DbbbAM335x -I"/home/dmitrii/ti/ccs930/ccs/workspace/Test04_GPIO" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include/newlib-nano" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/ti/posix/gcc" -I"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/arm-none-eabi/include" -I"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/gpio" -I"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/uart" -I"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/board" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

GPIO_soc.o: /home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/gpio/soc/am335x/GPIO_soc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc-7.2.1" -c -mcpu=cortex-a8 -march=armv7-a -mtune=cortex-a8 -marm -mfloat-abi=hard -Dam3359 -DUSE_BIOS -DSOC_AM335x -DbbbAM335x -I"/home/dmitrii/ti/ccs930/ccs/workspace/Test04_GPIO" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include/newlib-nano" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/ti/posix/gcc" -I"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/arm-none-eabi/include" -I"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/gpio" -I"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/uart" -I"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/board" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

build-911433343:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-911433343-inproc

build-911433343-inproc: ../app.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/home/dmitrii/ti/ccs930/xdctools_3_60_02_34_core/xs" --xdcpath="/home/dmitrii/ti/bios_6_76_03_01/packages;/home/dmitrii/ti/pdk_am335x_1_0_17/packages;" xdc.tools.configuro -o configPkg -t gnu.targets.arm.A8F -p ti.platforms.beaglebone -r release -c "/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-911433343 ../app.cfg
configPkg/compiler.opt: build-911433343
configPkg/: build-911433343

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc-7.2.1" -c -mcpu=cortex-a8 -march=armv7-a -mtune=cortex-a8 -marm -mfloat-abi=hard -Dam3359 -DUSE_BIOS -DSOC_AM335x -DbbbAM335x -I"/home/dmitrii/ti/ccs930/ccs/workspace/Test04_GPIO" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/gnu/targets/arm/libs/install-native/arm-none-eabi/include/newlib-nano" -I"/home/dmitrii/ti/bios_6_76_03_01/packages/ti/posix/gcc" -I"/home/dmitrii/ti/ccs930/ccs/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/arm-none-eabi/include" -I"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/gpio" -I"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/uart" -I"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/board" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


