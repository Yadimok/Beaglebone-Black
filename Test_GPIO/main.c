/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <string.h>

/* TI-RTOS Header files */
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/soc/UART_soc.h>

#include <ti/board/board.h>
#include <ti/csl/csl_clec.h>
#include <ti/csl/csl_gpio.h>
#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/csl_utils.h>

#include <ti/starterware/include/prcm.h>
#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/starterware/include/am335x/hw_cm_per.h>


void taskFxn(UArg a0, UArg a1) {

    Board_initCfg boardCfg;
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
            BOARD_INIT_MODULE_CLOCK |
            BOARD_INIT_UART_STDIO;

    Board_init(boardCfg);

    uint32_t gpioBaseAddr   = 0x4804C000;
    uint32_t gpioPin        = 0x17U;

    /* Writing to MODULEMODE field of CM_PER_GPIO1_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |=
            CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE !=
            (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
                    CM_PER_GPIO1_CLKCTRL_MODULEMODE));
    /*
     ** Writing to OPTFCLKEN_GPIO_1_GDBCLK bit in CM_PER_GPIO1_CLKCTRL
     ** register.
     */
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) |=
            CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK;

    /*
     ** Waiting for OPTFCLKEN_GPIO_1_GDBCLK bit to reflect the desired
     ** value.
     */
    while(CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK !=
            (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
                    CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK));

    /*
     ** Waiting for IDLEST field in CM_PER_GPIO1_CLKCTRL register to attain the
     ** desired value.
     */
    while((CM_PER_GPIO1_CLKCTRL_IDLEST_FUNC <<
            CM_PER_GPIO1_CLKCTRL_IDLEST_SHIFT) !=
                    (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1_CLKCTRL) &
                            CM_PER_GPIO1_CLKCTRL_IDLEST));

    /*
     ** Waiting for CLKACTIVITY_GPIO_1_GDBCLK bit in CM_PER_L4LS_CLKSTCTRL
     ** register to attain desired value.
     */
    while(CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_1_GDBCLK !=
            (HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
                    CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_1_GDBCLK));


    GPIOModuleReset(gpioBaseAddr);
    GPIOModuleEnable(gpioBaseAddr);
    GPIODirModeSet(gpioBaseAddr, gpioPin, GPIO_DIR_OUTPUT);

    while (1) {
        GPIOPinWrite(gpioBaseAddr, gpioPin, GPIO_PIN_HIGH);
        Task_sleep(250);


        GPIOPinWrite(gpioBaseAddr, gpioPin, GPIO_PIN_LOW);
        Task_sleep(250);

    }
}

int main(void)
{
    Task_Handle task;
    Error_Block eb;
    Error_init(&eb);
    task = Task_create(taskFxn, NULL, &eb);
    if (task == NULL) {
        BIOS_exit(0);
    }


    /* Start BIOS */
    BIOS_start();
    return (0);
}
