/*
 * Task.h
 *
 *  Created on: Mar 18, 2018
 *      Author: Andres Hernandez
 */

#ifndef TASK_H_
#define TASK_H_

status_t init_UART0(void);

status_t init_UART1(void);

void taskINIT(void *arg);


void taskREADI2C_Address(void *arg);

void taskREADI2C_Lenght(void *arg);

void taskREADI2C_Data(void *arg);

void taskREADI2C_FinalRead(void *arg);


void taskWRITEI2C_AddressWrite(void *arg);

void taskWRITEI2C_DataWrite(void *arg);

void taskWRITEI2C_FinalWrite(void *arg);

void taskWRITEI2C_FinalWriteI2C(void *arg);


void taskSETHOUR_SetTime(void *arg);

void taskSETHOUR_SaveTime(void *arg);

void taskSETHOUR_FinalSetHour(void *arg);


void taskSETDATE_SetCalendar(void *arg);

void taskSETDATE_SaveDate(void *arg);

void taskSETDATE_FinalSetDate(void *arg);


void taskFORMAT_ShowFormat(void *arg);

void taskFORMAT_Change(void *arg);

void taskFORMAT_SaveFormat(void *arg);

void taskFORMAT_FinalFormat(void *arg);


void taskREADHOUR_ReadTime(void *arg);

void taskREADHOUR_FinalRH(void *arg);


void taskREADDATE_ReadCalendar(void *arg);

void taskREADDATE_FinalRD(void *arg);


void taskECO_TransmitECO(void *arg);

void taskECO_FinalEco(void *arg);


void taskMENU_Menu(void *arg);

void taskMENU_Read(void *arg);

void taskMENU_Write(void *arg);

void taskMENU_SetHour(void *arg);

void taskMENU_SetDate(void *arg);

void taskMENU_Format(void *arg);

void taskMENU_ReadHour(void *arg);

void taskMENU_ReadDate(void *arg);

void taskMENU_Terminal2(void *arg);

void taskMENU_Eco(void *arg);


#endif /* TASK_H_ */
