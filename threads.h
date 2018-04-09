/*
 * Task.h
 *
 *  Created on: Mar 18, 2018
 *      Author: Andres Hernandez
 */

#ifndef THREADS_H_
#define THREADS_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"

/**
 * \brief This data type define the states of hour format
 */
typedef enum{NON_FORMAT,FORMAT_12H, FORMAT_24H}FORMAT_HOUR;

/**
 * \brief This data type define the time periods of the hour
 */
typedef enum{NON_PERIOD,PERIOD_AM, PERIOD_PM}PERIOD_TIME;

/**
 * \brief This data type define the structure of Hour
 */
typedef struct{
	uint32_t	hour;
	uint32_t	minutes;
	uint32_t	seconds;
	FORMAT_HOUR format;
	PERIOD_TIME period;
}Hour_Type;

/**
 * \brief This data type define the structure of date
 */
typedef struct
{
	uint32_t	year;
	uint32_t	month;
	uint32_t	day;
}Date_Type;

/**
 * \brief This data type define the Lock Flags to general use
 */
typedef struct
{
	uint8_t flag1 : 1;
	uint8_t flag2 : 1;
	uint8_t flag3 : 1;
	uint8_t flag4 : 1;
	uint8_t flag5 : 1;
	uint8_t flag6 : 1;
	uint8_t flag7 : 1;
	uint8_t flag8 : 1;
}LockFlags_Type;

/**
 * \brief This data type define the structure of Time
 */
typedef struct
{
	Hour_Type hour;
	Date_Type date;
	uint32_t modifyTime;
	uint32_t modifyDate;
}Time_Type;

void init_UART0(void);

void init_UART1(void);

void fifoByte_UART0(uint8_t *byte);

void taskINIT(void *arg);


void taskREADI2C_Read(void *arg);


void taskWRITEI2C_Write(void *arg);


void taskSETHOUR_SetTime(void *arg);


void taskSETDATE_SetCalendar(void *arg);


void taskFORMAT_ShowFormat(void *arg);


void taskREADHOUR_ReadTime(void *arg);


void taskREADDATE_ReadCalendar(void *arg);

void taskTERMINAL_1(void *arg);

void taskTERMINAL_2(void *arg);

void taskECO_TransmitECO(void *arg);


void taskMENU_Menu(void *arg);

void taskMENU_Read(void *arg);

void taskMENU_Write(void *arg);

void taskMENU_SetHour(void *arg);

void taskMENU_SetDate(void *arg);

void taskMENU_Format(void *arg);

void taskMENU_ReadHour(void *arg);

void taskMENU_ReadDate(void *arg);

void taskMENU_Terminal2(void *arg);

void taskMENU_Eco(void *arg);


#endif /* THREADS_H_ */
