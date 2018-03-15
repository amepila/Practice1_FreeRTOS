/*
 * Buttons.c
 *
 *  Created on: Mar 8, 2018
 *      Author: Andres Hernandez
 */
#include "Buttons.h"


#define TEST 	(0)
#define BIT_2	(2)
#define BIT_5	(5)
#define BIT_7	(7)
#define BIT_0	(0)

/*Flag of buttons*/
static Button_PressedFlag Button_Pressed;

static void Button_enablePORTC(uint8_t pin)
{
	CLOCK_EnableClock(kCLOCK_PortC);

	port_pin_config_t config_Button =
	{ 		kPORT_PullUp, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
	        kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAsGpio,
	        kPORT_UnlockRegister
	};

	PORT_SetPinInterruptConfig(PORTC, pin, kPORT_InterruptRisingEdge);
	PORT_SetPinConfig(PORTC, pin, &config_Button);

	gpio_pin_config_t button_config_gpio =
	{ kGPIO_DigitalInput, 1 };

	GPIO_PinInit(GPIOC, pin, &button_config_gpio);

#if TEST
	/*The configuration of buttons is Pull Up and Rising edge interrupter*/
	static GPIO_pinControlRegisterType pinControlRegisterPort = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_RISING_EDGE;

	/*Selector of port*/
	switch(port){
	case PORT_A:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_A,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_A,GPIO_INPUT,bit);
		break;
	case PORT_B:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_B,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_B,GPIO_INPUT,bit);
		break;
	case PORT_C:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_C,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,bit);
		break;
	case PORT_D:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_D,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_D,GPIO_INPUT,bit);
		break;
	case PORT_E:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_E,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_E,GPIO_INPUT,bit);
		break;
	default:
		break;
	}
#endif
}

void Button_statusFlag(uint8_t bit)
{
	Button_Pressed.flagButton0 = (bit == BIT_2) ? 0 : 1;
	Button_Pressed.flagButton1 = (bit == BIT_5) ? 0 : 1;
	Button_Pressed.flagButton2 = (bit == BIT_7) ? 0 : 1;
	Button_Pressed.flagButton3 = (bit == BIT_0) ? 0 : 1;
}

uint8_t Button_getFlag(Button_Type button){

	/**Selector of buttons**/
	switch(button){
	case BUTTON_0:
		/**Return the status flag Button 0**/
		return (Button_Pressed.flagButton0);
		break;
	case BUTTON_1:
		/**Return the status flag Button 1**/
		return (Button_Pressed.flagButton1);
		break;
	case BUTTON_2:
		/**Return the status flag Button 2**/
		return (Button_Pressed.flagButton2);
		break;
	case BUTTON_3:
		/**Return the status flag Button 3**/
		return (Button_Pressed.flagButton3);
		break;
	case BUTTON_4:
		/**Return the status flag Button 4**/
		return (Button_Pressed.flagButton4);
		break;
	case BUTTON_5:
		/**Return the status flag Button 5**/
		return (Button_Pressed.flagButton5);
		break;
	default:
		return 0;
		break;
	}
}

void Button_clearFlag(Button_Type button){

	/*Selector of buttons*/
	switch(button){
	case BUTTON_0:
		/**Clear the flag of button 0**/
		Button_Pressed.flagButton0 = 0;
		break;
	case BUTTON_1:
		/**Clear the flag of button 1**/
		Button_Pressed.flagButton1 = 0;
		break;
	case BUTTON_2:
		/**Clear the flag of button 2**/
		Button_Pressed.flagButton2 = 0;
		break;
	case BUTTON_3:
		/**Clear the flag of button 3**/
		Button_Pressed.flagButton3 = 0;
		break;
	default:
		break;
	}
}

void Buttons_init(const Button_ConfigType* Button_Config){

	/**Number of buttons**/
	uint8_t counter;

	/**Loop that set up each button**/
	for(counter = 0; counter < 3; counter++){
		/**Set the button like output**/
		Button_enablePORTC(Button_Config[counter].Bit_Type);
	}
}

