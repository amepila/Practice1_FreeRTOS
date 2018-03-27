/*
 * RTC.h
 *
 *  Created on: Mar 15, 2018
 *      Author: eric_
 */

#ifndef RTC_H_
#define RTC_H_


void I2Cinit();

/*\brief Esta funcion accesa a la direccion de memoria de segundos y regresa el dato
 * \return entero en formato BCD que representa segundos
 * */
uint8_t readRTC_sec();

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
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de año del RTC.
 * \param[year] numero entero para segundos.
 *
 * */
void setRTC_year(uint16_t year);


#endif /* RTC_H_ */
