/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    LPC54628J512_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54628.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "fsl_iocon.h"
#include <stdbool.h>
#include <string.h>
#include "fsl_sd.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "ffconf.h"

/* TODO: insert other definitions and declarations here. */

#define BUFFER_SIZE (100U)

static status_t sdcardWaitCardInsert(void);

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 5000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

void Board_InitSdifUnusedDataPin(void)
{
    IOCON_PinMuxSet(IOCON, 4, 29,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP)); /* sd data[4] */
    IOCON_PinMuxSet(IOCON, 4, 30,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP)); /* sd data[5] */
    IOCON_PinMuxSet(IOCON, 4, 31,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP)); /* sd data[6] */
    IOCON_PinMuxSet(IOCON, 5, 0,
                    (IOCON_FUNC2 | IOCON_PIO_SLEW_MASK | IOCON_DIGITAL_EN | IOCON_MODE_PULLUP)); /* sd data[7] */
}

static FATFS g_fileSystem; /* File system object */
static FIL g_fileObject;   /* File object */


SDK_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
SDK_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));



/*! @brief SDMMC host detect card configuration */
static const sdmmchost_detect_card_t s_sdCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
};

/*
 * @brief   Application entry point.
 */
int main(void) {

    FRESULT error;
    DIR directory; /* Directory object */
    FILINFO fileInformation;
    UINT bytesWritten;
    UINT bytesRead;
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
    volatile bool failedFlag = false;
    char ch = '0';
    BYTE work[FF_MAX_SS];

	FRESULT ret;
	FILINFO fno;
	char path[26] = {};
	FIL fil={};






	 CLOCK_EnableClock(kCLOCK_InputMux);
	/* attach main clock to SDIF */
	CLOCK_AttachClk(BOARD_SDIF_CLK_ATTACH);

  	/* Init board hardware. */
    BOARD_InitBootPins();

    Board_InitSdifUnusedDataPin();

    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    /* need call this function to clear the halt bit in clock divider register */
    CLOCK_SetClkDiv(kCLOCK_DivSdioClk, (uint32_t)(SystemCoreClock / FSL_FEATURE_SDIF_MAX_SOURCE_CLOCK + 1U), true);



    PRINTF("Hello World\n");

    LED1_ON();
    LED2_ON();
    LED3_ON();


    if (sdcardWaitCardInsert() != kStatus_Success)
    {
        return -1;
    }

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("Mount volume failed.\r\n");
        return -1;
    }
#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return -1;
    }
#endif


    /* 格式化SD卡 */
//#if FF_USE_MKFS
//    PRINTF("\r\nMake file system......The time may be long if the card capacity is big.\r\n");
//    if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work))
//    {
//        PRINTF("Make file system failed.\r\n");
//        return -1;
//    }
//#endif /* FF_USE_MKFS */

#if 0
    PRINTF("\r\nCreate directory......\r\n");
    error = f_mkdir(_T("/dir_4"));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("Directory exists.\r\n");
        }
        else
        {
            PRINTF("Make directory failed.\r\n");
            return -1;
        }
    }


    PRINTF("\r\nCreate a file in that directory......\r\n");
//    error = f_open(&g_fileObject, _T("/dir_1/f_1.dat"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    error = f_open(&g_fileObject, _T("/dir_4/f_4.txt"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
    if (error)
    {
        if (error == FR_EXIST)
        {
            PRINTF("File exists.\r\n");
        }
        else
        {
            PRINTF("Open file failed.\r\n");
            return -1;
        }
    }
#endif
    /* 创建目录
     * 暂时不用
     *  */
//    PRINTF("\r\nCreate a directory in that directory......\r\n");
//    error = f_mkdir(_T("/dir_1/dir_2"));
//    if (error)
//    {
//        if (error == FR_EXIST)
//        {
//            PRINTF("Directory exists.\r\n");
//        }
//        else
//        {
//            PRINTF("Directory creation failed.\r\n");
//            return -1;
//        }
//    }



    ret = f_stat("LOG",&fno);
    if ( FR_NO_FILE == ret ) {
    	PRINTF("LOG director is not exist.create it now! \r\n");
    	ret=f_mkdir("/LOG");
    	if( FR_OK != ret ) {
    		PRINTF("ERROR mkdir log \r\n");
    	}
    }


//    memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
//    g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
//    g_bufferWrite[BUFFER_SIZE - 1U] = '\n';
//
//    PRINTF("\r\nWrite/read file until encounters error......\r\n");


    while(1) {


    	memset(path, 0, sizeof(path));
    	sprintf(path, "/LOG/%d%02d%02d/log%03u.ulg", 2019,
    							9, 4, 1);
    	/* check if file exist */
    	ret = f_stat(path,&fno);
    	if( FR_OK==ret ) {
    		continue;
    	} else if( FR_NO_FILE == ret ) {
    		break;
    	} else {
    		PRINTF("ERROR path");
    		while(1);
    	}

    	/* create new log file */
		ret = f_open(&fil, path,
		FA_READ | FA_WRITE | FA_CREATE_NEW);
		if ( FR_OK != ret ) {
			PRINTF("ERROR OPEN \r\n");
		}

		ret = f_sync(&fil);
		if( FR_OK != ret ) {
			PRINTF("ERROR SYNC! \r\n");
		}




//        PRINTF("\r\nWrite to above created file.\r\n");
//        error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
//        if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
//        {
//            PRINTF("Write file failed. \r\n");
//            failedFlag = true;
//            continue;
//        }
//
//        /* Move the file pointer */
//        if (f_lseek(&g_fileObject, 0U))
//        {
//            PRINTF("Set file pointer position failed. \r\n");
//            failedFlag = true;
//            continue;
//        }


//    	f_open(&g_fileObject, _T("/dir_2/test.txt"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));

//    	f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
//    	f_lseek(&g_fileObject, 0U);
//    	f_close(&g_fileObject);
//
//    	delay();


    }

//    PRINTF("\r\nThe example will not read/write file again.\r\n");
//
//    if (f_close(&g_fileObject))
//    {
//        PRINTF("\r\nClose file failed.\r\n");
//        return -1;
//    }


    return 0 ;
}

static status_t sdcardWaitCardInsert(void)
{
    /* Save host information. */
    g_sd.host.base = SD_HOST_BASEADDR;
    g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
//    g_sd.usrParam.cd = &s_sdCardDetect;
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
    g_sd.usrParam.pwr = &s_sdCardPwrCtrl;
#endif


    if ( SD_Init(&g_sd) ) {
    	PRINTF("SD card init failed \r\n");
    } else {
    	PRINTF("SD card init successful \r\n");
    }

    /* SD host init function */
//    if (SD_HostInit(&g_sd) != kStatus_Success)
//    {
//        PRINTF("\r\nSD host init fail\r\n");
//        return kStatus_Fail;
//    }
//    /* power off card */
//    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);
//    /* wait card insert */
//    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success)
//    {
//        PRINTF("\r\nCard inserted.\r\n");
//        /* power on the card */
//        SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);
//    }
//    else
//    {
//        PRINTF("\r\nCard detect fail.\r\n");
//        return kStatus_Fail;
//    }

    return kStatus_Success;
}

