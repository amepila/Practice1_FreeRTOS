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
#include "Buttons.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"
#include "Task.h"

#define TEST			(0)
#define CODE_SPI 		(0)
#define CODE_UART0		(0)
#define CODE_UART0_SUB1	(0)

#define BIT2	(2)
#define BIT5	(5)
#define BIT7	(7)
#define BIT0	(0)

SemaphoreHandle_t g_semaphore_test;
EventGroupHandle_t g_button_events;
SemaphoreHandle_t g_mutex_uart0;
QueueHandle_t g_queue_uart0;

/**Set of pin of Buttons**/
const Button_ConfigType Buttons_Config[4] =
{
		{PORT_C,BIT2},	/**Button 1**/
		{PORT_C,BIT5},	/**Button 2**/
		{PORT_C,BIT7},	/**Button 3**/
		{PORT_C,BIT0}   /**Button 4**/
};

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    uart_config_t uart0Config;

    UART_GetDefaultConfig(&uart0Config);

    uart0Config.enableRx = true;
    uart0Config.enableTx = true;

	Buttons_init(Buttons_Config);
    UART_Init(UART0,&uart0Config,CLOCK_GetFreq(kCLOCK_CoreSysClk));

	//NVIC_EnableIRQ(PORTC_IRQn);
	//NVIC_SetPriority(PORTC_IRQn,5);

	NVIC_EnableIRQ(UART0_RX_TX_IRQn);
	NVIC_SetPriority(UART0_RX_TX_IRQn,5);

	g_button_events = xEventGroupCreate();
	g_semaphore_test = xSemaphoreCreateBinary();
	g_mutex_uart0 = xSemaphoreCreateMutex();
	g_queue_uart0 = xQueueCreate(1,sizeof(uint8_t));

	xTaskCreate(taskINIT, "Task Init", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	vTaskStartScheduler();

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
    uart_config_t uart1Config;
    const uint8_t data[15] = "HOLA MUNDO\r\n";

	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_Uart1);

	port_pin_config_t config_uart1 =
	{ 		kPORT_PullDisable, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAlt3,
	        kPORT_UnlockRegister
	};

	PORT_SetPinConfig(PORTC, 4, &config_uart1);	//Tx
	PORT_SetPinConfig(PORTC, 3, &config_uart1);	//Rx

    UART_GetDefaultConfig(&uart0Config);
    UART_GetDefaultConfig(&uart1Config);

    uart0Config.enableRx = true;
    uart0Config.enableTx = true;

    uart1Config.enableRx = true;
    uart1Config.enableTx = true;
    uart1Config.baudRate_Bps = 9600U;

    UART_Init(UART0,&uart0Config,CLOCK_GetFreq(kCLOCK_BusClk));
    UART_Init(UART1,&uart1Config,CLOCK_GetFreq(kCLOCK_BusClk));

    //UART_WriteBlocking(UART0, data, sizeof(data));

#endif

#if (TEST & CODE_UART0_SUB1)

    UART_EnableInterrupts(UART0, kUART_TxDataRegEmptyInterruptEnable
    		|kUART_RxDataRegFullInterruptEnable);

    interrupt = UART_GetEnabledInterrupts(UART0);

	if (kUART_TxDataRegEmptyInterruptEnable & interrupt)
	{
		printf("TEST_INTERRUPT\r\n");
	}
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
