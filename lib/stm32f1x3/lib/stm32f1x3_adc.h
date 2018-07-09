#ifndef STM32F103C8_ADC_H_
#define STM32F103C8_ADC_H_

/**
    @addtogroup STM32F103
* @{
*/

/**
    @addtogroup ADC
    @brief This module contains low level functions for ADC interaction.
    @details 

    The STM32f1x3 has one ADC with 10 channels on pins PA0 - PA7 and PB0 - PB1.
    This module containes basic routines to setup the ADC and obatin a sample
    from each of these channels.

    @author Stuart Ianna
    @version 1.0
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Minimum working example
    @include example_adc.c

 * @{
 */

/*! @file stm32f1x3_adc.h
    @brief Header file for stm32f103 ADC API
*/

#include "stm32f1x3_gpio.h"
#include "stm32f1x3_debug.h"
#include <libopencm3/stm32/adc.h>
#include <stdint.h>

/*! 
    @brief Typical voltage reference value used by ADC_volts()
*/
#define ADC_VOLT_REF 3.3f

/*! 
    @brief Initialize ADC for a given port and pin
    @details This must be called before any other API feature is used.

    @param port The port the pin is located on
    @param pin The pin of the port
    @return Error number as defined in #mcu_error, E_ADC_NOERROR if no error, E_ADC_PORT if port doesn't support
    ADC or E_ADC_PIN if given pin on port doesn't support ADC
*/
mcu_error ADC_setup(gpio_port port, gpio_pin pin);

/*! 
    @brief Add an aditional channel to the ADC
    @details This can be used after ADC_setup() has been called once to add additional ADC channels

    @param port The port the pin is located on
    @param pin The pin of the port
    @return Error number as defined in #mcu_error, E_ADC_NOERROR if no error, E_ADC_PORT if port doesn't support
    ADC or E_ADC_PIN if given pin on port doesn't support ADC
*/
uint16_t ADC_addChannel(gpio_port port, gpio_pin pin);

/*! 
    @brief Get a sample from the ADC
    @details 

    @param port The port the pin is located on
    @param pin The pin of the port
    @return The 12-bit sample from the ADC, returns 0 if given port or pin isn't valid
*/
uint16_t ADC_sample(gpio_port port, gpio_pin pin);
/*! 
    @brief Get a sample from the ADC and convert it to volts
    @details This uses #ADC_VOLT_REF to convert the sample value to volts

    @param port The port the pin is located on
    @param pin The pin of the port
    @return The voltage sample from the ADC, returns 0 if given port or pin isn't valid
*/
float ADC_volts(gpio_port port, gpio_pin pin);
/**@}*/
#endif
