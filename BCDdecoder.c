#include "I2C.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include  <stdlib.h>
#include "BCDdecoder.h"

uint8_t BCDtens(uint8_t BCD){
	BCD=BCD>>4;
	BCD&=~(0xF8);
	return BCD;
}
uint8_t BCDunits(uint8_t BCD){
	BCD&=~(0xF0);
	return BCD;
}

uint8_t BCDHDec(uint8_t BCD){
	BCD=BCD>>4;
	BCD&=~(0xFC);
	return BCD;
}
