/*
 * PeripheralFunctions.h
 *
 *  Created on: Mar 14, 2018
 *      Author: Andres Hernandez
 */

#ifndef PERIPHERALFUNCTIONS_H_
#define PERIPHERALFUNCTIONS_H_

#include "fsl_port.h"

/**
 * \brief A mail box type definition for serial port
 */
typedef struct
{
	uint8_t flag; /** Flag to indicate that there is new data*/
	uint8_t mailBox; /** it contains the received data*/
} UART_MailBoxType;

/**
 * \brief It defines states of FIFO
 */
typedef enum {EMPTY, NORMAL, FULL}FIFO_FlagType;

/**
 * \brief It defines the variables needed by FIFO
 */
typedef struct
{
	uint8_t data[50];
	FIFO_FlagType stateFIFO;
	uint32_t size;
}FIFO_Type;

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

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Delay for a time period
 	 \param[in]  delay Value of delay
 	 \return void
 */
void delay(uint32_t delay);

/*!
 	 \brief	 Multiplies the number by 10 depending of its position
 	 \param[in]  limit Value to convert
 	 \return Number converted
 */
uint32_t expBASE10(uint8_t limit);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Convert the ASCII number into Int
 	 \param[in]  string The value in ASCII
 	 \return Integer converted
 */
uint32_t Convert_numberASCIItoDATA(uint8_t *string);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Convert the ASCI word into Char
 	 \param[in]  word The character
 	 \return character converted
 */
uint8_t Convert_wordASCIItoDATA(uint8_t word);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pop the data into FIFO
 	 \param[in]  void
 	 \return Data Type of FIFO
 */
FIFO_Type popFIFO_0(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Pop the data into FIFO
 	 \param[in]  void
 	 \return Data Type of FIFO
 */
FIFO_Type popFIFO_1(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Push the data into FIFO
 	 \param[in]  character The value to set
 	 \return Flag of FIFO
 */
FIFO_FlagType pushFIFO_0(uint8_t character);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Push the data into FIFO
 	 \param[in]  character The value to set
 	 \return Flag of FIFO
 */
FIFO_FlagType pushFIFO_1(uint8_t character);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Clear the FIFO
 	 \param[in]  void
 	 \return Flag of FIFO
 */
FIFO_FlagType clearFIFO_0(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Clear the FIFO
 	 \param[in]  void
 	 \return Flag of FIFO
 */
FIFO_FlagType clearFIFO_1(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Print the hour and date in LCD
 	 \param[in]  time Data of time
 	 \return void
 */
void printTimeLCD(Time_Type time);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Set the time into RTC
 	 \param[in]  time Data of time
 	 \return void
 */
void setTimeLCD(Time_Type time);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Get the current time of RTC
 	 \param[in]  void
 	 \return The value of Time
 */
Time_Type getTime();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Print the hour and date in UART
 	 \param[in]  time Data of time
 	 \return void
 */
void printHourUART(Time_Type time);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Print the hour and date in UART
 	 \param[in]  time Data of time
 	 \return void
 */
void printDateUART(Time_Type time);


#endif /* PERIPHERALFUNCTIONS_H_ */
