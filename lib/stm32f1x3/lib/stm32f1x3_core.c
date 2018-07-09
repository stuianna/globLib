/*!
    @file stm32f1x3_core.c
    @brief Source file for stm32f103 core 
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

#include "stm32f1x3_core.h"

void USB_coms(timer_main timerNumber){

    //From _clock.h
    CLOCK_setSpeed(CLOCK_EX_8_OUT_72);

    //From _usb.h
    USB_setup();

    switch (timerNumber){

        case TIMER_1:

            //From _timer.h and usb.h
            TIMER_setupCount(timerNumber,4000,&USB_update);    //4KHz update
            break;

        case TIMER_2:

            //From _timer.h and usb.h
            nvic_set_priority(NVIC_TIM2_IRQ,0);
            TIMER_setupCount(timerNumber,5000,&USB_update);    //4KHz update
            break;

        case TIMER_3:

            //From _timer.h and usb.h
            TIMER_setupCount(timerNumber,4000,&USB_update);    //4KHz update
            break;

        case TIMER_4:

            //From _timer.h and usb.h
            TIMER_setupCount(timerNumber,4000,&USB_update);    //4KHz update
            break;

        default:

            //Systick is the default
            SYSTICK_setup(250,&USB_update);                    //4KHz update
            break;
    }

    //Setup the output for the string library
    STRING11_setOutput(&USB_put);
}

void delayms(uint32_t delay){

    switch(CLOCK_getSpeed() / 1000000){

        case 8:

            for(volatile uint32_t j = 0; j < delay; j++) {

                for(volatile uint16_t i = 0; i < 548; i++);
            }

            break;

        case 24:

            for(volatile uint32_t j = 0; j < delay; j++) {

                for(volatile uint16_t i = 0; i < 1732; i++);
            }
            break;

        case 48:

            for(volatile uint32_t j = 0; j < delay; j++) {

                for(volatile uint16_t i = 0; i < 2995; i++);
            }
            break;

        case 64:

            for(volatile uint32_t j = 0; j < delay; j++) {

                for(volatile uint16_t i = 0; i < 3375; i++);
            }
            break;

        case 72:

            for(volatile uint32_t j = 0; j < delay; j++) {

                for(volatile uint16_t i = 0; i < 3890; i++);
            }

            break;

    }

}
