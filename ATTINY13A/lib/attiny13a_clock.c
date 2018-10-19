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


mcu_error CLOCK_setSpeed(clock_option option){

    switch(option){

        case CLOCK_IN_9_6M:

            CLKPR = 0x80;
            CLKPR = 0x00; //No Division
            break;

        case CLOCK_IN_4_8M:

            CLKPR = 0x80;
            CLKPR = 0x01; //Division by 2.
            break;

        case CLOCK_IN_2_4M:

            CLKPR = 0x80;
            CLKPR = 0x02; //Division by 4.
            break;

        case CLOCK_IN_1_2M:

            CLKPR = 0x80;
            CLKPR = 0x03; //Division by 8.
            break;

        case CLOCK_IN_600K:

            CLKPR = 0x80;
            CLKPR = 0x04; //Division by 16.
            break;

        default:

            return E_CLOCK_NOSPEED;
            break;
    }

    return E_CLOCK_NOERROR;

}

uint32_t CLOCK_getSpeed(void){

    //F_CPU is part of the avr header and is set in the makefile.
    //It should only be changed if the fuses are changed
    return F_CPU >> CLKPR;
}

uint8_t CLOCK_getSpeedAlternate(void){

    return CLKPR;
}
