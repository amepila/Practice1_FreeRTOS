/*
 * Tasks.c
 *
 *  Created on: Mar 18, 2018
 *      Author: Andres Hernandez
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
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
#include "Task.h"

#define BUTTON_1	(1<<2)
#define BUTTON_2	(1<<5)
#define BUTTON_3	(1<<7)
#define BUTTON_4	(1<<0)

#define EVENT_BUTTON1	(1<<0)
#define EVENT_BUTTON2	(1<<1)
#define EVENT_BUTTON3	(1<<2)
#define EVENT_BUTTON4	(1<<3)

#define EVENT_READ_MENU			(1<<0)
#define EVENT_WRITE_MENU		(1<<1)
#define EVENT_HOUR_MENU			(1<<2)
#define EVENT_DATE_MENU			(1<<3)
#define EVENT_FORMAT_MENU		(1<<4)
#define EVENT_RHOUR_MENU		(1<<5)
#define EVENT_RDATE_MENU		(1<<6)
#define EVENT_TERMINAL_MENU		(1<<7)
#define EVENT_ECO_MENU			(1<<8)

#define EVENT_R12C_ADDRESS		(1<<0)
#define EVENT_R12C_LENGHT		(1<<1)
#define EVENT_R12C_DATA			(1<<2)
#define EVENT_R12C_FINAL		(1<<0)

#define EVENT_WI2C_ADDRESS		(1<<0)
#define EVENT_WI2C_DATA			(1<<1)
#define EVENT_WI2C_FINAL		(1<<2)
#define EVENT_WI2C_FINALI2C		(1<<3)

#define EVENT_HOUR_SET			(1<<0)
#define EVENT_HOUR_SAVE			(1<<1)
#define EVENT_HOUR_FINAL		(1<<2)

#define EVENT_DATE_SET			(1<<0)
#define EVENT_DATE_SAVE			(1<<1)
#define EVENT_DATE_FINAL		(1<<2)

#define EVENT_FORMAT_SHOW		(1<<0)
#define EVENT_FORMAT_CHANGE		(1<<1)
#define EVENT_FORMAT_SAVE		(1<<2)
#define EVENT_FORMAT_FINAL		(1<<3)

#define EVENT_RHOUR_READ		(1<<0)
#define EVENT_RHOUR_FINAL		(1<<1)

#define EVENT_RDATE_READ		(1<<0)
#define EVENT_RDATE_FINAL		(1<<1)

#define EVENT_ECO_TRANS			(1<<0)
#define EVENT_ECO_FINAL			(1<<1)

#define LENGHT_UART				(2)
#define TRUE					(1)
#define FALSE					(0)

SemaphoreHandle_t g_semaphore_Init;
QueueHandle_t g_timeQueue_Main;

EventGroupHandle_t g_eventsMenus;
EventGroupHandle_t g_button_events;
EventGroupHandle_t g_eventsReadI2C;
EventGroupHandle_t g_eventsWriteI2C;
EventGroupHandle_t g_eventsSetHour;
EventGroupHandle_t g_eventsSetDate;
EventGroupHandle_t g_eventsFormat;
EventGroupHandle_t g_eventsReadHour;
EventGroupHandle_t g_eventsReadDate;
EventGroupHandle_t g_eventsEco;

uart_handle_t g_uart0Handle;
uart_transfer_t g_receiveXUart0;
volatile bool rx0Finished;

uart_handle_t g_uart1Handle;
uart_transfer_t g_receiveXUart1;
volatile bool rx1Finished;

/**Structure with the time*/
Time_Type g_Time =
{
		{1, 0, 0, FORMAT_24H, NON_PERIOD},
		{2018, 3, 28},
		1,
		1
};

Time_Type *rtcTime = &g_Time;

typedef enum
{
	ASCII_1 = 49,
	ASCII_2 = 50,
	ASCII_3 = 51,
	ASCII_4 = 52,
	ASCII_5 = 53,
	ASCII_6 = 54,
	ASCII_7 = 55,
	ASCII_8 = 56,
	ASCII_9 = 57
}g_codeASCII_num;

/**ASCII Code to Esc and CR*/
const uint8_t ESC = 27;
const uint8_t CR = 13;

void PORTC_IRQHandler()
{
	uint32_t buttonPressed;
	BaseType_t xHigherPriorityTaskWoken;

	buttonPressed = PORT_GetPinsInterruptFlags(PORTC);

	if(BUTTON_1 == buttonPressed)
	{
		PORT_ClearPinsInterruptFlags(PORTC, BUTTON_1);
		xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(g_button_events,
				EVENT_BUTTON1, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}

	if(BUTTON_2 == buttonPressed)
	{
		PORT_ClearPinsInterruptFlags(PORTC, BUTTON_2);
		xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(g_button_events,
				EVENT_BUTTON2, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	if(BUTTON_3 == buttonPressed)
	{
		PORT_ClearPinsInterruptFlags(PORTC, BUTTON_3);
		xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(g_button_events,
				EVENT_BUTTON3, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	if(BUTTON_4 == buttonPressed)
	{
		PORT_ClearPinsInterruptFlags(PORTC, BUTTON_4);
		xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR(g_button_events,
				EVENT_BUTTON4, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}

static void uart0_transfer_callback(UART_Type *base, uart_handle_t *handle,
		status_t status, void *userData)
{
	if (kStatus_UART_RxIdle == status)
	{
		rx0Finished = true;
	}
}

static void uart1_transfer_callback(UART_Type *base, uart_handle_t *handle,
		status_t status, void *userData)
{
	if (kStatus_UART_RxIdle == status)
	{
		rx1Finished = true;
	}
}

status_t init_UART0(void)
{
    uart_config_t uart0Config;

    UART_GetDefaultConfig(&uart0Config);

    uart0Config.enableRx = true;
    uart0Config.enableTx = true;

    UART_Init(UART0,&uart0Config,CLOCK_GetFreq(kCLOCK_CoreSysClk));
    UART_TransferCreateHandle(UART0, &g_uart0Handle,
    		uart0_transfer_callback, NULL);

	return (kStatus_Success);
}

status_t init_UART1(void)
{
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_Uart1);

	port_pin_config_t config_uart1 =
	{ 		kPORT_PullDisable, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAlt3,
	        kPORT_UnlockRegister
	};

	PORT_SetPinConfig(PORTC, 4, &config_uart1);	//Tx
	PORT_SetPinConfig(PORTC, 3, &config_uart1);	//Rx

    uart_config_t uart1Config;

    UART_GetDefaultConfig(&uart1Config);

    uart1Config.enableRx = true;
    uart1Config.enableTx = true;
    uart1Config.baudRate_Bps = 38400U;

    UART_Init(UART1,&uart1Config,CLOCK_GetFreq(kCLOCK_CoreSysClk));
    UART_TransferCreateHandle(UART1, &g_uart1Handle,
    		uart1_transfer_callback, NULL);

	return (kStatus_Success);
}

void taskINIT(void *arg)
{
	g_semaphore_Init = xSemaphoreCreateBinary();
	g_button_events = xEventGroupCreate();
	g_timeQueue_Main = xQueueCreate(1,sizeof(Time_Type*));

	g_eventsMenus = xEventGroupCreate();
	g_eventsReadI2C = xEventGroupCreate();
	g_eventsWriteI2C = xEventGroupCreate();
	g_eventsSetHour = xEventGroupCreate();
	g_eventsSetDate = xEventGroupCreate();
	g_eventsFormat = xEventGroupCreate();
	g_eventsReadHour = xEventGroupCreate();
	g_eventsReadDate = xEventGroupCreate();
	g_eventsEco = xEventGroupCreate();

	xTaskCreate(taskMENU_Menu, "Main_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskMENU_Read, "Read_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);


	xTaskCreate(taskREADI2C_Address, "ReadI2C_Address",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskREADI2C_Lenght, "ReadI2C_Lenght",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskREADI2C_Data, "ReadI2C_Data",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskREADI2C_FinalRead, "ReadI2C_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
#if 0
	xTaskCreate(taskWRITEI2C_AddressWrite, "WriteI2C_Address",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskWRITEI2C_DataWrite, "WriteI2C_Write",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskWRITEI2C_FinalWrite, "WriteI2C_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskWRITEI2C_FinalWriteI2C, "WriteI2C_FinalI2C",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

	xTaskCreate(taskSETHOUR_SetTime, "SetHour_Set",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskSETHOUR_SaveTime, "SetHour_Save",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskSETHOUR_FinalSetHour, "SetHour_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

	xTaskCreate(taskSETDATE_SetCalendar, "SetDate_Set",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskSETDATE_SaveDate, "SetDate_Save",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskSETDATE_FinalSetDate, "SetDate_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

	xTaskCreate(taskFORMAT_ShowFormat, "Format_Show",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskFORMAT_Change, "Format_Change",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskFORMAT_SaveFormat, "Format_Save",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskFORMAT_FinalFormat, "Format_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

	xTaskCreate(taskREADHOUR_ReadTime, "ReadHour_Read",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskREADHOUR_FinalRH, "ReadHour_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

	xTaskCreate(taskREADDATE_ReadCalendar, "ReadDate_Read",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskREADDATE_FinalRD, "ReadDate_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

	xTaskCreate(taskECO_TransmitECO, "Eco_Trans",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskECO_FinalEco, "Eco_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);



	xTaskCreate(taskMENU_Write, "Write_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskMENU_SetHour, "SetHour_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskMENU_SetDate, "SetDate_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskMENU_Format, "Format_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskMENU_ReadHour, "ReadHour_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskMENU_ReadDate, "ReadDate_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskMENU_Terminal2, "Terminal2_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskMENU_Eco, "Eco_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

#endif

	xSemaphoreGive(g_semaphore_Init);

	for(;;)
	{
	}
}

void taskREADI2C_Address(void *arg)
{
	uint8_t counter = 0;
	uint8_t data[5] = {0};
	uint32_t realAddress = 0;
	uint8_t phase;

	for(;;)
	{
		UART_TransferReceiveNonBlocking(UART0,
				&g_uart0Handle, &g_receiveXUart0, NULL);
		while (!rx0Finished)
		{
			for(counter = 0; counter < 5; counter++)
			{
				if(counter < 4)
				{
					UART_WriteBlocking(UART0,
							&data[counter], sizeof(uint8_t));
				}
			}
		}
		realAddress = Convert_numberASCIItoDATA(&data);
		phase = 1;
		/**Send realAddress, phase to the queue*/
	}
}

void taskREADI2C_Lenght(void *arg)
{
	uint8_t counter = 0;
	uint8_t data[3] = {0};
	uint32_t realLength = 0;
	uint8_t phase;

	for(;;)
	{
		UART_TransferReceiveNonBlocking(UART0,
				&g_uart0Handle, &g_receiveXUart0, NULL);
		while (!rx0Finished)
		{
			for(counter = 0; counter < 3; counter++)
			{
				if(counter < 2)
				{
					UART_WriteBlocking(UART0,
							&data[counter], sizeof(uint8_t));
				}
			}
		}
		realLength = Convert_numberASCIItoDATA(&data);
		phase = 2;
		/**Send realLength, phase to the queue*/
	}
}

void taskREADI2C_Data(void *arg)
{
	uint8_t counterChar;
	uint32_t realLength;
	uint32_t realAddress;
	uint8_t phase = 3;

	for(;;)
	{
		/**realLength, realAddress received by queue*/

		for(counterChar = 0; counterChar < realLength; counterChar++)
		{
#if 0
			UART_WriteBlocking(UART0,
			readMemory(realAddress + counterChar),
			sizeof(uint8_t));
			E2PROM(6500);
#endif
		}
	}
}


void taskREADI2C_FinalRead(void *arg)
{

	for(;;)
	{
		/**Arrive of the Read Menu*/
		/**And go to Main Menu*/
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
/**********************************************************/
void taskMENU_Menu(void *arg)
{
	xSemaphoreTake(g_semaphore_Init, portMAX_DELAY);

	uint8_t data[2] = {0};
	uint8_t lock = FALSE;
	uint8_t lockMenu = FALSE;

	g_receiveXUart0.data = data;
	g_receiveXUart0.dataSize = sizeof(data);
	rx0Finished = false;

	/**Send the struct to RTC**/
	//setTimeLCD(*rtcTime);

	for(;;)
	{
		if(FALSE == lockMenu)
		{
			menu_Main0();
			lockMenu = TRUE;
		}

		/**Print the time in LCD*/
		//printTimeLCD(*rtcTime);

		xEventGroupGetBits(g_eventsMenus);

		UART_TransferReceiveNonBlocking(UART0,
				&g_uart0Handle, &g_receiveXUart0, NULL);
		while (!rx0Finished)
		{
			if((FALSE == lock) && (data[0] != 0))
			{
			    UART_WriteBlocking(UART0, &data[0],
			    		sizeof(uint8_t));
			    lock = TRUE;
			}
		}

		if(CR == data[1])
		{
			if(ASCII_1 == data[0])
			{
				data[0] = 0;
				data[1] = 0;
				xEventGroupSetBits(g_eventsMenus,
						EVENT_READ_MENU);
			}
			if(ASCII_2 == data[0])
			{
				data[0] = 0;
				data[1] = 0;
				xEventGroupSetBits(g_eventsMenus,
						EVENT_WRITE_MENU);
			}
			if(ASCII_3 == data[0])
			{
				data[0] = 0;
				data[1] = 0;
				xEventGroupSetBits(g_eventsMenus,
						EVENT_HOUR_MENU);
			}
			if(ASCII_4 == data[0])
			{
				data[0] = 0;
				data[1] = 0;
				xEventGroupSetBits(g_eventsMenus,
						EVENT_DATE_MENU);
			}
			if(ASCII_5 == data[0])
			{
				data[0] = 0;
				data[1] = 0;
				xEventGroupSetBits(g_eventsMenus,
						EVENT_FORMAT_MENU);
			}
			if(ASCII_6 == data[0])
			{
				data[0] = 0;
				data[1] = 0;
				xEventGroupSetBits(g_eventsMenus,
						EVENT_RHOUR_MENU);
			}
			if(ASCII_7 == data[0])
			{
				data[0] = 0;
				data[1] = 0;
				xEventGroupSetBits(g_eventsMenus,
						EVENT_RHOUR_MENU);
			}
			if(ASCII_8 == data[0])
			{
				data[0] = 0;
				data[1] = 0;
				xEventGroupSetBits(g_eventsMenus,
						EVENT_TERMINAL_MENU);
			}
			if(ASCII_9 == data[0])
			{
				data[0] = 0;
				data[1] = 0;
				xEventGroupSetBits(g_eventsMenus,
						EVENT_ECO_MENU);
			}
		}
	}
}

void taskMENU_Read(void *arg)
{
	uint8_t data[2] = {0};
	uint32_t phase = 0;
	uint32_t lockUART0 = FALSE;
	uint8_t flagAddress = FALSE;
	uint8_t flagUART0 = FALSE;

	for(;;)
	{
		xEventGroupWaitBits(g_eventsMenus,
				(EVENT_READ_MENU), pdTRUE,
				pdTRUE, portMAX_DELAY);

		if(false == flagUART0)
		{
			menu_ReadI2C(phase);
		}

		if(1 == phase)
		{
			if(FALSE == flagAddress)
			{
				/**save the address of queue*/
				flagAddress = TRUE;
			}
		}

		if(2 == phase)
		{
			/**save the length of queue*/

		}

		UART_TransferReceiveNonBlocking(UART0,
				&g_uart0Handle, &g_receiveXUart0, NULL);
		while (!rx0Finished)
		{
			if((FALSE == lockUART0) && (data[0] != 0))
			{
			    UART_WriteBlocking(UART0, &data[0],
			    		sizeof(uint8_t));
			    lockUART0 = true;
			}
		}
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
