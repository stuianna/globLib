#ifndef STM32F103CB_TIMERS_H_
#define STM32F103CB_TIMERS_H_

/**
    @addtogroup STM32F103CB 
* @{
*/

/**
    @addtogroup TIMERS
    @brief This module contains low level functions for timer interaction.

    The STM32F1x3 has four timers, each with four compare/capture channels.
    
    Each timer can be configured to a different function, however each channel of that timer should
    have the same function. Exceptions are:

    A timer can have PWM and pulse outputs on different channels as long as the frequency is the same.
    The ISR of the output timers, (PWM, pulse) can be targeted to function calls. The ISR will trigger at
    the same frequency as the timer.

    The resolution of the base timer is set as:

    - period <= 65535 -> 1 microsecond resolution
    - period <= 655350 -> 10 microsecond resolution
    - period <= 6553500 -> 100 microsecond resolution

    The period is modified with TIMER_setPeriod()

    Timer output/input channels are mapped to:

    - Timer 1
        - Channel 1 - PORT A, PIN 8
        - Channel 2 - PORT A, PIN 9
        - Channel 3 - PORT A, PIN 10
        - Channel 4 - PORT A, PIN 11

    - Timer 2
        - Channel 1 - PORT A, PIN 0
        - Channel 2 - PORT A, PIN 1
        - Channel 3 - PORT A, PIN 2
        - Channel 4 - PORT A, PIN 3

    - Timer 3
        - Channel 1 - PORT A, PIN 6
        - Channel 2 - PORT A, PIN 7
        - Channel 3 - PORT B, PIN 0
        - Channel 4 - PORT B, PIN 1

    - Timer 4
        - Channel 1 - PORT B, PIN 6
        - Channel 2 - PORT B, PIN 7
        - Channel 3 - PORT B, PIN 8
        - Channel 4 - PORT B, PIN 9

    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Working example

    @include example_timer.c

 * @{
 */

/*! @file stm32f103cb_timer.h
    @brief Header file for stm32f103cb timers
*/

#include "stm32f103cb_gpio.h"
#include "stm32f103cb_debug.h"
#include "stm32f103cb_clock.h"
#include <libopencm3/stm32/timer.h>
#include <stdint.h>

/*! 
    @brief Main timers available on the MCU.
    @details 
*/
typedef enum{

    TIMER_1,        //!<Timer 1.
    TIMER_2,        //!<Timer 2.
    TIMER_3,        //!<Timer 3.
    TIMER_4,        //!<Timer 4.
}timer_main;
/*! 
    @brief Timers channels available for each timer.
    @details 
*/
typedef enum{

    CHANNEL_1,      //!<Channel 1.
    CHANNEL_2,      //!<Channel 2.
    CHANNEL_3,      //!<Channel 3.
    CHANNEL_4,      //!<Channel 4.
}timer_channel;

/*! 
    @brief Initialize given timer for standard count with interrupt on timeout.
    @details 

    This mode triggers an ISR on timeout. This function can be impemented for any timer by
    using TIMER_setISR(), however the frequency will be set to the frequency of the already used timer.

    @param timerNumber The timer number to setup
    @param handler The function to call on timeout event.
    @param frequency The frequency of the timer count expiration.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist.
*/
mcu_error TIMER_setupCount(timer_main timerNumber, uint32_t frequency, void (*handler)(void));
/*! 
    @brief Initialize given timer for input capture. 
    @details 

    This supports input capture up for pulses up to 50ms long.

    This function utalizes the ISR for the timer, this means that the ISR cannnot be target
    to an external function using TIMER_enableISR();

    @param timerNumber The timer number to setup
    @param timerChannel The channel of that timer to use.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist.
*/
mcu_error TIMER_setupIC(timer_main timerNumber, timer_channel timerChannel);
/*! 
    @brief Initialize given timer for PWM mode.
    @details The given timer number #timer_main is now locked to this or pulse mode.

    All channels of this timer can only be used for PWM of Pulse width output. Each channel must also share the 
    same frequency. However each channel can be turned on and off

    @param timerNumber The timer number to setup
    @param timerChannel The channel of that timer to use.
    @param frequency The frequncy of the signal.
    @param duty The initial duty cycle.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
*/
mcu_error TIMER_setupPWM(timer_main timerNumber, timer_channel timerChannel, uint32_t frequency, uint8_t duty);
/*! 
    @brief Initialize given timer for Pulse mode.
    @details The given timer number #timer_main is now locked to this pulse or PWM modes.

    All channels of this timer can only be used for pulse of PWM. Each channel must also share the 
    same frequency. However each channel can be turned on and off

    @param timerNumber The timer number to setup
    @param timerChannel The channel of that timer to use.
    @param frequency The frequency of the total signal.
    @param pulse The initial pulse width of the signal.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
*/
mcu_error TIMER_setupPulse(timer_main timerNumber, timer_channel timerChannel, uint32_t frequency, uint32_t pulse);
/*! 
    @brief Set the period for the timer. (in microseconds)
    @details 

    @param timerNumber The timer number to modify
    @param period The desired timeout period.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
    E_TIMER_PERIOD if the period is too long
*/
mcu_error TIMER_setPeriod(timer_main timerNumber, uint32_t period);
/*! 
    @brief Set the frequency for the timer. (in hertz)
    @details 

    @param timerNumber The timer number to modify
    @param frequency The desired timeout frequency.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
    E_TIMER_PERIOD if the period is too long
*/
mcu_error TIMER_setFrequency(timer_main timerNumber, uint32_t frequency);
/*! 
    @brief Set the duty cycle for the waveform (PWM only, 0 - 100%).
    @details 

    @param timerNumber The timer number to modify
    @param channel The timer chanel to modify
    @param duty The desired duty cycle.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
*/
mcu_error TIMER_setDuty(timer_main timerNumber, timer_channel channel,  uint8_t duty);
/*! 
    @brief Set the pulse width for the output. (in microseconds)
    @details 

    @param timerNumber The timer number to modify
    @param channel The timer chanel to modify
    @param pulse The desired pulse width.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
    E_TIMER_PULSE if the pulse is too long
*/
mcu_error TIMER_setPulse(timer_main timerNumber, timer_channel channel,  uint32_t pulse);
/*! 
    @brief Get the current value of a running timer
    @details 

    This function returns the current value of the timer. The resolution of this value is detailed in module outline.

    @param timerNumber The timer number to modify
    @return The current value of the timer.
*/
uint16_t TIMER_getCount(timer_main timerNumber);
/*! 
    @brief Set the current value of a running timer
    @details 

    The resolution of this value is detailed in module outline.

    @param timerNumber The timer number to modify
    @param count The timer value
    @return The current value of the timer.
*/
mcu_error TIMER_setCount(timer_main timerNumber, uint16_t count);
/*! 
    @brief Get the last input capture time from a channel of a timer
    @details The timer must be setup with TIMER_setupIC() for this function to work.

    This function returns zero if no capture has occured within the last period. (Around 63ms). This value could be increased by
    setting a larger period with TIMER_setPeriod(). This however would scale up the value retuned by 10 or 100, as outlined in 
    the module description.

    @param timerNumber The timer number to modify
    @param channel The timer chanel to modify

    @return The last input capture time in microseconds
*/
uint16_t TIMER_getIC(timer_main timerNumber, timer_channel channel);
/*! 
    @brief Set the ISR target for timeout
    @details 

    This feature can be used for Count, Pulse and PWM modes. Function is called on timeout event recursivly until
    TIMER_disableISR() is used.

    @param timerNumber The timer number to modify
    @param handler The function to call on timeout
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist.
*/
mcu_error TIMER_enableISR(timer_main timerNumber, void(*handler)(void));
/*! 
    @brief Disable the timeout IRQ
    @details 

    This disables any previous use of timeout interrupts.

    @param timerNumber The timer number to modify
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist.
*/
mcu_error TIMER_disableISR(timer_main timerNumber);
/*! 
    @brief Pause an already running timer
    @details 

    The timer must have been already setup to use this.

    @param timerNumber The timer number to modify
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist.
*/
mcu_error TIMER_pause(timer_main timerNumber);
/*! 
    @brief Resume a paused timer
    @details 

    The timer must have been already setup and paused.

    @param timerNumber The timer number to modify
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist.
*/
mcu_error TIMER_resume(timer_main timerNumber);

/**@}*/
/**@}*/
#endif
