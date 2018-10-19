/*!
    @file attiny13a_core.c
    @brief Source file for attiny13a core 
    @author Stuart Ianna
    @version 0.1
    @date October 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
    - avr-gcc version 4.9.2
*/

#include "attiny13a_core.h"

void delayms(uint16_t delay){

    //Delay is initially multiplied by two so an uint8_t can
    //be used in the delay loop
    delay = (delay<<2)>>CLOCK_getSpeedAlternate();

    while(delay-- > 0){

        for(volatile uint8_t j = 0; j < 220;j++);
    }
}
