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

#define EVENT_RI2C_ADDRESS		(1<<0)
#define EVENT_RI2C_LENGTH		(1<<1)
#define EVENT_RI2C_DATA			(1<<2)
#define EVENT_RI2C_FINAL		(1<<3)

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
#define QUEUE_ELEMENTS			(1)
#define LENGTH_DATA_MEMORY		(20)

SemaphoreHandle_t g_semaphore_Init;
QueueHandle_t g_Queue_ReadI2C;
QueueHandle_t g_QueueFlags_ReadI2C;

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

typedef enum
{
	ADDRESS,
	LENGTH,
	DATA
}DataType_RI2C;

typedef struct
{
	uint32_t data[20];
	uint32_t phase;
	DataType_RI2C type;
}DataTransfer_ReadI2C_Type;

typedef enum
{
	ADDRESS,
	SIZE,
	DATA
}DataType_WI2C;

typedef struct
{
	uint8_t data[LENGTH_DATA_MEMORY];
	uint8_t size;
	uint32_t address;
	uint32_t phase;
	DataType_WI2C type;
}DataTransfer_WriteI2C_Type;

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
    uart1Config.baudRate_Bps = 9600U;

    UART_Init(UART1,&uart1Config,CLOCK_GetFreq(kCLOCK_CoreSysClk));
    UART_TransferCreateHandle(UART1, &g_uart1Handle,
    		uart1_transfer_callback, NULL);

	return (kStatus_Success);
}

void taskINIT(void *arg)
{
	g_semaphore_Init = xSemaphoreCreateBinary();
	g_button_events = xEventGroupCreate();

	g_Queue_ReadI2C = xQueueCreate(3,
			sizeof(DataTransfer_ReadI2C_Type));

	g_Queue_WriteI2C = xQueueCreate(3,
			sizeof(DataTransfer_Type));
#if 0
	g_Queue_SetHour = xQueueCreate(QUEUE_ELEMENTS,
			sizeof(DataTransfer_Type));
	g_Queue_SetDate = xQueueCreate(QUEUE_ELEMENTS,
			sizeof(DataTransfer_Type));
	g_Queue_Format = xQueueCreate(QUEUE_ELEMENTS,
			sizeof(DataTransfer_Type));
	g_Queue_ReadHour = xQueueCreate(QUEUE_ELEMENTS,
			sizeof(DataTransfer_Type));
	g_Queue_ReadDate = xQueueCreate(QUEUE_ELEMENTS,
			sizeof(DataTransfer_Type));
	g_Queue_WriteI2C = xQueueCreate(QUEUE_ELEMENTS,
			sizeof(DataTransfer_Type));
	g_Queue_Eco = xQueueCreate(QUEUE_ELEMENTS,
			sizeof(DataTransfer_Type));
#endif

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
	xTaskCreate(taskMENU_Write, "Write_Menu",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

	xTaskCreate(taskREADI2C_Address, "ReadI2C_Address",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskREADI2C_Lenght, "ReadI2C_Lenght",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskREADI2C_Data, "ReadI2C_Data",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskREADI2C_FinalRead, "ReadI2C_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

	xTaskCreate(taskWRITEI2C_AddressWrite, "WriteI2C_Address",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskWRITEI2C_DataWrite, "WriteI2C_Write",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskWRITEI2C_FinalWrite, "WriteI2C_Final",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(taskWRITEI2C_FinalWriteI2C, "WriteI2C_FinalI2C",
			configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
#if 0
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
	DataTransfer_ReadI2C_Type *data_queue;

	g_receiveXUart0.data = data;
	g_receiveXUart0.dataSize = sizeof(data);
	rx0Finished = false;

	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsReadI2C,
				(EVENT_RI2C_ADDRESS), pdTRUE,
				pdTRUE, portMAX_DELAY);

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
		realAddress = Convert_numberASCIItoDATA(data);
		phase = 1;

		data_queue = pvPortMalloc(sizeof(DataTransfer_ReadI2C_Type));
		data_queue->data = realAddress;
		data_queue->phase = phase;
		data_queue->type = ADDRESS;
		xQueueSend(g_Queue_ReadI2C, &data_queue, portMAX_DELAY);

		xEventGroupSetBits(g_eventsMenus, EVENT_READ_MENU);
		xEventGroupGetBits(g_eventsMenus);
	}
}

void taskREADI2C_Lenght(void *arg)
{
	uint8_t counter = 0;
	uint8_t data[3] = {0};
	uint32_t realLength = 0;
	uint8_t phase;
	DataTransfer_ReadI2C_Type *data_queue;

	g_receiveXUart0.data = data;
	g_receiveXUart0.dataSize = sizeof(data);
	rx0Finished = false;

	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsReadI2C,
				(EVENT_RI2C_LENGTH), pdTRUE,
				pdTRUE, portMAX_DELAY);

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
		realLength = Convert_numberASCIItoDATA(data);
		phase = 2;

		data_queue = pvPortMalloc(sizeof(DataTransfer_ReadI2C_Type));
		data_queue->data = realLength;
		data_queue->phase = phase;
		data_queue->type = LENGTH;
		xQueueSend(g_Queue_ReadI2C, &data_queue, portMAX_DELAY);

		xEventGroupSetBits(g_eventsMenus, EVENT_READ_MENU);
		xEventGroupGetBits(g_eventsMenus);
	}
}

void taskREADI2C_Data(void *arg)
{
	uint8_t counterChar;
	uint32_t realLength;
	uint32_t realAddress;
	uint8_t phase = 3;
	DataTransfer_ReadI2C_Type *data_queueReceived;
	DataTransfer_ReadI2C_Type *data_queueSend;

	const uint8_t test[8] = "Hola\n";

	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsReadI2C,
				(EVENT_RI2C_DATA), pdTRUE,
				pdTRUE, portMAX_DELAY);

		/**Queue is received with the address and length*/
		do
		{
			xQueueReceive(g_Queue_ReadI2C, &data_queueReceived,
					portMAX_DELAY);
			switch(data_queueReceived->type)
			{
			case (ADDRESS):
					realAddress = data_queueReceived->data;
				break;
			case (LENGTH):
					realLength = data_queueReceived->data;
				break;
			default:
				break;
			}
		}
		while(0 != uxQueueMessagesWaiting(g_Queue_ReadI2C));

		/**Print the data memory*/
		for(counterChar = 0; counterChar < realLength; counterChar++)
		{
#if 0
			UART_WriteBlocking(UART0,
			readMemory(realAddress + counterChar),
			sizeof(uint8_t));
			E2PROM(6500);
#endif
		}
		/**Testing*/
		UART_WriteBlocking(UART0, test, sizeof(test));

		/**Send the final element of the queue*/
		data_queueSend = pvPortMalloc(sizeof(DataTransfer_ReadI2C_Type));
		data_queueSend->phase = phase;
		data_queueSend->type = DATA;
		xQueueSend(g_Queue_ReadI2C, &data_queueSend, portMAX_DELAY);

		xEventGroupSetBits(g_eventsMenus, EVENT_READ_MENU);
		xEventGroupGetBits(g_eventsMenus);
	}
}


void taskREADI2C_FinalRead(void *arg)
{

	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsReadI2C,
				(EVENT_RI2C_FINAL), pdTRUE,
				pdTRUE, portMAX_DELAY);

		xSemaphoreGive(g_semaphore_Init);
	}
}

void taskWRITEI2C_AddressWrite(void *arg)
{
	uint8_t counter = 0;
	uint8_t data[5] = {0};
	uint32_t realAddress = 0;
	uint8_t phase;
	DataTransfer_WriteI2C_Type *data_queue;

	g_receiveXUart0.data = data;
	g_receiveXUart0.dataSize = sizeof(data);
	rx0Finished = false;

	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsWriteI2C,
				(EVENT_WI2C_ADDRESS), pdTRUE,
				pdTRUE, portMAX_DELAY);

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
		realAddress = Convert_numberASCIItoDATA(data);
		phase = 1;

		data_queue = pvPortMalloc(sizeof(DataTransfer_WriteI2C_Type));
		data_queue->address = realAddress;
		data_queue->phase = phase;
		data_queue->type = ADDRESS;
		xQueueSend(g_Queue_WriteI2C, &data_queue, portMAX_DELAY);

		xEventGroupSetBits(g_eventsMenus, EVENT_WRITE_MENU);
		xEventGroupGetBits(g_eventsMenus);
	}
}

void taskWRITEI2C_DataWrite(void *arg)
{
	uint8_t counter;
	uint8_t counter2;
	uint8_t sizeData;
	uint8_t data[LENGTH_DATA_MEMORY];
	uint8_t phase;
	DataTransfer_WriteI2C_Type *data_queue;

	g_receiveXUart0.data = data;
	g_receiveXUart0.dataSize = sizeof(data);
	rx0Finished = false;

	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsWriteI2C,
				(EVENT_WI2C_DATA), pdTRUE,
				pdTRUE, portMAX_DELAY);

		UART_TransferReceiveNonBlocking(UART0,
				&g_uart0Handle, &g_receiveXUart0, NULL);

		while (!rx0Finished)
		{
			for(counter = 0; counter < LENGTH_DATA_MEMORY; counter++)
			{
				if(data[counter] != CR)
				{
					UART_WriteBlocking(UART0,
							&data[counter], sizeof(uint8_t));
				}

				if(CR == data[counter])
				{
					sizeData = counter;

					for(counter2 = counter;
							counter2 < LENGTH_DATA_MEMORY; counter2++)
					{
						data[counter2] = NULL;
					}
				}
			}
		}
		phase = 2;
		data_queue = pvPortMalloc(sizeof(DataTransfer_WriteI2C_Type));

		for(counter = 0; counter < LENGTH_DATA_MEMORY; counter++)
		{
			data_queue->data[counter] = data[counter];
		}

		data_queue->phase = phase;
		data_queue->type = ADDRESS;
		data_queue->size = sizeData;
		xQueueSend(g_Queue_WriteI2C, &data_queue, portMAX_DELAY);

		xEventGroupSetBits(g_eventsMenus, EVENT_WRITE_MENU);
		xEventGroupGetBits(g_eventsMenus);
	}
}

void taskWRITEI2C_FinalWrite(void *arg)
{
	uint8_t counterSave;
	uint8_t counterAddress = 0;
	uint32_t address;
	uint8_t size;
	uint8_t phase;
	uint32_t data[LENGTH_DATA_MEMORY];
	DataTransfer_WriteI2C_Type *data_queueReceived;
	DataTransfer_WriteI2C_Type *data_queueSend;

	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsWriteI2C,
				(EVENT_WI2C_FINAL), pdTRUE,
				pdTRUE, portMAX_DELAY);

		/**Queue is received to get the phase*/
		do
		{
			xQueueReceive(g_Queue_WriteI2C, &data_queueReceived,
					portMAX_DELAY);
			if(1 == data_queueReceived->phase)
			{
				address = data_queueReceived->address;
			}
			if(2 == data_queueReceived->phase)
			{
				size = data_queueReceived->size;
				for(counter = 0; counter < LENGTH_DATA_MEMORY;
						counter++)
				{
					data[counter] = data_queueReceived->data[counter];
				}
			}
		}
		while(0 != uxQueueMessagesWaiting(g_Queue_WriteI2C));

		for(counterSave = size; counterSave != 0; counterSave--)
		{
			writeMemory((address + counterAddress), data[counter]);
		}

		phase = 3;
		data_queueSend = pvPortMalloc(sizeof(DataTransfer_WriteI2C_Type));
		data_queueSend->phase = phase;
		xQueueSend(g_Queue_WriteI2C, &data_queueSend, portMAX_DELAY);
	}
}

void taskWRITEI2C_FinalWriteI2C(void *arg)
{
	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsReadI2C,
				(EVENT_WI2C_FINALI2C), pdTRUE,
				pdTRUE, portMAX_DELAY);

		xSemaphoreGive(g_semaphore_Init);
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
	uint8_t data[2] = {0};
	uint8_t lock = FALSE;
	uint8_t lockMenu = FALSE;

	g_receiveXUart0.data = data;
	g_receiveXUart0.dataSize = sizeof(data);
	rx0Finished = false;
#if 0
	/**Send the struct to RTC**/
	setTimeLCD(*rtcTime);
#endif
	for(;;)
	{
		xSemaphoreTake(g_semaphore_Init, portMAX_DELAY);

		if(FALSE == lockMenu)
		{
			menu_Main0();
			//lockMenu = TRUE;
		}
#if 0
		/**Print the time in LCD*/
		printTimeLCD(*rtcTime);
#endif
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
				xEventGroupSetBits(g_eventsMenus,
						EVENT_READ_MENU);
			}
			if(ASCII_2 == data[0])
			{
				xEventGroupSetBits(g_eventsMenus,
						EVENT_WRITE_MENU);
			}
			if(ASCII_3 == data[0])
			{
				xEventGroupSetBits(g_eventsMenus,
						EVENT_HOUR_MENU);
			}
			if(ASCII_4 == data[0])
			{
				xEventGroupSetBits(g_eventsMenus,
						EVENT_DATE_MENU);
			}
			if(ASCII_5 == data[0])
			{
				xEventGroupSetBits(g_eventsMenus,
						EVENT_FORMAT_MENU);
			}
			if(ASCII_6 == data[0])
			{
				xEventGroupSetBits(g_eventsMenus,
						EVENT_RHOUR_MENU);
			}
			if(ASCII_7 == data[0])
			{
				xEventGroupSetBits(g_eventsMenus,
						EVENT_RHOUR_MENU);
			}
			if(ASCII_8 == data[0])
			{
				xEventGroupSetBits(g_eventsMenus,
						EVENT_TERMINAL_MENU);
			}
			if(ASCII_9 == data[0])
			{
				xEventGroupSetBits(g_eventsMenus,
						EVENT_ECO_MENU);
			}
		}
	}
}

void taskMENU_Read(void *arg)
{
	uint32_t phase = 0;
	uint8_t lockAddress = FALSE;
	uint8_t lockLength = FALSE;
	uint8_t lockData = FALSE;
	DataTransfer_ReadI2C_Type *data_queue;

	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsMenus,
				(EVENT_READ_MENU), pdTRUE,
				pdTRUE, portMAX_DELAY);

		/**Print the menu by parts*/
		menu_ReadI2C(phase);

		/**Queue is received to get the phase*/
		do
		{
			xQueueReceive(g_Queue_ReadI2C, &data_queue, portMAX_DELAY);
			if((ADDRESS == data_queue->type) && (FALSE == lockAddress))
			{
				phase = data_queue->phase;
				lockAddress = TRUE;
			}
			if((LENGTH == data_queue->type) && (FALSE == lockLength))
			{
				phase = data_queue->phase;
				lockLength = TRUE;
			}
			if((DATA == data_queue->type) && (FALSE == lockData))
			{
				phase = data_queue->phase;
				lockData = TRUE;
			}
		}
		while(0 != uxQueueMessagesWaiting(g_Queue_ReadI2C));


		/**Set the flag event to jump to the next task*/
		xEventGroupGetBits(g_eventsReadI2C);

		if(0 == phase)
		{
			/**Jump to ReadI2C Address*/
			xEventGroupSetBits(g_eventsReadI2C, EVENT_RI2C_ADDRESS);
		}
		if(1 == phase)
		{
			/**Jump to ReadI2C Length*/
			xEventGroupSetBits(g_eventsReadI2C, EVENT_RI2C_LENGTH);
		}
		if(2 == phase)
		{
			/**Jump to ReadI2C Data*/
			xEventGroupSetBits(g_eventsReadI2C, EVENT_RI2C_DATA);
		}
		if(3 == phase)
		{
			/**Jump to ReadI2C Final*/
			xEventGroupSetBits(g_eventsReadI2C, EVENT_RI2C_FINAL);
		}
	}
}

void taskMENU_Write(void *arg)
{
	uint8_t phase = 0;
	uint8_t lockAddress = FALSE;
	uint8_t lockLength = FALSE;
	uint8_t counterChar;
	DataTransfer_WriteI2C_Type *data_queue;

	for(;;)
	{
		/**Wait the event flag to continue the task*/
		xEventGroupWaitBits(g_eventsMenus,
				(EVENT_WRITE_MENU), pdTRUE,
				pdTRUE, portMAX_DELAY);

		/**Print the menu by parts*/
		menu_ReadI2C(phase);

		/**Queue is received to get the phase*/
		do
		{
			xQueueReceive(g_Queue_WriteI2C, &data_queue, portMAX_DELAY);
			if((ADDRESS == data_queue->type) && (FALSE == lockAddress))
			{
				phase = data_queue->phase;
				lockAddress = TRUE;
			}
			if((LENGTH == data_queue->type) && (FALSE == lockLength))
			{
				phase = data_queue->phase;
				lockLength = TRUE;
			}
		}
		while(0 != uxQueueMessagesWaiting(g_Queue_WriteI2C));

		/**Set the flag event to jump to the next task*/
		xEventGroupGetBits(g_eventsWriteI2C);

		if(0 == phase)
		{
			/**Jump to WriteI2C Address*/
			xEventGroupSetBits(g_eventsWriteI2C, EVENT_WI2C_ADDRESS);
		}
		if(1 == phase)
		{
			/**Jump to WriteI2C Data*/
			xEventGroupSetBits(g_eventsWriteI2C, EVENT_WI2C_DATA);
		}
		if(2 == phase)
		{
			/**Jump to WriteI2C Final*/
			xEventGroupSetBits(g_eventsWriteI2C, EVENT_WI2C_FINAL);
		}
		if(3 == phase)
		{
			/**Jump to WriteI2C FinalI2C*/
			xEventGroupSetBits(g_eventsWriteI2C, EVENT_WI2C_FINALI2C);
		}
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
