/*!
    @file stm32f1x3_clock.c
    @brief Source file for stm32f1x3 clock 
    @author Stuart Ianna
    @version 1.0
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
*/

#include "stm32f1x3_clock.h"

mcu_error CLOCK_setSpeed(clock_option option){

    switch(option){

        case CLOCK_IN_OUT_64:

            rcc_clock_setup_in_hsi_out_64mhz();
            break;

        case CLOCK_IN_OUT_48:

            rcc_clock_setup_in_hsi_out_48mhz();
            break;

        case CLOCK_IN_OUT_24:

            rcc_clock_setup_in_hsi_out_24mhz();
            break;

        case CLOCK_EX_8_OUT_24:

            rcc_clock_setup_in_hse_8mhz_out_24mhz();
            break;

        case CLOCK_EX_8_OUT_72:

            rcc_clock_setup_in_hse_8mhz_out_72mhz();
            break;
        default:

            return E_CLOCK_NOSPEED;
            break;
    }

    return E_CLOCK_NOERROR;

}

uint32_t CLOCK_getSpeed(void){

    return rcc_apb2_frequency;

}

uint32_t CLOCK_getSpeedAlternate(void){

    return rcc_apb1_frequency;

}
