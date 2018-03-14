/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
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
 * @file    Practice1_FreeRTOS.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "LCDNokia5110.h"
#include "fsl_uart.h"

#define TEST		(1)
#define CODE_SPI 	(0)
#define CODE_UART0	(1)
#define CODE_UART0_SUB1	(0)
#define CODE_UART1	(0)

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

#if (TEST & CODE_SPI)

    dspi_master_config_t masterConfig;
    bool_t flag_PrinLCD = 1;

    DSPI_MasterGetDefaultConfig(&masterConfig);
    DSPI_MasterInit(SPI0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));

    LCDNokia_init();
    LCDNokia_clear();

#endif

#if	(TEST & CODE_UART0)

    uart_config_t uart0Config;
    const uint8_t data[15] = "HOLA MUNDO\r\n";

    UART_GetDefaultConfig(&uart0Config);

    uart0Config.enableRx = true;
    uart0Config.enableTx = true;

    UART_Init(UART0,&uart0Config,120000000U);
#if (TEST & CODE_UART0_SUB1)

    UART_EnableInterrupts(UART0, kUART_TxDataRegEmptyInterruptEnable
    		|kUART_RxDataRegFullInterruptEnable);

    interrupt = UART_GetEnabledInterrupts(UART0);

	if (kUART_TxDataRegEmptyInterruptEnable & interrupt)
	{
		printf("TEST_INTERRUPT\r\n");
	}
#endif

    UART_WriteBlocking(UART0, data, sizeof(data));

#endif

#if	(TEST & CODE_UART1)

#endif

    for(;;)
    {



#if (TES & CODE_SPI0)

    if(1 == flag_PrintLCD)
    {
    	LCDNokia_printValue(5);
    	flag_PrintLCD
    }


#endif

    }
    return 0 ;
}
