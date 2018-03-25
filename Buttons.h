/*
 * Buttons.h
 *
 *  Created on: Mar 8, 2018
 *      Author: Andres Hernandez
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "fsl_port.h"
#include "fsl_gpio.h"

/*Data type to assign the buttons*/
typedef enum{
	BUTTON_0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_5
}Button_Type;

/*Data type to assign the port of buttons to use*/
typedef enum{PORT_A,PORT_B,PORT_C,PORT_D,PORT_E}Button_PortType;

/*Data type to set up the buttons*/
typedef struct{
	Button_PortType Button_Port;
	uint8_t Bit_Type;
}Button_ConfigType;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This set up of the buttons
 	\param[in] Button_Config Configurations of buttons
 	\return void
 */
void Buttons_init(const Button_ConfigType*);



#endif /* BUTTONS_H_ */
