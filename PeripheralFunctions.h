/*
 * PeripheralFunctions.h
 *
 *  Created on: Mar 14, 2018
 *      Author: Andres Hernandez
 */

#ifndef PERIPHERALFUNCTIONS_H_
#define PERIPHERALFUNCTIONS_H_

/**
 * \brief A mail box type definition for serial port
 */
typedef struct{
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
typedef struct{
	uint8_t data[50];
	FIFO_FlagType stateFIFO;
	uint32_t size;
}FIFO_Type;

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


#endif /* PERIPHERALFUNCTIONS_H_ */
