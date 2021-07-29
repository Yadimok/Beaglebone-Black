#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>

#include <ti/fs/fatfs/ff.h>
#include <ti/csl/cslr_device.h>
#include <ti/fs/fatfs/example/console/src/FATFS_log.h>
#include <ti/fs/fatfs/example/console/src/fs_shell_app_utils.h>
#include <ti/fs/fatfs/diskio.h>
#include <ti/fs/fatfs/FATFS.h>

#include <ti/drv/mmcsd/MMCSD.h>

#include <ti/board/board.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/sdo/edma3/drv/edma3_drv.h>

#include <ti/osal/MuxIntcP.h>

#include "aes.h"
#include "random.h"

#define MMCSD_INSTANCE_MMCSD            (0U)

#define GPIO_PIN_VAL_LOW                (0U)
#define GPIO_PIN_VAL_HIGH               (1U)



/* ON Board LED pins which are connected to GPIO pins. */
typedef enum GPIO_PIN {
    GPIO_PIN_MMC_SDCD      = 0U,
    GPIO_PIN_COUNT
}GPIO_PIN;

/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] = {
                                   /* Output pin : AM335X_ICE V2_LD_PIN */
                                   GPIO_DEVICE_CONFIG(GPIO_MMC_SDCD_PORT_NUM, GPIO_MMC_SDCD_PIN_NUM) |
                                   GPIO_CFG_IN_INT_BOTH_EDGES | GPIO_CFG_INPUT,
};

/* GPIO Driver call back functions */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
                                            NULL,
};

/* GPIO Driver configuration structure */
GPIO_v1_Config GPIO_v1_config = {
                                 gpioPinConfigs,
                                 gpioCallbackFunctions,
                                 sizeof(gpioPinConfigs) / sizeof(GPIO_PinConfig),
                                 sizeof(gpioCallbackFunctions) / sizeof(GPIO_CallbackFxn),
                                 0,
};



/* MMCSD function table for MMCSD implementation */
FATFS_DrvFxnTable FATFS_drvFxnTable = {
                                       MMCSD_close,
                                       MMCSD_control,
                                       MMCSD_init,
                                       MMCSD_open,
                                       MMCSD_write,
                                       MMCSD_read
};

/* FATFS configuration structure */
FATFS_HwAttrs FATFS_initCfg[_VOLUMES] =
{
 {
  0U
 },
 {
  1U
 },
 {
  2U
 },
 {
  3U
 }
};

/* FATFS objects */
FATFS_Object FATFS_objects[_VOLUMES];

/* FATFS configuration structure */
const FATFSConfigList FATFS_config = {
                                      {
                                       &FATFS_drvFxnTable,
                                       &FATFS_objects[0],
                                       &FATFS_initCfg[0]
                                      },

                                      {
                                       &FATFS_drvFxnTable,
                                       &FATFS_objects[1],
                                       &FATFS_initCfg[1]
                                      },

                                      {
                                       &FATFS_drvFxnTable,
                                       &FATFS_objects[2],
                                       &FATFS_initCfg[2]
                                      },

                                      {NULL, NULL, NULL},

                                      {NULL, NULL, NULL}
};

FATFS_Handle fatfsHandle = NULL;


Uint32 fatfsShellProcessFlag = 0;
Uint32 fs_media_needs_initialization=0;
Uint32 fs_media_needs_close=0;

void AppGpioCallbackFxn(void);
void taskFxn(UArg a0, UArg a1);
void fatfs_console();
static void command_ls();

///
/* Defines size of the buffers that hold temporary data. */
#define FS_SHELL_APP_UTILS_DATA_BUF_SIZE   512

static char gFsShellAppUtilsCwd[FS_SHELL_APP_UTILS_DATA_BUF_SIZE]
                                __attribute__ ((aligned (SOC_CACHELINE_SIZE)));
static DIR gFsShellAppUtilsDirObj;
static FILINFO gFsShellAppUtilsFileInfo;
static FIL     fp;
static FRESULT fresult = FR_OK;

static char src_file[] = "lorem.txt";
static char enc_file[] = "lorem_enc.bin";
static char dec_file[] = "lorem_dec.txt";
///

void taskFxn(UArg a0, UArg a1)
{
    Board_initCfg boardCfg;
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
            BOARD_INIT_UART_STDIO |
            BOARD_INIT_MODULE_CLOCK;
    Board_init(boardCfg);

    fatfs_console();
}


void fatfs_console()
{
    UINT length_file = 0;
    UINT bytesRead;
    uint8_t *src_buf = NULL, *enc_buf = NULL, *dec_buf = NULL;

    ///

    GPIO_init();

    /* Set the callback function */
    GPIO_setCallback(GPIO_PIN_MMC_SDCD, AppGpioCallbackFxn);

    /* Enable GPIO interrupt on the specific gpio pin */
    GPIO_enableInt(GPIO_PIN_MMC_SDCD);

    FATFS_init();

    if (GPIO_PIN_MMC_SDCD_ACTIVE_STATE == GPIO_read(GPIO_PIN_MMC_SDCD))
    {
        FATFS_open(MMCSD_INSTANCE_MMCSD, NULL, &fatfsHandle);
        fatfsShellProcessFlag = 1;
        fs_is_media_inserted = 1;

    }
    else
    {
        FATFS_log ("\nPlease insert card.\r\n");
        fs_is_media_inserted = 0;
    }

    while(1)
    {

        if(fs_media_needs_initialization) {

            FATFS_log("\nMedia Inserted..Initializing..\n");
            FATFS_open(MMCSD_INSTANCE_MMCSD, NULL, &fatfsHandle);
            fatfsShellProcessFlag = 1;
            fs_media_needs_initialization=0;
        }

        if(fs_media_needs_close) {
            FATFS_close(fatfsHandle);
            fatfsShellProcessFlag = 0;
            fs_media_needs_close=0;
            FATFS_log("\nMedia Removed..Please insert media..\n");
        }

        if (0 != fatfsShellProcessFlag)
        {
            FATFS_log("List files:\n");
            command_ls();
            FATFS_log("\n-------------------------------\n");

            fresult = f_open(&fp, src_file, FA_READ);
            if (fresult != FR_OK) {
                FATFS_log("File %s not exists\n", src_file);
                System_exit(1);
            }

            fresult = f_stat(src_file, &gFsShellAppUtilsFileInfo);
            if (fresult != FR_OK) {
                FATFS_log("File not f_stat\n");
                f_close(&fp);
            }

            FATFS_log("Stat file: size = %d, name = %s\n",
                      gFsShellAppUtilsFileInfo.fsize,
                      gFsShellAppUtilsFileInfo.fname);


            length_file = gFsShellAppUtilsFileInfo.fsize;
            src_buf = (uint8_t *)malloc(length_file+1);
            if (src_buf == NULL) {
                FATFS_log("Buffer1 allocation failed\n");
                System_exit(1);
            }

            enc_buf = (uint8_t *)malloc(length_file+1);
            if (enc_buf == NULL) {
                FATFS_log("Buffer2 allocation failed\n");
                System_exit(1);
            }

            dec_buf = (uint8_t *)malloc(length_file+1);
            if (dec_buf == NULL) {
                FATFS_log("Buffer3 allocation failed\n");
                System_exit(1);
            }


            fresult = f_read(&fp, src_buf, length_file, &bytesRead);
            if (fresult != FR_OK) {
                FATFS_log("Cannot read file lorem.txt\n");
                f_close(&fp);
                System_exit(2);
            }
            src_buf[length_file] = '\0';
            FATFS_log("Text:\n%s\n", (char *)src_buf);

            f_close(&fp);


            uint8_t key[32] = {
                               0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                               0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                               0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
            };

            uint8_t iv[16]  = {
                               0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
                               0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77
            };

            struct AES_Ctx ctx;

            AES_init_ctx_iv(&ctx, key, iv);
            AES_CTR_xcrypt_buffer(&ctx, src_buf, length_file);

            memcpy(enc_buf, src_buf, length_file);
            enc_buf[length_file] = '\0';

            fresult = f_open(&fp, enc_file, FA_CREATE_ALWAYS | FA_WRITE);
            if (fresult != FR_OK) {
                FATFS_log("File %s not created\n", enc_file);
                System_exit(1);
            }

            fresult = f_write(&fp, enc_buf, length_file, &bytesRead);
            if (fresult != FR_OK) {
                FATFS_log("File %s not written\n", enc_file);
                f_close(&fp);
                System_exit(1);
            }

            f_sync(&fp);
            f_close(&fp);

            FATFS_log("\n-------------------------------\n");
            FATFS_log("After encrypted:\n");

            int i;
            for (i=0; i<length_file; i++) {
                FATFS_log("%02X", enc_buf[i]);
            }

            AES_init_ctx_iv(&ctx, key, iv);
            AES_CTR_xcrypt_buffer(&ctx, enc_buf, length_file);

            memcpy(dec_buf, enc_buf, length_file);
            dec_buf[length_file] = '\0';

            fresult = f_open(&fp, dec_file, FA_CREATE_ALWAYS | FA_WRITE);
            if (fresult != FR_OK) {
                FATFS_log("File %s not created\n", dec_file);
                System_exit(1);
            }

            fresult = f_write(&fp, dec_buf, length_file, &bytesRead);
            if (fresult != FR_OK) {
                FATFS_log("File %s not written\n", dec_file);
                f_close(&fp);
                System_exit(1);
            }

            f_sync(&fp);
            f_close(&fp);


            FATFS_log("\n-------------------------------\n");
            FATFS_log("After decrypted:\n");
            for (i=0; i<length_file; i++) {
                FATFS_log("%c", (char *)dec_buf[i]);
            }

            FATFS_log("\n-------------------------------\n");
            command_ls();

            free(src_buf);
            free(enc_buf);
            free(dec_buf);

            fatfsShellProcessFlag = 0;
        }
    }
}


Int main()
{ 
    Task_Handle task;
    Error_Block eb;
    Error_init(&eb);
    task = Task_create(taskFxn, NULL, &eb);
    if (task == NULL) {
        BIOS_exit(0);
    }


    BIOS_start();    /* does not return */
    return(0);
}

void media_open()
{
    FATFS_open(MMCSD_INSTANCE_MMCSD, NULL, &fatfsHandle);
    fatfsShellProcessFlag = 1;
}

void media_close()
{
    FATFS_close(fatfsHandle);
    fatfsShellProcessFlag = 0;
}

void AppGpioCallbackFxn(void)
{
    if (GPIO_PIN_MMC_SDCD_ACTIVE_STATE == GPIO_read(GPIO_PIN_MMC_SDCD))
    {
        fs_is_media_inserted=1; /* The media has been inserted now */
        fs_media_needs_initialization=1; /* Need to initialize FATFS with the media */
    }
    else
    {
        fs_is_media_inserted=0;/*  The media has been ejected now */
        fs_media_needs_close=1; /* Need to close FATFS with the media */
    }
}

static void command_ls()
{
    uint32_t totalSize = 0U;
    uint32_t fileCount = 0U;
    uint32_t dirCount = 0U;
    FRESULT fresult;
    FATFS *pFatFs;

    /* Open the current directory for access. */
    fresult = f_opendir(&gFsShellAppUtilsDirObj, gFsShellAppUtilsCwd);

    /* Enter loop to enumerate through all directory entries. */
    while(FR_OK == fresult)
    {
        /* Read an entry from the directory. */
        fresult = f_readdir(&gFsShellAppUtilsDirObj, &gFsShellAppUtilsFileInfo);

        /* Check for error and return if there is a problem. */
        if(FR_OK == fresult)
        {
            /* If the file name is blank, then this is the end of the listing. */
            if('\0' == gFsShellAppUtilsFileInfo.fname[0])
            {
                break;
            }

            /* If the attribute is directory, then increment the directory count. */
            if(AM_DIR == (gFsShellAppUtilsFileInfo.fattrib & AM_DIR))
            {
                dirCount++;
            }

            /*
             * Otherwise, it is a file.  Increment the file count, and add in the
             * file size to the total.
             */
            else
            {
                fileCount++;
                totalSize += gFsShellAppUtilsFileInfo.fsize;
            }

            /*
             * Print the entry information on a single line with formatting to show
             * the attributes, date, time, size, and name.
             */
            FATFS_log("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9u  %s\n",
                      (gFsShellAppUtilsFileInfo.fattrib & AM_DIR) ? 'D' : '-',
                              (gFsShellAppUtilsFileInfo.fattrib & AM_RDO) ? 'R' : '-',
                                      (gFsShellAppUtilsFileInfo.fattrib & AM_HID) ? 'H' : '-',
                                              (gFsShellAppUtilsFileInfo.fattrib & AM_SYS) ? 'S' : '-',
                                                      (gFsShellAppUtilsFileInfo.fattrib & AM_ARC) ? 'A' : '-',
                                                              (gFsShellAppUtilsFileInfo.fdate >> 9) + 1980,
                                                              (gFsShellAppUtilsFileInfo.fdate >> 5) & 15,
                                                              gFsShellAppUtilsFileInfo.fdate & 31,
                                                              (gFsShellAppUtilsFileInfo.ftime >> 11),
                                                              (gFsShellAppUtilsFileInfo.ftime >> 5) & 63,
                                                              gFsShellAppUtilsFileInfo.fsize,
                                                              gFsShellAppUtilsFileInfo.fname);
        }
    }

    /* Check for error and return if there is a problem. */
    if(FR_OK == fresult)
    {
        /* Print summary lines showing the file, dir, and size totals. */
        FATFS_log("\n%4u File(s),%10u bytes total\n%4u Dir(s)",
                  fileCount, totalSize, dirCount);

        /* Get the free space. */
        fresult = f_getfree("/", (DWORD *)&totalSize, &pFatFs);

        /* Check for error and return if there is a problem. */
        if(FR_OK == fresult)
        {
            /* Display the amount of free space that was calculated. */
            FATFS_log(", %10uK bytes free\n", totalSize * pFatFs->csize / 2);
        }
    }
}
