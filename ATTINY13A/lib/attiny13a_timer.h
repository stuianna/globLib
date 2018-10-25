#ifndef ATTINY13A_TIMERS_H_
#define ATTINY13A_TIMERS_H_

/**
    @addtogroup ATTINY13A 
* @{
*/

/**
    @addtogroup TIMERS
    @brief This module contains low level functions for timer interaction.

    The ATTINY13A has a single 8 bit timer with two output channels with output channels mapped to:

    - Timer 0
        - Channel 1 - PORT B, PIN 0
        - Channel 2 - PORT B, PIN 1

    The low width of the timer means there are frequency and resolution restrictions. See the given
    function descriptions.

    @note Some features of this module are disabled by default to save space in flash ROM. The
    follow should be done to enable these features:
     - To use TIMER_setupCount(), TIMER_setPeriod(), TIMER_setFrequency(), TIMER_enableISR() or 
     TIMER_disableISR() the TIMER_ISR flag needs to be uncommented in the Makefile. 
     - To use TIMER_setupIC() or TIMER_getIC() TIMER_IC flag needs to be uncommented in the Makefile.

    Many of the functions in the module have unused parameters. These parameters are kept in place for
    compatability with other MCUs.

    @warning Timer input capture and external interrupts (GPIO_ISREnable()) cannot both be used in the
    same application. Input caputre relies on external interrupt generation for triggers.

    @author Stuart Ianna
    @version 0.1
    @date October 2018
    @copyright GNU GPLv3
    @bug None
    @todo

    @par Compilers
    - avr-gcc V4.9.2

    @par Working example

    @include example_timer.c

 * @{
 */

/*! @file attiny13a_timer.h
    @brief Header file for attiny13a timers
*/

#include "attiny13a_gpio.h"
#include "attiny13a_debug.h"
#include "attiny13a_clock.h"
#include <stdint.h>

#ifndef IC_PIN
#define IC_PIN PIN4     //!<Pin used for input capture
#endif

/*! 
    @brief Main timers available on the MCU.
    @details 
*/
typedef enum{

    TIMER_0,        //!<Timer 0.
}timer_main;
/*! 
    @brief Timers channels available for each timer.
    @details 
*/
typedef enum{

    CHANNEL_1,      //!<Channel 1.
    CHANNEL_2,      //!<Channel 2.
}timer_channel;

/*! 
    @brief Initialize given timer for standard count with interrupt trigging the function handler on timeout.
    @details The minimum and maximum frequency value that can be represented are dependant on the frequency
    of the main clock as specified:

    |F_CPU          |Minimum    |Maximum |
    |---------------|:---------:|:------:|
    |9.6MHz         |40Hz       |10kHz   |
    |4.8MHz         |20Hz       |5kHz    |
    |1.2MHz         |5Hz        |2.5kHz  |
    |0.6MHz         |3Hz        |1.25kHz |

    @param timerNumber The timer number to setup (not used)
    @param handler The function to call on timeout event.
    @param frequency The frequency of the timer count expiration.
    @warning TIMER_ISR needs to be uncommented in the makefile for this to work
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist.
*/
mcu_error TIMER_setupCount(timer_main timerNumber, uint16_t frequency, void (*handler)(void));
/*! 
    @brief Initialize given timer for input capture. 
    @details The input capture channel is fixed to PORTB, PIN4. This can be changed by adding the compiler flag
    -DIC_PIN=xxx, where xxx is the pin number defined by gpio_pin.
    @warning Input capture and external pin interrupts cannot be used at the same time, as input capture uses external
    interrupts.
    @note TIMER_IC needs to be uncommented in the makefile for this to work

    The signal period and pulse resolution is dependant on the clock frequency of the device, as set in the Makefile.

    |F_CPU          |Period     |Resolution |
    |---------------|:---------:|:---------:|
    |9.6MHz         |6.8ms      |26.6us     |
    |4.8MHz         |13.6ms     |53.3us     |
    |1.2MHz         |13.5ms     |53.3us     |
    |0.6MHz         |27.9ms     |106us      |

    @param timerNumber The timer number to setup (not used)
    @param timerChannel The channel of that timer to use. (not used)
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist.
*/
mcu_error TIMER_setupIC(timer_main timerNumber, timer_channel timerChannel);
/*! 
    @brief Initialize given timer for PWM mode.
    @details The given timer number #timer_main is now locked to this or pulse mode.

    All channels of this timer can only be used for PWM of Pulse width output. Each channel must also share the 
    same frequency. However each channel can be turned on and off

    The frequency of the PWM waveform is determined by the clock frequency as follows:

    |F_CPU          |Frequency  |
    |---------------|:---------:|
    |9.6MHz         |37.5kHz    |
    |4.8MHz         |18.75kHz   |
    |1.2MHz         |4.69kHz    |
    |0.6MHz         |2.3kHz     |

    @param timerNumber The timer number to setup (not used)
    @param timerChannel The channel of that timer to use.
    @param frequency The frequency of the signal. (not used)
    @param duty The initial duty cycle.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
*/
mcu_error TIMER_setupPWM(timer_main timerNumber, timer_channel timerChannel, uint16_t frequency, uint8_t duty);
/*! 
    @brief Initialize given timer for Pulse mode.
    @details The given timer number #timer_main is now locked to this pulse or PWM modes.

    All channels of this timer can only be used for pulse of PWM. Each channel must also share the 
    same frequency. However each channel can be turned on and off

    The signal period and pulse resolution is dependant on the clock frequency of the device, as set in the Makefile.

    |F_CPU          |Period     |Resolution |
    |---------------|:---------:|:---------:|
    |9.6MHz         |6.8ms      |26.6us     |
    |4.8MHz         |13.6ms     |53.3us     |
    |1.2MHz         |13.5ms     |53.3us     |
    |0.6MHz         |27.9ms     |106us      |

    @param timerNumber The timer number to setup (not used)
    @param timerChannel The channel of that timer to use. 
    @param frequency The frequency of the total signal. (not used)
    @param pulse The initial pulse width of the signal.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
*/
mcu_error TIMER_setupPulse(timer_main timerNumber, timer_channel timerChannel, uint16_t frequency, uint16_t pulse);
/*! 
    @brief Set the period for the timer. (in microseconds)
    @details The minimum and maximum periods that can be represented are dependant on the frequency
    of the main clock as specified:

    |F_CPU          |Minimum    |Maximum |
    |---------------|:---------:|:------:|
    |9.6MHz         |100us      |25ms    |
    |4.8MHz         |200us      |50ms    |
    |1.2MHz         |800us      |200ms   |
    |0.6MHz         |1.6ms      |400ms   |


    @param timerNumber The timer number to modify (not used)
    @param period The desired timeout period.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
    E_TIMER_PERIOD if the period is too long
*/
mcu_error TIMER_setPeriod(timer_main timerNumber, uint32_t period);
/*! 
    @brief Set the frequency for the timer. (in hertz)
    @details The minimum and maximum frequency value that can be represented are dependant on the frequency
    of the main clock as specified:

    |F_CPU          |Minimum    |Maximum |
    |---------------|:---------:|:------:|
    |9.6MHz         |40Hz       |10kHz   |
    |4.8MHz         |20Hz       |5kHz    |
    |1.2MHz         |5Hz        |2.5kHz  |
    |0.6MHz         |3Hz        |1.25kHz |

    @param timerNumber The timer number to modify (not used)
    @param frequency The desired timeout frequency.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
    E_TIMER_PERIOD if the period is too long
*/
mcu_error TIMER_setFrequency(timer_main timerNumber, uint16_t frequency);
/*! 
    @brief Set the duty cycle for the waveform (PWM only, 0 - 100%).

    @param timerNumber The timer number to modify (not used)
    @param channel The timer chanel to modify
    @param duty The desired duty cycle.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
*/
mcu_error TIMER_setDuty(timer_main timerNumber, timer_channel channel,  uint8_t duty);
/*! 
    @brief Set the pulse width for the output. (in milliseconds)
    @details The resolution and period of the pulse widths are limited by the clock frequency as follows:

    |F_CPU          |Period     |Resolution |
    |---------------|:---------:|:---------:|
    |9.6MHz         |6.8ms      |26.6us     |
    |4.8MHz         |13.6ms     |53.3us     |
    |1.2MHz         |13.5ms     |53.3us     |
    |0.6MHz         |27.9ms     |106us      |

    @param timerNumber The timer number to modify (not used)
    @param channel The timer channel to modify
    @param pulse The desired pulse width.
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist
    E_TIMER_PULSE if the pulse is too long
*/
mcu_error TIMER_setPulse(timer_main timerNumber, timer_channel channel,  uint16_t pulse);
/*! 
    @brief Get the current value of a running timer
    @details The resolution of the value returned depends of the frequency of timer as setup.

    @param timerNumber The timer number to modify (not used)
    @return The current value of the timer.
*/
uint8_t TIMER_getCount(timer_main timerNumber);
/*! 
    @brief Set the current value of a running timer
    @details 

    @param timerNumber The timer number to modify (not used)
    @param count The timer value
    @return The current value of the timer.
*/
mcu_error TIMER_setCount(timer_main timerNumber, uint8_t count);
/*! 
    @brief Get the last input capture time from a channel of a timer
    @details The timer must be setup with TIMER_setupIC() for this function to work.

    @param timerNumber The timer number to modify (not used)
    @param channel The timer channel to modify

    @note This function returns 0 if called twice when no reading has occured.

    @return The last input capture time in microseconds
*/
uint16_t TIMER_getIC(timer_main timerNumber, timer_channel channel);
/*! 
    @brief Set the ISR target for timeout
    @details 

    This feature can be used for Count, Pulse and PWM modes. Function is called on timeout event recursively until
    TIMER_disableISR() is used.

    @param timerNumber The timer number to modify (not used)
    @param handler The function to call on timeout
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error.
*/
mcu_error TIMER_enableISR(timer_main timerNumber, void(*handler)(void));
/*! 
    @brief Disable the timeout IRQ
    @details 

    This disables any previous use of timeout interrupts.

    @param timerNumber The timer number to modify (not used)
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error.
*/
mcu_error TIMER_disableISR(timer_main timerNumber);
/*! 
    @brief Pause an already running timer
    @details 

    The timer must have been already setup to use this.

    @param timerNumber The timer number to modify (not used)
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error, E_TIMER_PORT if port doesn't exist.
*/
mcu_error TIMER_pause(timer_main timerNumber);
/*! 
    @brief Resume a paused timer
    @details Due to the internal register configuration of the ATTINY13A the timer needs to be restarted with
    one of the setup functions. E.g TIMER_setupCount()

    @warning This should not be used to restart a paused timer for the ATTINY13a.

    @param timerNumber The timer number to modify (not used)
    @return Error number as defined in #mcu_error, E_TIMER_NOERROR if no error.
*/
mcu_error TIMER_resume(timer_main timerNumber);

/**@}*/
/**@}*/
#endif
