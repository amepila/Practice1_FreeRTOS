/*
 * BCDdecoder.h
 *
 *  Created on: Apr 5, 2018
 *      Author: eric_
 */

#ifndef BCDDECODER_H_
#define BCDDECODER_H_

/*\brief Convierte la parte alta de la representacion BCD de un byte a entero
 * \return parte alta de la representacion BCD a entero
 * */
uint8_t BCDtens(uint8_t BCD);
/*\brief Convierte la parte baja de la representacion BCD de un byte a entero
 * \return parte baja de la representacion BCD a entero
 * */
uint8_t BCDunits(uint8_t BCD);
/*\brief Convierte la parte alta de la representacion BCD de un byte a entero adaptado para la direccion de memoria de horas del RTC
 * \return parte alta de la representacion BCD a entero
 * */
uint8_t BCDHDec(uint8_t BCD);
#endif /* BCDDECODER_H_ */
