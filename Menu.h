/*
 * Menu.h
 *
 *  Created on: Mar 8, 2018
 *      Author: Andres Hernandez
 */

#ifndef MENU_H_
#define MENU_H_

#include <stdio.h>
#include "board.h"
#include "MK64F12.h"
#include "fsl_uart.h"
#include "fsl_port.h"

/**
 * \brief This data type define the flags to general use
 */
typedef struct{
	uint8_t flag1 : 1;
	uint8_t flag2 : 1;
	uint8_t flag3 : 1;
	uint8_t flag4 : 1;
	uint8_t flag5 : 1;
}Flags_Type;

status_t init_UART0(void);

status_t init_UART1(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display main menu
 	 \param[in]  void
 	 \return State of phases
 */
uint8_t menu_Main0(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display main menu
 	 \param[in]  void
 	 \return State of phases
 */
uint8_t menu_Main1(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display Read memory menu
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_ReadI2C(uint8_t phase);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display Write memory menu
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_WriteI2C(uint8_t phase);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display Set Hour menu
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_SetHour(uint8_t phase);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display Set date menu
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_SetDate(uint8_t phase);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display format menu
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_FormatHour(uint8_t phase);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display Read hour menu
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_ReadHour(uint8_t phase);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display Read Date menu
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_ReadDate(uint8_t phase);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display Terminal 2 menu
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_CommTerminal2(uint8_t phase);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display Read memory
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_EcoLCD(uint8_t phase);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Display Menu of uart1
 	 \param[in]  phase Value of substate
 	 \return State of phases
 */
uint8_t menu_Main1(void);



#endif /* MENU_H_ */
