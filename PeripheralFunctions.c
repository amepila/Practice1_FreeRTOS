/*
 * PeripheralFunctions.c
 *
 *  Created on: Mar 14, 2018
 *      Author: Andres Hernandez
 */

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "fsl_dspi.h"
#include "PeripheralFunctions.h"

FIFO_Type FIFO_UART0;
FIFO_Type FIFO_UART1;

void delay(uint32_t delay){
	volatile uint32_t counter;

	for(counter=delay;counter>0;counter--){
	}
}

uint32_t expBASE10(uint8_t limit){
	/*Use the exponent base 10 to different digits*/
	static uint32_t value;

	if(limit == 0){	value = 1;}
	if(limit == 1){	value = 10;}
	if(limit > 1){	value *= 10;}

	return value;
}

uint32_t Convert_numberASCIItoDATA(uint8_t *string){

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
	while(string[counter1] != CR){
		counter1++;
	}

	/*Second loop to begin in last position of array*/
	for(counter2 = counter1; counter2 != 0; counter2--){
		/*Elevate the number by 10 depending of its position*/
		expValue = expBASE10(counter3);

		/*Converts the ASCII to INT*/
		if(counter3 == 0){
			tmpData2 = string[counter2 - 1];
			tmpData2 -= adjustASCII;
			tmpData1 += tmpData2;
		}
		/*Save the value and sums*/
		if(counter3 > 0){
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

uint8_t Convert_wordASCIItoDATA(uint8_t word){
	uint8_t valueWord = (uint8_t)word;
	return (valueWord);
}

FIFO_Type popFIFO_0(void){

	uint32_t counterSize = 0;
	uint32_t position = 0;
	uint32_t counterClear;
	static uint32_t counterChar;
	FIFO_Type fifo;

	/*Loop to find the size*/
	while(FIFO_UART0.data[counterSize] != '\0'){
		counterSize++;
	}

	/*Second loop to clear the FIFO*/
	for(counterClear = 0; counterClear < 50; counterClear++){
		fifo.data[counterClear] = '\0';
	}
	/*Third loop to save the value into FIFO*/
	for(counterChar = counterSize; counterChar != 0; counterChar--){
		fifo.data[position] = FIFO_UART0.data[position];
		position++;
	}

	fifo.size = counterSize;
	fifo.stateFIFO = NORMAL;
	return (fifo);
}

FIFO_Type popFIFO_1(void){

	uint32_t counterSize = 0;
	uint32_t position = 0;
	uint32_t counterClear;
	static uint32_t counterChar;
	FIFO_Type fifo;

	/*Loop to find the size*/
	while(FIFO_UART1.data[counterSize] != '\0'){
		counterSize++;
	}
	/*Second loop to clear the FIFO*/
	for(counterClear = 0; counterClear < 50; counterClear++){
		fifo.data[counterClear] = '\0';
	}
	/*Third loop to save the data into FIFO*/
	for(counterChar = counterSize; counterChar != 0; counterChar--){
		fifo.data[position] = FIFO_UART1.data[position];
		position++;

	}

	fifo.size = counterSize;
	fifo.stateFIFO = NORMAL;
	return (fifo);
}

FIFO_FlagType pushFIFO_0(uint8_t character){

	static uint32_t counterChar = 0;
	const uint32_t CR = 13;

	/*Get the info into FIFO until find the CR value*/
	if(character != CR){
		FIFO_UART0.data[counterChar] = character;
		counterChar++;
		FIFO_UART0.stateFIFO = NORMAL;
		FIFO_UART0.size = 0;
	}else{
		FIFO_UART0.data[counterChar] = character;
		FIFO_UART0.size = counterChar;
		counterChar = 0;
		FIFO_UART0.stateFIFO = NORMAL;
		if(FIFO_UART0.size >= 50){
			FIFO_UART0.stateFIFO = FULL;
		}
	}
	return (FIFO_UART0.stateFIFO);

}

FIFO_FlagType pushFIFO_1(uint8_t character){

	static uint32_t counterChar = 0;
	const uint32_t CR = 13;

	/*Get the info into FIFO until find the CR value*/
	if(character != CR){
		FIFO_UART1.data[counterChar] = character;
		counterChar++;
		FIFO_UART1.stateFIFO = NORMAL;
	}else{
		FIFO_UART1.data[counterChar] = character;
		FIFO_UART1.size = counterChar;
		counterChar = 0;
		FIFO_UART1.stateFIFO = NORMAL;
		if(FIFO_UART1.size >= 50){
			FIFO_UART1.stateFIFO = FULL;
		}
	}
	return (FIFO_UART1.stateFIFO);
}

FIFO_FlagType clearFIFO_0(void){

	uint32_t counter;

	/*Clear the FIFO*/
	for(counter = 0; counter < 50; counter++){
		FIFO_UART0.data[counter] = '\0';
	}
	FIFO_UART0.size = 0;
	FIFO_UART0.stateFIFO = EMPTY;

	return (FIFO_UART0.stateFIFO);
}

FIFO_FlagType clearFIFO_1(void){

	uint32_t counter;

	/*Clear the FIFO*/
	for(counter = 0; counter < 50; counter++){
		FIFO_UART1.data[counter] = '\0';
	}
	FIFO_UART1.size = 0;
	FIFO_UART1.stateFIFO = EMPTY;

	return (FIFO_UART1.stateFIFO);
}


