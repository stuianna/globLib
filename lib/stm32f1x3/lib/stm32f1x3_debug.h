#ifndef STM32F103C8_DEBUG_H
#define STM32F103C8_DEBUG_H

/**
    @addtogroup STM32F103
* @{
*/

/**
    @addtogroup DEBUG
    @brief This module contains error code definitions for all libraries
    @details Each low level  module has an error handling feature

    @author Stuart Ianna
    @version 1.0
    @date May 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo
        - Formulate a more efficient way of printing errors.


    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Minimum working example
    @include example_debug.c

 * @{
 */

/*! @file stm32f1x3_debug.h
    @brief Header file for stm32f103 DEBUG 
*/

#include <stdint.h>
#include <stdlib.h>
/*! 
    @brief Error enumerators for the Debug peripheral
    @details These values are used to quantify errors which occur during configuration of each module
    When debugging is enabled through MCU_debugEnable(), these determine which string is passed to the error output stream
*/
typedef enum{

    E_GPIO_NOERROR,     //!<ERROR GPIO: No Error
    E_GPIO_ISR,         //!<ERROR GPIO: ISR EXTI line in use
    E_GPIO_PORT,        //!<ERROR GPIO: Port doesn't exist
    E_GPIO_PIN,         //!<ERROR GPIO: Pin doesn't exist
    E_GPIO_TRIGGER,     //!<ERROR GPIO: ISR trigger doesn't exist

    E_MCU_UNDEFINED,    //!<ERROR MCU: Unknown error code.
    E_USART_NOERROR,    //!<Error USART: No Error
    E_USART_NOPORT,     //!<Error USART: Port doesn't exist
    E_USART_NOBAUD,     //!<Error USART: Baud rate not available
    E_USART_NOSTOP,     //!<Error USART: Stop bits don't exist
    E_USART_NODATA,     //!<Error USART: Data frame not available
    E_USART_NOPARITY,   //!<Error USART: Parity option not available
    E_USART_NOINT,      //!<Error USART: Interrupt option doesn't exist

    E_I2C_NOERROR,      //!<ERROR I2C: No Error
    E_I2C_PORT,         //!<ERROR I2C: Port doesn't exist
    E_I2C_WRITE,        //!<ERROR I2C: Timeout during put()
    E_I2C_READ,         //!<ERROR I2C: Timeout during get()
    E_I2C_START,        //!<ERROR I2C: Timeout during start()
    E_I2C_STOP,         //!<ERROR I2C: Timeout during stop()

    E_CLOCK_NOERROR,    //!<Error clock: No error
    E_CLOCK_NOSPEED,    //!<Error clock: Clock speed doesn't exist 

    E_SYSTICK_NOERROR,  //!<Error systick: No error
    E_SYSTICK_TOOLONG,  //!<Error systick: Timeout value is too short


    E_TIMER_NOERROR,    //!<Error timer: No error
    E_TIMER_NOTIMER,    //!<Error timer: Timer doesn't exist
    E_TIMER_NOCHANNEL,  //!<Error timer: Channel doesn't exist
    E_TIMER_PERIOD,     //!<Error timer: Period too long / frequency too high
    E_TIMER_PULSE,      //!<Error timer: Pulse too long

    E_ADC_NOERROR,      //!<Error ADC: No error
    E_ADC_PORT,         //!<Error ADC: Port doesn't exist with ADC
    E_ADC_PIN,          //!<Error ADC: No ADC on pin

    E_IWDG_NOERROR,     //!<Error IWDG: No error
    E_IWDG_PERIOD,      //!<Error IWDG: Period too long

    E_FLASH_NOERROR,    //!<Error FLASH: No error
    E_FLASH_PAGE,       //!<Error FLASH: Page number out of bounds

    E_RTC_NOERROR,      //!<Error RTC: No error
    E_RTC_PRELOAD,      //!<Error RTC: Invalid period specified.

    E_SPI_NOERROR,      //!<Error SPI: No error
    E_SPI_PORT,         //!<Error SPI: Port doesn't exist.
}mcu_error;

/*! 
    @brief Print a given error number as a character stream.
    @details This used the function pointer set with MCU_debugEnable to print the error
    definition as a stream of characters. The message matches the values set in #mcu_error.

    This is called automattically when an error has occured in the module. It can also be used manually
    from the return value of configuration functions (if available)
    @param #mcu_error The error number to display.
    @return none
*/
void MCU_printError(mcu_error);

/*! 
    @brief Enables the debug feature
    @details This sets the function pointer for the output stream utalized by MCU_printError().
    
    This has to be called before any debug feature will work.
    @param #output_handle Funtion pointer to the output stream. 
    @return none
*/
void MCU_debugEnable(void (*output_handle)(uint8_t));

/*! 
    @brief Disables the debug feature
    @details This will set any previous function pointer for the output stream to NULL.
    @param none
    @return none
*/
void MCU_debugDisable(void);

/*! 
    @brief Checks if debug is enabled
    @details This funtion is used internally in  modules before calling MCU_printError().
    @param none
    @return 1 if debug is enable, 0 if debug is not enabled.
*/
uint8_t MCU_debugIsEnabled(void);


/**@}*/
#endif
