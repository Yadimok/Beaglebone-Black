################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../app.cfg 

C_SRCS += \
/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/gpio/test/led_blink/src/GPIO_log.c \
/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/gpio/soc/am335x/GPIO_soc.c \
../main.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./GPIO_log.d \
./GPIO_soc.d \
./main.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./GPIO_log.o \
./GPIO_soc.o \
./main.o 

GEN_MISC_DIRS__QUOTED += \
"configPkg/" 

OBJS__QUOTED += \
"GPIO_log.o" \
"GPIO_soc.o" \
"main.o" 

C_DEPS__QUOTED += \
"GPIO_log.d" \
"GPIO_soc.d" \
"main.d" 

GEN_FILES__QUOTED += \
"configPkg/linker.cmd" \
"configPkg/compiler.opt" 

C_SRCS__QUOTED += \
"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/gpio/test/led_blink/src/GPIO_log.c" \
"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/gpio/soc/am335x/GPIO_soc.c" \
"../main.c" 


