/*
 * I2C.h
 *
 *  Created on: Mar 26, 2018
 *      Author: eric_
 */

#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>


/*\brief funcion para escribir en memoria recibe una direccion de 16 bits y el dato que se quiere escribir, por medio del protocolo estandar de I2C
 * \param[add] direccion a la que se quiere escribir
 *\param[data] datos a escribir
 * */
void writeMemory(uint16_t add,uint8_t data);
/*\brief funcion para leer la memoria recibe una direccion de 16 bits para obtener el dato por medio del protocolo estandar de I2C
 * \param[add] direccion a la que se quiere leer
 * */
uint8_t readMemory(uint16_t add);
/*\brief Esta funcion accesa a la direccion de memoria de segundos y regresa el dato
 * \return entero en formato BCD que representa segundos
 * */
uint8_t readRTC_sec();

void I2Cinit();



/*\brief Esta funcion accesa a la direccion de memoria de minutos y la regresa el dato
 * \return entero en formato BCD que representa minutos
 * */
uint8_t readRTC_min();

/*\brief Esta funcion accesa a la direccion de memoria de horas del RTC para regresar el valor
 * \return entero en formato BCD que representa horas
 *
 * */
uint8_t readRTC_hour();

/*\brief Esta funcion accesa a la direccion de memoria de dia y regresa el datoa
 * \return entero en formato BCD que representa dia
 * */
uint8_t readRTC_day();
/*\brief Esta funcion accesa a la direccion de memoria de meses y regresa el dato
 * \return entero en formato BCD que representa meses
 * */
uint8_t readRTC_month();
/*\brief Esta funcion accesa a la direccion de memoria de años y regresa el dato
 * \return entero en formato BCD que representa años
 * */
uint8_t readRTC_year();

/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de segundos del RTC.
 * \param[sec] numero entero para segundos.
 *
 * */
void setRTC_sec(uint8_t sec);
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de minutos del RTC.
 * \param[min] numero entero para segundos.
 *
 * */
void setRTC_min(uint8_t min);
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de segundos del RTC.
 * \param[hour] numero entero para segundos.
 *
 * */
void setRTC_hour(uint8_t hour);
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de dias del RTC.
 * \param[day] numero entero para segundos.
 *
 * */
void setRTC_day(uint8_t day);
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de mes del RTC.
 * \param[month] numero entero para segundos.
 *
 * */
void setRTC_month(uint8_t month);

void i2crestart(uint8_t sec,uint8_t min, uint8_t hour, uint8_t day, uint8_t month);

#endif /* I2C_H_ */
