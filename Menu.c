/*
 * Menu.c
 *
 *  Created on: Mar 8, 2018
 *      Author: Andres Hernandez
 */

#include "Menu.h"

const uint8_t clearScreen[10] = "\033[2J";
const uint8_t line10[15] = "\033[10;10H";
const uint8_t line11[15] = "\033[11;10H";
const uint8_t line12[15] = "\033[12;10H";
const uint8_t line13[15] = "\033[13;10H";
const uint8_t line14[15] = "\033[14;10H";
const uint8_t line15[15] = "\033[15;10H";
const uint8_t line16[15] = "\033[16;10H";
const uint8_t line17[15] = "\033[17;10H";
const uint8_t line18[15] = "\033[18;10H";
const uint8_t line19[15] = "\033[19;10H";
const uint8_t line20[15] = "\033[20;10H";

uint8_t menu_Main0(void)
{
	const uint8_t string1[35] = "MENU PRINCIPAL\r";
	const uint8_t string2[35] = "1) Leer memoria I2C\r";
	const uint8_t string3[35] = "2) Escribir memoria I2C\r";
	const uint8_t string4[35] = "3) Establecer hora\r";
	const uint8_t string5[35] = "4) Establecer fecha\r";
	const uint8_t string6[35] = "5) Formato de hora\r";
	const uint8_t string7[35] = "6) Leer hora\r";
	const uint8_t string8[35] = "7) Leer fecha\r";
	const uint8_t string9[35] = "8) Comunicacion con terminal 2\r";
	const uint8_t string10[35] = "9) Eco en LCD\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	/*VT100 command for clearing the screen*/
	UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));

	/** VT100 command for positioning the cursor in x and y position*/

    UART_WriteBlocking(UART0, line10, sizeof(line10));
    UART_WriteBlocking(UART0, string1, sizeof(string1));

    UART_WriteBlocking(UART0, line11, sizeof(line11));
    UART_WriteBlocking(UART0, string2, sizeof(string2));

    UART_WriteBlocking(UART0, line12, sizeof(line12));
    UART_WriteBlocking(UART0, string3, sizeof(string3));

    UART_WriteBlocking(UART0, line13, sizeof(line13));
    UART_WriteBlocking(UART0, string4, sizeof(string4));

    UART_WriteBlocking(UART0, line14, sizeof(line14));
    UART_WriteBlocking(UART0, string5, sizeof(string5));

    UART_WriteBlocking(UART0, line15, sizeof(line15));
    UART_WriteBlocking(UART0, string6, sizeof(string6));

    UART_WriteBlocking(UART0, line16, sizeof(line16));
    UART_WriteBlocking(UART0, string7, sizeof(string7));

    UART_WriteBlocking(UART0, line17, sizeof(line17));
    UART_WriteBlocking(UART0, string8, sizeof(string8));

    UART_WriteBlocking(UART0, line18, sizeof(line18));
    UART_WriteBlocking(UART0, string9, sizeof(string9));

    UART_WriteBlocking(UART0, line19, sizeof(line19));
    UART_WriteBlocking(UART0, string10, sizeof(string10));

	/** VT100 command for positioning the cursor in x and y position*/
    UART_WriteBlocking(UART0, line20, sizeof(line20));

	return (1);
}

uint8_t menu_Main1(void)
{
	const uint8_t string1[35] = "MENU PRINCIPAL\r";
	const uint8_t string2[35] = "1) Leer memoria I2C\r";
	const uint8_t string3[35] = "2) Escribir memoria I2C\r";
	const uint8_t string4[35] = "3) Establecer hora\r";
	const uint8_t string5[35] = "4) Establecer fecha\r";
	const uint8_t string6[35] = "5) Formato de hora\r";
	const uint8_t string7[35] = "6) Leer hora\r";
	const uint8_t string8[35] = "7) Leer fecha\r";
	const uint8_t string9[35] = "8) Comunicacion con terminal 2\r";
	const uint8_t string10[35] = "9) Eco en LCD\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	/*VT100 command for clearing the screen*/
	UART_WriteBlocking(UART1, clearScreen, sizeof(clearScreen));

	/** VT100 command for positioning the cursor in x and y position*/

    UART_WriteBlocking(UART1, line10, sizeof(line10));
    UART_WriteBlocking(UART1, string1, sizeof(string1));

    UART_WriteBlocking(UART1, line11, sizeof(line11));
    UART_WriteBlocking(UART1, string2, sizeof(string2));

    UART_WriteBlocking(UART1, line12, sizeof(line12));
    UART_WriteBlocking(UART1, string3, sizeof(string3));

    UART_WriteBlocking(UART1, line13, sizeof(line13));
    UART_WriteBlocking(UART1, string4, sizeof(string4));

    UART_WriteBlocking(UART1, line14, sizeof(line14));
    UART_WriteBlocking(UART1, string5, sizeof(string5));

    UART_WriteBlocking(UART1, line15, sizeof(line15));
    UART_WriteBlocking(UART1, string6, sizeof(string6));

    UART_WriteBlocking(UART1, line16, sizeof(line16));
    UART_WriteBlocking(UART1, string7, sizeof(string7));

    UART_WriteBlocking(UART1, line17, sizeof(line17));
    UART_WriteBlocking(UART1, string8, sizeof(string8));

    UART_WriteBlocking(UART1, line18, sizeof(line18));
    UART_WriteBlocking(UART1, string9, sizeof(string9));

    UART_WriteBlocking(UART1, line19, sizeof(line19));
    UART_WriteBlocking(UART1, string10, sizeof(string10));

	/** VT100 command for positioning the cursor in x and y position*/
    UART_WriteBlocking(UART1, line20, sizeof(line20));

	return (1);
}

uint8_t menu_ReadI2C0(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Direccion de la lectura:\t";
	const uint8_t string2[35] = "Longitud en bytes: \t";
	const uint8_t string3[35] = "Contenido: \r";
	const uint8_t string4[40] = "Presione una tecla para continuar....\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line10, sizeof(line10));
		    UART_WriteBlocking(UART0, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		flagContinue.flag4 = 0;
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART0, line11, sizeof(line11));
		    UART_WriteBlocking(UART0, string2, sizeof(string2));

			flagContinue.flag2 = 1;
		}
		return 0;
	}

	if(phase == 2)
	{
		if(0 == flagContinue.flag3)
		{
		    UART_WriteBlocking(UART0, line11, sizeof(line11));
		    UART_WriteBlocking(UART0, string3, sizeof(string3));
		    UART_WriteBlocking(UART0, line11, sizeof(line11));

			flagContinue.flag3 = 1;
		}
		return 0;
	}


	if(phase == 3)
	{
		if(0  == flagContinue.flag4)
		{
		    UART_WriteBlocking(UART0, line16, sizeof(line16));
		    UART_WriteBlocking(UART0, string4, sizeof(string4));

			flagContinue.flag4 = 1;
		}
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;
		flagContinue.flag3 = 0;

		return 0;
	}

	return 1;
}

uint8_t menu_ReadI2C1(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Direccion de la lectura:\t";
	const uint8_t string2[35] = "Longitud en bytes: \t";
	const uint8_t string3[35] = "Contenido: \r";
	const uint8_t string4[40] = "Presione una tecla para continuar....\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART1, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line10, sizeof(line10));
		    UART_WriteBlocking(UART1, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		flagContinue.flag4 = 0;
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART1, line11, sizeof(line11));
		    UART_WriteBlocking(UART1, string2, sizeof(string2));

			flagContinue.flag2 = 1;
		}
		return 0;
	}

	if(phase == 2)
	{
		if(0 == flagContinue.flag3)
		{
		    UART_WriteBlocking(UART1, line11, sizeof(line11));
		    UART_WriteBlocking(UART1, string3, sizeof(string3));
		    UART_WriteBlocking(UART1, line11, sizeof(line11));

			flagContinue.flag3 = 1;
		}
		return 0;
	}


	if(phase == 3)
	{
		if(0  == flagContinue.flag4)
		{
		    UART_WriteBlocking(UART1, line16, sizeof(line16));
		    UART_WriteBlocking(UART1, string4, sizeof(string4));

			flagContinue.flag4 = 1;
		}
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;
		flagContinue.flag3 = 0;

		return 0;
	}

	return 1;
}


uint8_t menu_WriteI2C0(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Direccion de la escritura:\t";
	const uint8_t string2[35] = "Texto a guardar: \r";
	const uint8_t string3[35] = "Su texto ha sido guardado...\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line10, sizeof(line10));
		    UART_WriteBlocking(UART0, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART0, line10, sizeof(line10));
		    UART_WriteBlocking(UART0, string2, sizeof(string2));
		    UART_WriteBlocking(UART0, line12, sizeof(line12));

			flagContinue.flag2 = 1;
		}
		return 0;
	}
	if(phase ==  2)
	{
		if(0 == flagContinue.flag3)
		{
		    UART_WriteBlocking(UART0, line13, sizeof(line13));
		    UART_WriteBlocking(UART0, string3, sizeof(string3));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line14, sizeof(line14));

			flagContinue.flag3 = 1;
		}

		return 0;
	}
	if(phase == 3)
	{
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;
		flagContinue.flag3 = 0;
	}
	return 1;
}

uint8_t menu_WriteI2C1(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Direccion de la escritura:\t";
	const uint8_t string2[35] = "Texto a guardar: \r";
	const uint8_t string3[35] = "Su texto ha sido guardado...\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART1, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line10, sizeof(line10));
		    UART_WriteBlocking(UART1, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART1, line10, sizeof(line10));
		    UART_WriteBlocking(UART1, string2, sizeof(string2));
		    UART_WriteBlocking(UART1, line12, sizeof(line12));

			flagContinue.flag2 = 1;
		}
		return 0;
	}
	if(phase ==  2)
	{
		if(0 == flagContinue.flag3)
		{
		    UART_WriteBlocking(UART1, line13, sizeof(line13));
		    UART_WriteBlocking(UART1, string3, sizeof(string3));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line14, sizeof(line14));

			flagContinue.flag3 = 1;
		}

		return 0;
	}
	if(phase == 3)
	{
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;
		flagContinue.flag3 = 0;
	}
	return 1;
}
uint8_t menu_SetHour0(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Escribir hora en hh/mm/ss:\t";
	const uint8_t string2[35] = "La hora ha sido cambiada...: \r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line10, sizeof(line10));
		    UART_WriteBlocking(UART0, string1, sizeof(string1));
		    UART_WriteBlocking(UART0, line11, sizeof(line11));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART0, line12, sizeof(line12));
		    UART_WriteBlocking(UART0, string2, sizeof(string2));

			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line20, sizeof(line20));

			flagContinue.flag2 = 1;
		}
		return 0;
	}

	if(phase == 2)
	{
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;

		return 0;
	}
	return 1;
}
uint8_t menu_SetHour1(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Escribir hora en hh/mm/ss:\t";
	const uint8_t string2[35] = "La hora ha sido cambiada...: \r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART1, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line10, sizeof(line10));
		    UART_WriteBlocking(UART1, string1, sizeof(string1));
		    UART_WriteBlocking(UART1, line11, sizeof(line11));
			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART1, line12, sizeof(line12));
		    UART_WriteBlocking(UART1, string2, sizeof(string2));

			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line20, sizeof(line20));

			flagContinue.flag2 = 1;
		}
		return 0;
	}

	if(phase == 2)
	{
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;

		return 0;
	}
	return 1;
}

uint8_t menu_SetDate0(uint8_t phase)
{
	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Escribir fecha: en dd/mm/aa:\t";
	const uint8_t string2[35] = "La hora ha sido cambiada: \r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line10, sizeof(line10));
		    UART_WriteBlocking(UART0, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART0, line11, sizeof(line11));
		    UART_WriteBlocking(UART0, string2, sizeof(string2));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line20, sizeof(line20));

			flagContinue.flag2 = 1;
		}
		return 0;
	}

	if(phase ==  2)
	{
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;

		return 0;
	}
	return 1;
}
uint8_t menu_SetDate1(uint8_t phase)
{
	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Escribir fecha: en dd/mm/aa:\t";
	const uint8_t string2[35] = "La hora ha sido cambiada: \r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART1, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line10, sizeof(line10));
		    UART_WriteBlocking(UART1, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART1, line11, sizeof(line11));
		    UART_WriteBlocking(UART1, string2, sizeof(string2));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line20, sizeof(line20));

			flagContinue.flag2 = 1;
		}
		return 0;
	}

	if(phase ==  2)
	{
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;

		return 0;
	}
	return 1;
}
uint8_t menu_FormatHour0(uint8_t phase)
{
	static Flags_Type flagContinue;
	const uint8_t string1[35] = "El formato de hora actual es  ";
	const uint8_t string2[35] = "Desea cambiar el formato a  ";
	const uint8_t string3[35] = "si/no?\t";
	const uint8_t string4[40] = "El formato de la hora ha sido cambiado\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			flagContinue.flag4 = 0;
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line10, sizeof(line10));
		    UART_WriteBlocking(UART0, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART0, line11, sizeof(line11));
		    UART_WriteBlocking(UART0, string2, sizeof(string2));

			flagContinue.flag2 = 1;
		}
		return 0;
	}

	if(phase == 2)
	{
		if(0 == flagContinue.flag3)
		{
		    UART_WriteBlocking(UART0, string3, sizeof(string3));

			flagContinue.flag3 = 1;
		}
		return 0;
	}

	if(phase == 3)
	{
		if(0 == flagContinue.flag4)
		{
		    UART_WriteBlocking(UART0, line12, sizeof(line12));
		    UART_WriteBlocking(UART0, string4, sizeof(string4));

			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line20, sizeof(line20));

			flagContinue.flag4 = 1;
		}
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;
		flagContinue.flag3 = 0;

		return 0;
	}
	return 1;
}

uint8_t menu_FormatHour1(uint8_t phase)
{
	static Flags_Type flagContinue;
	const uint8_t string1[35] = "El formato de hora actual es  ";
	const uint8_t string2[35] = "Desea cambiar el formato a  ";
	const uint8_t string3[35] = "si/no?\t";
	const uint8_t string4[40] = "El formato de la hora ha sido cambiado\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			flagContinue.flag4 = 0;
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART1, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line10, sizeof(line10));
		    UART_WriteBlocking(UART1, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART1, line11, sizeof(line11));
		    UART_WriteBlocking(UART1, string2, sizeof(string2));

			flagContinue.flag2 = 1;
		}
		return 0;
	}

	if(phase == 2)
	{
		if(0 == flagContinue.flag3)
		{
		    UART_WriteBlocking(UART1, string3, sizeof(string3));

			flagContinue.flag3 = 1;
		}
		return 0;
	}

	if(phase == 3)
	{
		if(0 == flagContinue.flag4)
		{
		    UART_WriteBlocking(UART1, line12, sizeof(line12));
		    UART_WriteBlocking(UART1, string4, sizeof(string4));

			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line20, sizeof(line20));

			flagContinue.flag4 = 1;
		}
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;
		flagContinue.flag3 = 0;

		return 0;
	}
	return 1;
}
uint8_t menu_ReadHour0(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "La hora actual es: ";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line10, sizeof(line10));
		    UART_WriteBlocking(UART0, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		flagContinue.flag1 = 0;
		return 0;
	}
	return 1;
}
uint8_t menu_ReadHour1(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "La hora actual es: ";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART1, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line10, sizeof(line10));
		    UART_WriteBlocking(UART1, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		flagContinue.flag1 = 0;
		return 0;
	}
	return 1;
}
uint8_t menu_ReadDate0(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "La fecha actual es: ";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line10, sizeof(line10));
		    UART_WriteBlocking(UART0, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		flagContinue.flag1 = 0;
		return 0;
	}
	return 1;
}
uint8_t menu_ReadDate1(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "La fecha actual es: ";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART1, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line10, sizeof(line10));
		    UART_WriteBlocking(UART1, string1, sizeof(string1));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		flagContinue.flag1 = 0;
		return 0;
	}
	return 1;
}

uint8_t menu_CommTerminal2(uint8_t phase)
{
	const uint8_t string1[35] = "Terminal 1: \r";
	const uint8_t string2[35] = "\r";
	const uint8_t string3[35] = "Terminal 2: \r";
	const uint8_t string4[35] = "[ESC]\r";
	const uint8_t string5[35] = "Terminal 2 se desconecto \r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	/*VT100 command for clearing the screen*/
    UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));

	/** VT100 command for positioning the cursor in x and y position*/
    UART_WriteBlocking(UART0, line10, sizeof(line10));
    UART_WriteBlocking(UART0, string1, sizeof(string1));
    UART_WriteBlocking(UART0, string2, sizeof(string2));


    UART_WriteBlocking(UART0, line11, sizeof(line11));
    UART_WriteBlocking(UART0, string3, sizeof(string3));
    UART_WriteBlocking(UART0, string4, sizeof(string4));

    UART_WriteBlocking(UART0, line12, sizeof(line12));
    UART_WriteBlocking(UART0, string5, sizeof(string5));

	/** VT100 command for positioning the cursor in x and y position*/
    UART_WriteBlocking(UART0, line13, sizeof(line13));

	return 1;
}

uint8_t menu_EcoLCD0(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Escribir texto 1: \r";
	const uint8_t string2[35] = "Termino la conexion...\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART0, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART0, line10, sizeof(line10));
		    UART_WriteBlocking(UART0, string1, sizeof(string1));
		    UART_WriteBlocking(UART0, line11, sizeof(line11));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART0, line13, sizeof(line13));
		    UART_WriteBlocking(UART0, string2, sizeof(string2));

			flagContinue.flag2 = 1;
		}
		return 0;
	}
	if(phase == 2)
	{
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;
		return 0;
	}
	return 1;
}

uint8_t menu_EcoLCD1(uint8_t phase)
{

	static Flags_Type flagContinue;
	const uint8_t string1[35] = "Escribir texto 1: \r";
	const uint8_t string2[35] = "Termino la conexion...\r";

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/

	if(phase == 0)
	{
		if(0 == flagContinue.flag1)
		{
			/*VT100 command for clearing the screen*/
		    UART_WriteBlocking(UART1, clearScreen, sizeof(clearScreen));
			/** VT100 command for positioning the cursor in x and y position*/
		    UART_WriteBlocking(UART1, line10, sizeof(line10));
		    UART_WriteBlocking(UART1, string1, sizeof(string1));
		    UART_WriteBlocking(UART1, line11, sizeof(line11));

			flagContinue.flag1 = 1;
		}
		return 0;
	}

	if(phase == 1)
	{
		if(0 == flagContinue.flag2)
		{
		    UART_WriteBlocking(UART1, line13, sizeof(line13));
		    UART_WriteBlocking(UART1, string2, sizeof(string2));

			flagContinue.flag2 = 1;
		}
		return 0;
	}
	if(phase == 2)
	{
		flagContinue.flag1 = 0;
		flagContinue.flag2 = 0;
		return 0;
	}
	return 1;
}
