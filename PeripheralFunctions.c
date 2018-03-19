/*
 * PeripheralFunctions.c
 *
 *  Created on: Mar 14, 2018
 *      Author: Andres Hernandez
 */

#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "fsl_dspi.h"
#include "PeripheralFunctions.h"

FIFO_Type FIFO_UART0;
FIFO_Type FIFO_UART1;

void delay(uint32_t delay)
{
	volatile uint32_t counter;

	for(counter=delay;counter>0;counter--)
	{
	}
}

uint32_t expBASE10(uint8_t limit)
{
	/*Use the exponent base 10 to different digits*/
	static uint32_t value;

	if(limit == 0){	value = 1;}
	if(limit == 1){	value = 10;}
	if(limit > 1){	value *= 10;}

	return value;
}

uint32_t Convert_numberASCIItoDATA(uint8_t *string)
{

	const uint32_t adjustASCII = 48;
	const uint32_t CR = 13;
	uint32_t counter1 = 0;
	uint32_t counter2 = 0;
	uint32_t counter3 = 0;
	uint32_t data;
	uint32_t tmpData1 = 0;
	uint32_t tmpData2 = 0;
	uint32_t expValue;

	/*First loop to manage find the size*/
	while(string[counter1] != CR)
	{
		counter1++;
	}

	/*Second loop to begin in last position of array*/
	for(counter2 = counter1; counter2 != 0; counter2--)
	{
		/*Elevate the number by 10 depending of its position*/
		expValue = expBASE10(counter3);

		/*Converts the ASCII to INT*/
		if(counter3 == 0)
		{
			tmpData2 = string[counter2 - 1];
			tmpData2 -= adjustASCII;
			tmpData1 += tmpData2;
		}
		/*Save the value and sums*/
		if(counter3 > 0)
		{
			tmpData2 = string[counter2 - 1];
			tmpData2 -= adjustASCII;
			tmpData2 *= expValue;
			tmpData1 += tmpData2;
		}
		counter3++;
	}

	data = tmpData1;
	return (data);
}

uint8_t Convert_wordASCIItoDATA(uint8_t word)
{
	uint8_t valueWord = (uint8_t)word;
	return (valueWord);
}

FIFO_Type popFIFO_0(void)
{

	uint32_t counterSize = 0;
	uint32_t position = 0;
	uint32_t counterClear;
	static uint32_t counterChar;
	FIFO_Type fifo;

	/*Loop to find the size*/
	while(FIFO_UART0.data[counterSize] != '\0')
	{
		counterSize++;
	}

	/*Second loop to clear the FIFO*/
	for(counterClear = 0; counterClear < 50; counterClear++)
	{
		fifo.data[counterClear] = '\0';
	}
	/*Third loop to save the value into FIFO*/
	for(counterChar = counterSize; counterChar != 0; counterChar--)
	{
		fifo.data[position] = FIFO_UART0.data[position];
		position++;
	}

	fifo.size = counterSize;
	fifo.stateFIFO = NORMAL;
	return (fifo);
}

FIFO_Type popFIFO_1(void)
{
	uint32_t counterSize = 0;
	uint32_t position = 0;
	uint32_t counterClear;
	static uint32_t counterChar;
	FIFO_Type fifo;

	/*Loop to find the size*/
	while(FIFO_UART1.data[counterSize] != '\0')
	{
		counterSize++;
	}
	/*Second loop to clear the FIFO*/
	for(counterClear = 0; counterClear < 50; counterClear++)
	{
		fifo.data[counterClear] = '\0';
	}
	/*Third loop to save the data into FIFO*/
	for(counterChar = counterSize; counterChar != 0; counterChar--)
	{
		fifo.data[position] = FIFO_UART1.data[position];
		position++;
	}

	fifo.size = counterSize;
	fifo.stateFIFO = NORMAL;
	return (fifo);
}

FIFO_FlagType pushFIFO_0(uint8_t character)
{

	static uint32_t counterChar = 0;
	const uint32_t CR = 13;

	/*Get the info into FIFO until find the CR value*/
	if(character != CR)
	{
		FIFO_UART0.data[counterChar] = character;
		counterChar++;
		FIFO_UART0.stateFIFO = NORMAL;
		FIFO_UART0.size = 0;
	}
	else
	{
		FIFO_UART0.data[counterChar] = character;
		FIFO_UART0.size = counterChar;
		counterChar = 0;
		FIFO_UART0.stateFIFO = NORMAL;
		if(FIFO_UART0.size >= 50)
		{
			FIFO_UART0.stateFIFO = FULL;
		}
	}
	return (FIFO_UART0.stateFIFO);

}

FIFO_FlagType pushFIFO_1(uint8_t character)
{

	static uint32_t counterChar = 0;
	const uint32_t CR = 13;

	/*Get the info into FIFO until find the CR value*/
	if(character != CR)
	{
		FIFO_UART1.data[counterChar] = character;
		counterChar++;
		FIFO_UART1.stateFIFO = NORMAL;
	}
	else
	{
		FIFO_UART1.data[counterChar] = character;
		FIFO_UART1.size = counterChar;
		counterChar = 0;
		FIFO_UART1.stateFIFO = NORMAL;
		if(FIFO_UART1.size >= 50)
		{
			FIFO_UART1.stateFIFO = FULL;
		}
	}
	return (FIFO_UART1.stateFIFO);
}

FIFO_FlagType clearFIFO_0(void)
{
	uint32_t counter;

	/*Clear the FIFO*/
	for(counter = 0; counter < 50; counter++)
	{
		FIFO_UART0.data[counter] = '\0';
	}
	FIFO_UART0.size = 0;
	FIFO_UART0.stateFIFO = EMPTY;

	return (FIFO_UART0.stateFIFO);
}

FIFO_FlagType clearFIFO_1(void)
{
	uint32_t counter;

	/*Clear the FIFO*/
	for(counter = 0; counter < 50; counter++)
	{
		FIFO_UART1.data[counter] = '\0';
	}
	FIFO_UART1.size = 0;
	FIFO_UART1.stateFIFO = EMPTY;

	return (FIFO_UART1.stateFIFO);
}

void setTimeLCD(Time_Type time)
{
	/**Set the hour*/
	if(TRUE == time.modifyTime)
	{
		setRTC_sec((uint8_t)time.hour.seconds);
		setRTC_min((uint8_t)time.hour.minutes);
		setRTC_hour((uint8_t)time.hour.hour);
		/**Save the info into Clock*/
		Clock = time;
		time.modifyTime= FALSE;
	}
	/**Set the date*/
	if(TRUE == time.modifyDate)
	{
		setRTC_day((uint8)time.date.day);
		setRTC_month((uint8)time.date.month);
		setRTC_year((uint8)time.date.year);
		/**Save the info into Clock*/
		Clock = time;
		time.modifyDate = FALSE;
	}
}

Time_Type getTime(void)
{
	Time_Type realTime;
	/**Separate the digits of hour*/
	uint32_t decHour;
	uint32_t unHour;
	uint32_t decMin;
	uint32_t unMin;
	uint32_t decSec;
	uint32_t unSec;
	/**Separate the digits of date*/
	uint32_t decYear;
	uint32_t unYear;
	uint32_t decMonth;
	uint32_t unMonth;
	uint32_t decDay;
	uint32_t unDay;
	/**Converts from BCD*/
	decHour = BCDHDec(readRTC_hour());
	unHour = BCDUni(readRTC_hour());
	decMin = BCDDec(readRTC_min());
	unMin = BCDUni(readRTC_min());
	decSec = BCDDec(readRTC_sec());
	unSec = BCDUni(readRTC_sec());
	/**Converts from BCD*/
	decYear = BCDYearDec(readRTC_year());
	unYear = BCDUni(readRTC_year());
	decMonth = BCDMonthDec(readRTC_month());
	unMonth = BCDUni(readRTC_month());
	decDay = BCDDayDec(readRTC_day());
	unDay = BCDUni(readRTC_day());

	/**The second digit is multiplied by 10*/
	decHour *= 10;
	decMin *= 10;
	decSec *= 10;
	decYear *= 10;
	decMonth *= 10;
	decDay *= 10;
	/**The pair of digits are summed and saved into realTime*/
	realTime.hour.hour = decHour + unHour;
	realTime.hour.minutes = decMin + unMin;
	realTime.hour.seconds = decSec + unSec;
	realTime.hour.format = Clock.hour.format;
	realTime.hour.period = Clock.hour.period;

	realTime.date.year = decYear + unYear;
	realTime.date.month = decMonth + unMonth;
	realTime.date.day = decDay + unDay;

	return (realTime);
}

void printTimeLCD(Time_Type time)
{
	uint32_t tmpHour;
	/******************HOUR****************************/
	if(FORMAT_24H == Clock.hour.format)
	{
		/*Print the hour converted from BCD**/
		LCDNokia_gotoXY(15,2);
		delay(6500);
		LCDNokia_printValue(BCDHDec(readRTC_hour()));
		LCDNokia_printValue(BCDUni(readRTC_hour()));
		LCDNokia_sendChar(ASCII_DOUBLEPOINT);

		LCDNokia_gotoXY(35,2);
		LCDNokia_printValue(BCDDec(readRTC_min()));
		LCDNokia_printValue(BCDUni(readRTC_min()));
		LCDNokia_sendChar(ASCII_DOUBLEPOINT);

		LCDNokia_gotoXY(55,2);
		LCDNokia_printValue(BCDDec(readRTC_sec()));
		LCDNokia_printValue(BCDUni(readRTC_sec()));
		/**Clear the space of AM PM*/
		LCDNokia_gotoXY(35,3);
		LCDNokia_sendChar(' ');
		LCDNokia_sendChar(' ');

	}
	if(FORMAT_12H == Clock.hour.format)
	{
		/**If the value is more than 12 then is rested 12*/
		if(readRTC_hour() > (Medium_Hour))
		{

			tmpHour = readRTC_hour();
			tmpHour -= (Medium_Hour);

			LCDNokia_gotoXY(15,2);
			LCDNokia_printValue(BCDHDec(tmpHour));
			LCDNokia_printValue(BCDUni(tmpHour));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(35,2);
			LCDNokia_printValue(BCDDec(readRTC_min()));
			LCDNokia_printValue(BCDUni(readRTC_min()));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(55,2);
			LCDNokia_printValue(BCDDec(readRTC_sec()));
			LCDNokia_printValue(BCDUni(readRTC_sec()));

			LCDNokia_gotoXY(35,3);
			LCDNokia_sendChar('P');
			LCDNokia_sendChar('M');
		}
		/**If the value is 12 then put PM*/
		if(readRTC_hour() == (Medium_Hour))
		{
			LCDNokia_gotoXY(15,2);
			LCDNokia_printValue(1);
			LCDNokia_printValue(2);
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(35,2);
			LCDNokia_printValue(BCDDec(readRTC_min()));
			LCDNokia_printValue(BCDUni(readRTC_min()));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(55,2);
			LCDNokia_printValue(BCDDec(readRTC_sec()));
			LCDNokia_printValue(BCDUni(readRTC_sec()));

			LCDNokia_gotoXY(35,3);
			LCDNokia_sendChar('P');
			LCDNokia_sendChar('M');
		}
		/**If the value of RTC is less than 12 then put AM*/
		if(readRTC_hour() < (Medium_Hour))
		{
			LCDNokia_gotoXY(15,2);
			LCDNokia_printValue(BCDHDec(readRTC_hour()));
			LCDNokia_printValue(BCDUni(readRTC_hour()));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(35,2);
			LCDNokia_printValue(BCDDec(readRTC_min()));
			LCDNokia_printValue(BCDUni(readRTC_min()));
			LCDNokia_sendChar(ASCII_DOUBLEPOINT);

			LCDNokia_gotoXY(55,2);
			LCDNokia_printValue(BCDDec(readRTC_sec()));
			LCDNokia_printValue(BCDUni(readRTC_sec()));

			LCDNokia_gotoXY(35,3);
			LCDNokia_sendChar('A');
			LCDNokia_sendChar('M');
		}
	}

	/*******************DATE***************************/
	LCDNokia_gotoXY(15,4);
	LCDNokia_printValue(BCDDayDec(readRTC_day()));
	LCDNokia_printValue(BCDUni(readRTC_day()));
	LCDNokia_sendChar(ASCII_DIAG);

	LCDNokia_gotoXY(35,4);
	LCDNokia_printValue(BCDMonthDec(readRTC_month()));
	LCDNokia_printValue(BCDUni(readRTC_month()));
	LCDNokia_sendChar(ASCII_DIAG);

	LCDNokia_gotoXY(55,4);
	LCDNokia_printValue(BCDYearDec(readRTC_year()));
	LCDNokia_printValue(BCDUni(readRTC_year()));
}

void printHourUART(Time_Type time)
{
	uint32_t partDecHour;
	uint32_t partUnHour;
	uint32_t partDecMinutes;
	uint32_t partUnMinutes;
	uint32_t partDecSeconds;
	uint32_t partUnSeconds;
	static uint8_t date[6];

	/**To print the value is separated in two digits*/
	partDecHour = time.hour.hour / 10;
	partUnHour = time.hour.hour % 10;
	/**The first digit is converted and saved*/
	if(0 == partDecHour)
	{
		date[0] = '0';
	}
	else
	{
		date[0] = '0' + partDecHour;
	}
	/**The second digit is converted and saved*/
	date[1] = '0' + partUnHour;

	partDecMinutes = time.hour.minutes / 10;
	partUnMinutes = time.hour.minutes % 10;
	/**The first digit is converted and saved*/
	if(0 == partDecMinutes)
	{
		date[2] = '0';
	}
	else
	{
		date[2] = '0' + partDecMinutes;
	}
	/**The second digit is converted and saved*/
	date[3] = '0' + partUnMinutes;

	partDecSeconds = time.hour.seconds / 10;
	partUnSeconds = time.hour.seconds % 10;
	/**The first digit is converted and saved*/
	if(0 == partDecSeconds)
	{
		date[4] = '0';
	}
	else
	{
		date[4] = '0' + partDecSeconds;
	}
	/**The second digit is converted and saved*/
	date[5] = '0' + partUnSeconds;

	/**Verified if the data is different that zero*/
	if('0' == date[0])
	{
		UART_putChar(UART_0, ASCII_0);
	}
	else
	{
		UART_putChar(UART_0, date[0]);
	}

	/**Verified if the data is different that zero*/
	if(0 == date[1])
	{
		UART_putChar(UART_0, ASCII_0);
	}
	else
	{
		UART_putChar(UART_0, date[1]);
	}
	UART_putChar(UART_0, ASCII_DOUBLEPOINT);

	/**Verified if the data is different that zero*/
	if(0 == date[2])
	{
		UART_putChar(UART_0, ASCII_0);
	}
	else
	{
		UART_putChar(UART_0, date[2]);
	}

	/**Verified if the data is different that zero*/
	if(0 == date[3])
	{
		UART_putChar(UART_0, ASCII_0);
	}
	else
	{
		UART_putChar(UART_0, date[3]);
	}
	UART_putChar(UART_0, ASCII_DOUBLEPOINT);

	/**Verified if the data is different that zero*/
	if(0 == date[4])
	{
		UART_putChar(UART_0, ASCII_0);
	}
	else
	{
		UART_putChar(UART_0, date[4]);
	}

	/**Verified if the data is different that zero*/
	if(0 == date[5])
	{
		UART_putChar(UART_0, ASCII_0);
	}
	else
	{
		UART_putChar(UART_0, date[5]);
	}

	UART_putString(UART_0,"\033[10;36H");
}

void printDateUART(Time_Type time)
{
	uint32_t partDecDay;
	uint32_t partUnDay;
	uint32_t partDecMonth;
	uint32_t partUnMonth;
	uint32_t partDecYear;
	uint32_t partUnYear;
	uint8_t date[6];

	/**To print the value is separated in two digits*/
	partDecDay = time.date.day / 10;
	partUnDay = time.date.day % 10;
	/**The pair of digits is converted and saved*/
	date[0] = '0' + partDecDay;
	date[1] = '0' + partUnDay;

	partDecMonth = time.date.month / 10;
	partUnMonth = time.date.month % 10;
	/**The pair of digits is converted and saved*/
	date[2] = '0' + partDecMonth;
	date[3] = '0' + partUnMonth;

	if(time.date.year > 1000)
	{
		partDecYear = (time.date.year - 2000)/ 10;
	}

	if(time.date.year < 1000)
	{
		partDecYear = (time.date.year / 10);
	}

	partUnYear = time.date.year % 10;
	/**The pair of digits is converted and saved*/
	date[4] = '0' + partDecYear;
	date[5] = '0' + partUnYear;

	/**Print the date*/
	UART_putChar(UART_0, date[0]);
	UART_putChar(UART_0, date[1]);
	UART_putChar(UART_0, ASCII_DIAG);
	UART_putChar(UART_0, date[2]);
	UART_putChar(UART_0, date[3]);
	UART_putChar(UART_0, ASCII_DIAG);
	UART_putChar(UART_0, date[4]);
	UART_putChar(UART_0, date[5]);
}


