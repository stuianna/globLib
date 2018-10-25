/*!
    @file attiny13a_clock.c
    @brief Source file for attiny13a clock module
    @author Stuart Ianna
    @version 0.1
    @date October 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
    - acr-gcc version 4.9.2
*/

#include "attiny13a_clock.h"

uint32_t CLOCK_getSpeed(void){

    //F_CPU is part of the avr header and is set in the makefile.
    //It should only be changed if the fuses are changed
    return F_CPU;
}

uint8_t CLOCK_getSpeedAlternate(void){

    return CLOCK_BASE / F_CPU ;
}
