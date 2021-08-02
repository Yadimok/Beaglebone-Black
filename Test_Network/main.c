#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/a8/Mmu.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/board/board.h>

/* UART Header files */
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/drv/uart/test/src/UART_board.h>

#include <ti/ndk/inc/socket.h>
#include <ti/ndk/inc/nettools/inc/inet.h>
#include <ti/ndk/inc/stkmain.h>

#include <ti/drv/emac/emac_drv.h>
#include <ti/drv/emac/src/v4/emac_drv_v4.h>

#include <ti/starterware/include/types.h>
#include <ti/starterware/include/hw/hw_types.h>
#include <ti/starterware/include/hw/hw_control_am335x.h>
#include <ti/starterware/include/hw/soc_am335x.h>
#include <ti/starterware/include/ethernet.h>
#include <ti/starterware/include/soc_control.h>


#define MAX     256
#define PORT    3456

/**Phy address of the CPSW port 1*/
#define EMAC_CPSW_PORT0_PHY_ADDR_EVM    0
/**Phy address of the CPSW port 1*/
#define EMAC_CPSW_PORT1_PHY_ADDR_EVM    1

#define MAX_TABLE_ENTRIES   3

Task_Handle main_task;

static int nimu_device_index = 0U;
static uint16_t port;

NIMU_DEVICE_TABLE_ENTRY NIMUDeviceTable[MAX_TABLE_ENTRIES];

void taskFxn(UArg a0, UArg a1);
extern char *LocalIPAddr;
extern int CpswEmacInit (STKEVENT_Handle hEvent);

Int main()
{
    Task_Params taskParams;

    Board_initCfg boardCfg;
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
            BOARD_INIT_MODULE_CLOCK  |
            BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    EMAC_HwAttrs_V4 cfg;

    SOCCtrlCpswPortMacModeSelect(1, ETHERNET_MAC_TYPE_MII);
    SOCCtrlCpswPortMacModeSelect(2, ETHERNET_MAC_TYPE_MII);

    EMAC_socGetInitCfg(0, &cfg);
    cfg.port[0].phy_addr = EMAC_CPSW_PORT0_PHY_ADDR_EVM;
    cfg.port[1].phy_addr = EMAC_CPSW_PORT0_PHY_ADDR_EVM;
    cfg.macModeFlags = EMAC_CPSW_CONFIG_MODEFLG_FULLDUPLEX;
    EMAC_socSetInitCfg(0, &cfg);

    port = PORT;

    Task_Params_init(&taskParams);
    taskParams.priority = 1;
    taskParams.stackSize = 32768;
    main_task = Task_create(taskFxn, &taskParams, NULL);
    if (main_task == NULL) {
        BIOS_exit(0);
    }

    NIMUDeviceTable[nimu_device_index++].init =  &CpswEmacInit ;
    NIMUDeviceTable[nimu_device_index].init =  NULL ;

    BIOS_start();    /* does not return */
    return(0);
}

Void taskFxn(UArg a0, UArg a1)
{
    struct sockaddr_in server_addr, client_addr;
    SOCKET listen_sock, client_sock;
    int len = sizeof(struct sockaddr_in);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = NDK_htonl(INADDR_ANY);
    server_addr.sin_port = NDK_htons(port);

    fdOpenSession (TaskSelf ());

    UART_printf("Local IP address and port:\n\tLocal IP address: %s\n\tLocal port: %d\n",
                LocalIPAddr,
                port);

    if ((listen_sock = NDK_socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        UART_printf("socket() failed\n");
        return;
    }
    UART_printf("socket() success!\n");

    if (NDK_bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
        UART_printf("bind() failed\n");
        fdClose(listen_sock);
        return;
    }
    UART_printf("bind() success!\n");


    if (NDK_listen(listen_sock, 5) == SOCKET_ERROR)
    {
        UART_printf("listen() failed\n");
        fdClose(listen_sock);
        return;
    }
    UART_printf("listen() success!\n");

    if( (client_sock = NDK_accept(listen_sock, (struct sockaddr*)&client_addr, &len)) == INVALID_SOCKET)
    {
        UART_printf("accept() failed\n");
        fdClose (listen_sock);
        Task_sleep (100);
    }
    UART_printf("accept() success!\n");

    char buffer[MAX];
    int n;
    memset(buffer, 0x00, MAX);


    if ((n = NDK_recv(client_sock, buffer, sizeof(buffer), 0)) < 0) {
        UART_printf("recv() failed\n");;
    }
    UART_printf("From client: %s\n", buffer);

    char msg[] = "Hello from server, Yadimok!";
    memcpy(buffer, msg, strlen(msg));
    if (NDK_send(client_sock, buffer, sizeof(buffer), 0) != sizeof(buffer)) {
        UART_printf("send() failed\n");
    }

    UART_printf("\nDone!\n");


    fdClose(client_sock);
    fdClose(listen_sock);

    fdCloseSession(TaskSelf());
}

