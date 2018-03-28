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
#include "E2PROM.h"
#include "RTC.h"

/**ASCII table such as reference**/

typedef enum{
		ASCII_SPACE = 32,	ASCII_EXCLAMATION,	ASCII_QUOTATION,	ASCII_SHARP,		ASCII_DOLLAR,
		ASCII_PERCENT,		ASCII_AMPERSON,		ASCII_APOS,			ASCII_PAROPEN,		ASCII_PARCLOSE,
		ASCII_AST,			ASCII_PLUS,			ASCII_COMMA,		ASCII_MINUS,		ASCII_POINT,
		ASCII_DIAG,			ASCII_0,			ASCII_1,			ASCII_2,			ASCII_3,
		ASCII_4,			ASCII_5,			ASCII_6,			ASCII_7,			ASCII_8,
		ASCII_9,			ASCII_DOUBLEPOINT,	ASCII_COMMAPOINT,	ASCII_MINOR,		ASCII_EQUAL,
		ASCII_MAYOR,		ASCII_QUESTION,		ASCII_COMMAT,		ASCII_A,			ASCII_B,
		ASCII_C,			ASCII_D,			ASCII_E,			ASCII_F,			ASCII_G,
		ASCII_H,			ASCII_I,			ASCII_J,			ASCII_K,			ASCII_L,
		ASCII_M,			ASCII_N,			ASCII_O,			ASCII_P,			ASCII_Q,
		ASCII_R,			ASCII_S,			ASCII_T,			ASCII_U,			ASCII_V,
		ASCII_W,			ASCII_X,			ASCII_Y,			ASCII_Z,			ASCII_CORCHOPEN,
		ASCII_DIAG2,		ASCII_CORCHCLOSE,	ASCII_HAT,			ASCII_LOWBAR,		ASCII_GRAVE,
		ASCII_a,			ASCII_b,			ASCII_c,			ASCII_d,			ASCII_e,
		ASCII_f,			ASCII_g,			ASCII_h,			ASCII_i,			ASCII_j,
		ASCII_k,			ASCII_l,			ASCII_m,			ASCII_n,			ASCII_o,
		ASCII_p,			ASCII_q,			ASCII_r,			ASCII_s,			ASCII_t,
		ASCII_u,			ASCII_v,			ASCII_w,			ASCII_x,			ASCII_y,
		ASCII_z,			ASCII_BRACEOPEN,	ASCII_VERT,			ASCII_BRACECLOSE,	ASCII_TILDE
}ASCII_code;

/**Structure with the time*/
static Time_Type Clock;
const uint32_t Medium_Hour = (0x12U);

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

void setTimeLCD(Time_Type time)
{
	/**Set the hour*/
	if(1 == time.modifyTime)
	{
		setRTC_sec((uint8_t)time.hour.seconds);
		setRTC_min((uint8_t)time.hour.minutes);
		setRTC_hour((uint8_t)time.hour.hour);
		/**Save the info into Clock*/
		Clock = time;
		time.modifyTime= 0;
	}
	/**Set the date*/
	if(1 == time.modifyDate)
	{
		setRTC_day((uint8_t)time.date.day);
		setRTC_month((uint8_t)time.date.month);
		setRTC_year((uint8_t)time.date.year);
		/**Save the info into Clock*/
		Clock = time;
		time.modifyDate = 0;
	}
}
#if 0
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
#endif
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
	    UART_WriteBlocking(UART0, (uint8_t*)ASCII_0, sizeof(uint8_t));
	}
	else
	{
	    UART_WriteBlocking(UART0, &date[0], sizeof(uint8_t));
	}

	/**Verified if the data is different that zero*/
	if(0 == date[1])
	{
	    UART_WriteBlocking(UART0, (uint8_t*)ASCII_0, sizeof(uint8_t));
	}
	else
	{
	    UART_WriteBlocking(UART0, &date[1], sizeof(uint8_t));
	}
    UART_WriteBlocking(UART0, (uint8_t*)ASCII_DOUBLEPOINT, sizeof(uint8_t));

	/**Verified if the data is different that zero*/
	if(0 == date[2])
	{
	    UART_WriteBlocking(UART0, (uint8_t*)ASCII_0, sizeof(uint8_t));
	}
	else
	{
	    UART_WriteBlocking(UART0, &date[2], sizeof(uint8_t));
	}

	/**Verified if the data is different that zero*/
	if(0 == date[3])
	{
	    UART_WriteBlocking(UART0, (uint8_t*)ASCII_0, sizeof(uint8_t));
	}
	else
	{
	    UART_WriteBlocking(UART0, &date[3], sizeof(uint8_t));
	}
    UART_WriteBlocking(UART0, (uint8_t*)ASCII_DOUBLEPOINT, sizeof(uint8_t));

	/**Verified if the data is different that zero*/
	if(0 == date[4])
	{
	    UART_WriteBlocking(UART0, (uint8_t*)ASCII_0, sizeof(uint8_t));
	}
	else
	{
	    UART_WriteBlocking(UART0, &date[4], sizeof(uint8_t));
	}

	/**Verified if the data is different that zero*/
	if(0 == date[5])
	{
	    UART_WriteBlocking(UART0, (uint8_t*)ASCII_0, sizeof(uint8_t));
	}
	else
	{
	    UART_WriteBlocking(UART0, &date[5], sizeof(uint8_t));
	}

    UART_WriteBlocking(UART0, (uint8_t*)"\033[10;36H", sizeof(uint8_t));
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
    UART_WriteBlocking(UART0, &date[0], sizeof(uint8_t));
    UART_WriteBlocking(UART0, &date[1], sizeof(uint8_t));
    UART_WriteBlocking(UART0, (uint8_t*)ASCII_DIAG, sizeof(uint8_t));
    UART_WriteBlocking(UART0, &date[2], sizeof(uint8_t));
    UART_WriteBlocking(UART0, &date[3], sizeof(uint8_t));
    UART_WriteBlocking(UART0, (uint8_t*)ASCII_DIAG, sizeof(uint8_t));
    UART_WriteBlocking(UART0, &date[4], sizeof(uint8_t));
    UART_WriteBlocking(UART0, &date[5], sizeof(uint8_t));

}


