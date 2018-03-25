/*
 * Tasks.c
 *
 *  Created on: Mar 18, 2018
 *      Author: Andres Hernandez
 */


#include "PeripheralFunctions.h"
#include "LCDNokia5110.h"
#include "Buttons.h"
#include "Menu.h"
#include "RTC.h"
#include "E2PROM.h"
#include "Configurations.h"
#include "Bluetooth.h"
#include "fsl_port.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"

#define BUTTON_1	(1<<2)
#define BUTTON_2	(1<<5)
#define BUTTON_3	(1<<7)
#define BUTTON_4	(1<<0)

#define EVENT_BUTTON1	(1<<0)
#define EVENT_BUTTON2	(1<<1)
#define EVENT_BUTTON3	(1<<2)
#define EVENT_BUTTON4	(1<<3)

extern SemaphoreHandle_t g_semaphore_test;
extern EventGroupHandle_t g_button_events;
extern uint32_t g_interrupt_UART0;

void UART0_RX_TX_IRQHandler()
{
	BaseType_t xHigherPriorityTaskWoken;

	UART_ClearStatusFlags(UART0, kUART_RxActiveEdgeInterruptEnable);

	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(g_semaphore_test, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void PORTC_IRQHandler()
{
	uint32_t buttonPressed;
	BaseType_t xHigherPriorityTaskWoken;

	buttonPressed = PORT_GetPinsInterruptFlags(PORTC);

	if(BUTTON_1 == buttonPressed)
	{
		PORT_ClearPinsInterruptFlags(PORTC, BUTTON_1);
		xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(g_button_events, EVENT_BUTTON1, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}

	if(BUTTON_2 == buttonPressed)
	{
		PORT_ClearPinsInterruptFlags(PORTC, BUTTON_2);
		xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(g_button_events, EVENT_BUTTON2, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	if(BUTTON_3 == buttonPressed)
	{
		PORT_ClearPinsInterruptFlags(PORTC, BUTTON_3);
		xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(g_button_events, EVENT_BUTTON3, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	if(BUTTON_4 == buttonPressed)
	{
		PORT_ClearPinsInterruptFlags(PORTC, BUTTON_4);
		xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(g_button_events, EVENT_BUTTON4, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}

void taskINIT(void *arg)
{
	uint8_t lock = 0;
	for(;;)
	{
		xSemaphoreTake(g_semaphore_test,portMAX_DELAY);
		xSemaphoreGive(g_semaphore_test);
		if(0 == lock)
		{
			menu_Main();
			lock = 1;
		}

	    g_interrupt_UART0 = UART_GetEnabledInterrupts(UART0);
	}

}

void taskREADI2C_Address(void *arg)
{

	for(;;)
	{


	}
}

void taskREADI2C_Lenght(void *arg)
{

	for(;;)
	{

	}
}

void taskREADI2C_Data(void *arg)
{

	for(;;)
	{

	}
}


void taskREADI2C_FinalRead(void *arg)
{
	for(;;)
	{

	}
}

void taskWRITEI2C_AddressWrite(void *arg)
{

	for(;;)
	{

	}
}

void taskWRITEI2C_DataWrite(void *arg)
{

	for(;;)
	{

	}
}

void taskWRITEI2C_FinalWrite(void *arg)
{

	for(;;)
	{

	}
}

void taskWRITEI2C_FinalWriteI2C(void *arg)
{
	for(;;)
	{

	}
}

void taskSETHOUR_SetTime(void *arg)
{

	for(;;)
	{

	}
}

void taskSETHOUR_SaveTime(void *arg)
{

	for(;;)
	{

	}
}

void taskSETHOUR_FinalSetHour(void *arg)
{
	for(;;)
	{

	}
}

void taskSETDATE_SetCalendar(void *arg)
{
	for(;;)
	{

	}
}

void taskSETDATE_SaveDate(void *arg)
{

	for(;;)
	{

	}
}

void taskSETDATE_FinalSetDate(void *arg)
{

	for(;;)
	{

	}
}

void taskFORMAT_ShowFormat(void *arg)
{

	for(;;)
	{

	}
}

void taskFORMAT_Change(void *arg)
{

	for(;;)
	{

	}
}

void taskFORMAT_SaveFormat(void *arg)
{

	for(;;)
	{

	}
}

void taskFORMAT_FinalFormat(void *arg)
{

	for(;;)
	{

	}
}

void taskREADHOUR_ReadTime(void *arg)
{

	for(;;)
	{

	}
}

void taskREADHOUR_FinalRH(void *arg)
{

	for(;;)
	{

	}
}

void taskREADDATE_ReadCalendar(void *arg)
{

	for(;;)
	{

	}
}

void taskREADDATE_FinalRD(void *arg)
{

	for(;;)
	{

	}
}

void taskECO_TransmitECO(void *arg)
{

	for(;;)
	{

	}
}

void taskECO_FinalEco(void *arg)
{

	for(;;)
	{

	}
}

void taskMENU_Menu(void *arg)
{

	for(;;)
	{

	}
}

void taskMENU_Read(void *arg)
{

	for(;;)
	{

	}
}

void taskMENU_Write(void *arg)
{

	for(;;)
	{

	}
}

void taskMENU_SetHour(void *arg)
{

	for(;;)
	{

	}
}

void taskMENU_SetDate(void *arg)
{

	for(;;)
	{

	}
}

void taskMENU_Format(void *arg)
{
	for(;;)
	{

	}
}

void taskMENU_ReadHour(void *arg)
{

	for(;;)
	{

	}
}

void taskMENU_ReadDate(void *arg)
{

	for(;;)
	{

	}
}

void taskMENU_Terminal2(void *arg)
{

	for(;;)
	{

	}
}

void taskMENU_Eco(void *arg)
{

	for(;;)
	{

	}
}
