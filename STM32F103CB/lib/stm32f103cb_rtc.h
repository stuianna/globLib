#ifndef STM32F103CB_RTC_H_
#define STM32F103CB_RTC_H_

/**
    @addtogroup STM32F103CB 
* @{
*/

/**
    @addtogroup RTC
    @brief This module contains functions for working with the real time clock.
    @details 

    The real time clock works with the internal LSI oscillator, this runs at
    approximately 40kHz. This clock functions seperate from the main clock and
    can be used in standby mode to wake the core from sleep state. This is acheived
    by setting the alarm to the desired wakeup time.

    Both the counter and alarm registers are 32 bits wide, for a 1 second tick
    this is approximately 136 years before an overflow event occurs! At 1ms 
    it is 7 weeks. The prescaler is 20 bits wide, giving a meer 3600 years
    as a maximum value.

    The accuracy of the RTC increases with increasing resolution approximated as
        - 1ms tick (minimum) ~0.5ms accuracy
        - 10ms tick  ~0.05ms accuracy
        - 100ms tick  ~0.005ms accuracy
        
    and so on. These values assume RTC_calibrate() was used to determine the
    preload value.

    There are three interrupts which can be generated with the RTC
        - resolution, occuring with the value set (ms) in RTC_calibrate() or RTC_setup()
        - alarm, occuring when the counter value reaches a set value. This is also used to wake the core from standby mode.
        - overflow, occurs when the counter overflows.

    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @bug None
    @todo
    @warning RTC_configure() requires a one-shot usage of systick. This means
    that this function should be called before any other systick usage.

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Working example. 
    @include example_rtc.c

 * @{
 */

/*! @file stm32f103cb_rtc.h
    @brief Header file for stm32f103cb real time clock (RTC)
*/

#include "stm32f103cb_debug.h"
#include "stm32f103cb_systick.h"
#include <libopencm3/stm32/rtc.h>
#include <stdint.h>

/*! 
    @brief Initialize the Real time clock
    @details This must be called before any other feature is used.

    The resolution here is loaded directly to the prescalar register. The
    final period of the clock can be estimated using

        period (ms) = (resolution/4)

    This doesn't produce a very accurate result, and if possible RTC_calibrate
    should be called following this funcition. This automatically sets the
    period to the desired value.

    @param resolution The value written to the prescaler register
    @return Error number as defined in #mcu_error, E_RTC_NOERROR if ok or
    E_RTC_PRESCALER if an error with prescaler value has occured.
*/
mcu_error RTC_setup(uint32_t resolution);
/*! 
    @brief Calibrate the RTC to a given resolution in milliseconds
    @details 
    
    This function uses SYSTICK to calculate the required RTC prescaler. SYSTICK
    must not be used for any other function while this is being called. After 
    complete SYSTICK can be utalised again.

    This function will take as long as the resultion to complete!

    @param resolution The required resolution of the RTC in milliseconds.
    @warning Read description.
    @return Error number as defined in #mcu_error, E_RTC_NOERROR if ok or
    E_RTC_PRESCALER if an error with prescaler value has occured.
*/
mcu_error RTC_calibrate(uint32_t resolution);
/*! 
    @brief Set the raw prescaler value of the RTC
    @details 

    This value determines the period of the count incrementation. Usage of
    RTC_calabrate() superseeds the need for this function.

    @param resolution The value written the the RTC prescaler register.
    @return Error number as defined in #mcu_error, E_RTC_NOERROR if ok or
    E_RTC_PRESCALER if an error with prescaler value has occured.
*/
mcu_error RTC_setPrescaler(uint32_t resolution);
/*! 
    @brief Determine if the alarm interrupt is enabled.
    @details 

    @return 
        - 0 if the alarm is disabled
        - Non-zero if the alarm is enabled
*/
uint8_t RTC_isAlarmEnabled(void);
/*! 
    @brief Determine if the overflow interrupt is enabled.
    @details 

    @return 
        - 0 if the overflow is disabled
        - Non-zero if the overflow is enabled
*/
uint8_t RTC_isOverflowEnabled(void);
/*! 
    @brief Determine if the resolution (second) interrupt is enabled.
    @details 

    @return 
        - 0 if the second is disabled
        - Non-zero if the second is enabled
*/
uint8_t RTC_isResolutionEnabled(void);
/*! 
    @brief Get the current count value.
    @return The current value (Of units respect to resolution)
*/
uint32_t RTC_getCount(void);
/*! 
    @brief Set the current count value.
    @param count The current value to set the count to
*/
void RTC_setCount(uint32_t count);
/*! 
    @brief Enables the resolution ISR.
    @details This interrupt occurs each resoultion time value.
    @param res_isr The function to call on interrupt.
*/
void RTC_enableResolutionISR(void (*res_isr)(void));
/*! 
    @brief Enables the alarm ISR.
    @details This interrupt occurs when the counter value matches the alarm 
    value.
    @param count The value at which the count register triggers the interrupt.
    @param al_isr The function to call on interrupt.
*/
void RTC_enableAlarmISR(uint32_t count, void (*al_isr)(void));
/*! 
    @brief Enables the overflow ISR.
    @details This interrupt occurs when the counter register overflows (never)
    @param ov_isr The function to call on interrupt.
*/
void RTC_enableOverflowISR(void (*ov_isr)(void));
/*! 
    @brief disables the resolution ISR.
*/
void RTC_disableResolutionISR(void);
/*! 
    @brief disables the alarm ISR.
*/
void RTC_disableAlarmISR(void);
/*! 
    @brief disables the overflow ISR.
*/
void RTC_disableOverflowISR(void);
/*! 
    @brief Enable the RTC clock 
    @details This can be used to renable the clock after system has been in
    stop mode.
*/
void RTC_clockEnable(void);

/**@}*/
/**@}*/
#endif
