/*
 * Buttons.c
 *
 *  Created on: Mar 8, 2018
 *      Author: Andres Hernandez
 */
#include "Buttons.h"

#define BIT_2	(2)
#define BIT_5	(5)
#define BIT_7	(7)
#define BIT_0	(0)

static void Button_enablePORTC(uint8_t pin)
{
	CLOCK_EnableClock(kCLOCK_PortC);

	port_pin_config_t config_Button =
	{ 		kPORT_PullUp, kPORT_SlowSlewRate,
			kPORT_PassiveFilterDisable, kPORT_OpenDrainDisable,
			kPORT_LowDriveStrength, kPORT_MuxAsGpio,
	        kPORT_UnlockRegister
	};

	PORT_SetPinInterruptConfig(PORTC, pin, kPORT_InterruptRisingEdge);
	PORT_SetPinConfig(PORTC, pin, &config_Button);

	gpio_pin_config_t button_config_gpio =
	{ kGPIO_DigitalInput, 0 };

	GPIO_PinInit(GPIOC, pin, &button_config_gpio);
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

