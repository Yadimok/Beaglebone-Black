################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../app.cfg 

C_SRCS += \
/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/uart/soc/am335x/UART_soc.c \
../main.c \
/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/transport/ndk/nimu/example/src/nimu_osal.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./UART_soc.d \
./main.d \
./nimu_osal.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./UART_soc.o \
./main.o \
./nimu_osal.o 

GEN_MISC_DIRS__QUOTED += \
"configPkg/" 

OBJS__QUOTED += \
"UART_soc.o" \
"main.o" \
"nimu_osal.o" 

C_DEPS__QUOTED += \
"UART_soc.d" \
"main.d" \
"nimu_osal.d" 

GEN_FILES__QUOTED += \
"configPkg/linker.cmd" \
"configPkg/compiler.opt" 

C_SRCS__QUOTED += \
"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/drv/uart/soc/am335x/UART_soc.c" \
"../main.c" \
"/home/dmitrii/ti/pdk_am335x_1_0_17/packages/ti/transport/ndk/nimu/example/src/nimu_osal.c" 


