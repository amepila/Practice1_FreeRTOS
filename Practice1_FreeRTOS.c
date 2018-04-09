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

#define BIT2	(2)
#define BIT5	(5)
#define BIT7	(7)
#define BIT0	(0)

/**Set of pin of Buttons**/
const Button_ConfigType Buttons_Config[4] =
{
		{PORT_C,BIT2},	/**Button 1**/
		{PORT_C,BIT5},	/**Button 2**/
		{PORT_C,BIT7},	/**Button 3**/
		{PORT_C,BIT0}   /**Button 4**/
};

int main(void)
{
  	/* Init board hardware. */
	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

	/**Configurations of devices**/
	Buttons_init(Buttons_Config);
    init_UART0();
    init_UART1();
    init_SPI0();
	LCDNokia_clear();

	/***Enable IRQ's***/
	NVIC_EnableIRQ(PORTC_IRQn);
	/**Set the priority**/
	NVIC_SetPriority(PORTC_IRQn,5);

	xTaskCreate(taskINIT, "Task Init", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-2, NULL);
	vTaskStartScheduler();

    for(;;)
    {
	}
    return 0 ;
}
