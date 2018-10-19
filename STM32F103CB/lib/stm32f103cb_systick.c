/*!**************************************************************************
    @file stm32f103cb_systick.c
    @brief Source file for stm32f103cb systick
    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
******************************************************************************/

#include "stm32f103cb_systick.h"

//The systick handler function pointer
void (*systick_handler)(void);

//Setup systick counter and set handler
mcu_error SYSTICK_setup(uint32_t timeout, void (*handler)(void)){

    mcu_error error;
    nvic_set_priority(NVIC_SYSTICK_IRQ,16);
    error =  SYSTICK_timeout(timeout);
    SYSTICK_handler(handler);
    return error;
}

//Setup the timout
mcu_error SYSTICK_timeout(uint32_t timeout){

    uint32_t reload;
    mcu_error error = E_SYSTICK_NOERROR;

    //Disable systick before changing values
    systick_counter_disable();
    systick_clear();

    if((CLOCK_getSpeed() < 24000000) && (timeout < 1000)){

        systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
        systick_set_reload((uint32_t)(timeout * (float)((float)(CLOCK_getSpeed())/1000000.0f)));

    }
    else{

        systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
        reload = (uint32_t)(timeout * (float)((float)(CLOCK_getSpeed()>>3)/1000000.0f));

        //Check if reload value is greater than max value (24-bit)
        if(reload > 0xFFFFFF){

            systick_set_reload(0xFFFFFF);
            error = E_SYSTICK_TOOLONG;

            //Print error if needed
            if(MCU_debugIsEnabled()){

                MCU_printError(E_SYSTICK_TOOLONG);
            }

        }
        else{

            systick_set_reload(reload);
        }
    }

    //Enable systick again
    systick_counter_enable();

    return error;
}

void SYSTICK_handler(void (*handler)(void)){

    //Turn interrupts off first
    systick_interrupt_disable();

    //Set the handler
    systick_handler = handler;

    //Enable interrupts again
    systick_interrupt_enable();
}

//Return current clock value
uint32_t SYSTICK_count(void){

    return systick_get_value();
}

void sys_tick_handler(void){

    systick_handler();

}

void SYSTICK_start(void){

    systick_counter_enable();

}

void SYSTICK_stop(void){

    systick_counter_disable();
}
