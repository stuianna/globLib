#ifndef ATTINY13A_GPIO_H_
#define ATTINY13A_GPIO_H_

/**
    @addtogroup ATTINY13A
* @{
*/

#include <stdlib.h>
#include <stdio.h>
#include "attiny13a_debug.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
    @addtogroup GPIO
    @brief This module contains low level functions GPIO access.
    @details The attiny13a only has a single GPIO port, port B, with pins numbered 0 - 5. Pin 5 is used for the reset (active low). To use this pin as a regular GPIO the correct fuses need to be set in the Makefile. 


    @author Stuart Ianna
    @version 0.1
    @date October 2018
    @copyright GNU GPLv3
    @warning Interrupts also need to be enabled in the Makefile, they are disabled by default to save space.
    @bug None
    @todo

    @details

    @par Verified Compilers
    - avr-gcc 4.9.2
    

    @par Minimum working example (blinky)
    @include example_gpio_min.c

    @par Minimum working example (Pin input)
    @include example_gpio_input.c

    @par Minimum working example (interrupt)
    @include example_gpio_int.c

 * @{
 */

/*! @file attiny13a_gpio.h
    @brief Header file for attiny13a GPIO 
*/

//These values are used as part of AVR libraries, undefine them here
//so they can be redefined for use with this library.
#undef PORTB
#undef PIN0
#undef PIN1
#undef PIN2
#undef PIN3
#undef PIN4
#undef PIN5

/*! 
    @brief GPIO setup options
    @details These values are used when setting up the pin for use. 
*/
typedef enum{
    GPIO_DI = 0x8,          //!<Setup the port for digital input
    GPIO_DO = 0x2,          //!<Setup the port for digital output
    GPIO_UART_TX = 0xB,     //!<Setup the port for USART transmit
    GPIO_UART_RX = 0x8,     //!<Setup the port for USART receive
    GPIO_I2C = 0xF,         //!<Setup the port for I2C
    GPIO_PWM = 0xB,         //!<Setup the port for output PWM
    GPIO_IC = 0x4,          //!<Setup the port for input capture
    GPIO_ADC = 0x4,         //!<Setup the port for ADC
    GPIO_SPI_OUT = 0xB,     //!<Setup the port for output SPI 
    GPIO_SPI_IN = 0x4,      //!<Setup the port for input SPI
}gpio_mode;

/*! 
    @brief GPIO pin states
    @details These values are used when writing a value to a pin with pinWrite().
*/
typedef enum{
    GPIO_LOW,       //!<Set the corresponding pin to logic low
    GPIO_HIGH,       //!<Set the corresponding pin to logic high
}gpio_state;


/*! 
    @brief GPIO ports available for the MCU
    @details There is only one port available on the Attiny13a. This macro is
    used for combatability with other MCU
*/
typedef enum{
    PORTB = 0x18 + __SFR_OFFSET,  //!<Port B of the MCU
}gpio_port;


/*! 
    @brief GPIO interrupt options for pins 
    @details 
    corresponds with the memory address offset used.
    @warning Interrupts need to be enabled in the Makefile before they will work. They are disabled by default to save space.
*/
typedef enum{
    GPIO_RISING,    //!<Interrupt occurs on rising edge detection
    GPIO_FALLING,   //!<Interrupt occurs on falling edge detection
    GPIO_BOTH,      //!<Interrupt occurs on rising and falling edge detection
    GPIO_LEVEL_LOW,      //!<Interrupt occurs on low level 
}gpio_isr;

/*! 
    @brief GPIO pins available for each port
    @details These macros are used to access the hardware pins for each port. Each hexidecimal number
    corresponds with the memory address offset used.
*/
typedef enum{

    PIN0 = 0x00,    //!<Pin 0 of the port
    PIN1 = 0x01,    //!<Pin 1 of the port
    PIN2 = 0x02,    //!<Pin 2 of the port
    PIN3 = 0x03,    //!<Pin 3 of the port
    PIN4 = 0x04,   //!<Pin 4 of the port
    PIN5 = 0x05,   //!<Pin 5 of the port
}gpio_pin;

/*! 
    @brief Setup a GPIO pin for a given function
    @details This function is also employed by other modules to configure pins and ports.
    @param mode The mode for the pin to operate, defined by #gpio_mode
    @param port The port of the MCU, defined by #gpio_port
    @param pin The pin of the port, defined by #gpio_pin
    @return none.
*/
mcu_error pinSetup(gpio_mode mode, gpio_port port, gpio_pin pin);
/*! 
    @brief Set a given gpio pin on a port to logic high
    @details Pin must be set for digital output to use this function
    @param port The port of the MCU, defined by #gpio_port
    @param pin The pin of the port, defined by #gpio_pin
    @return none.
*/
void pinHigh(gpio_port port, gpio_pin pin);
/*! 
    @brief Set a given gpio pin on a port to logic low
    @details Pin must be set for digital output to use this function
    @param port The port of the MCU, defined by #gpio_port
    @param pin The pin of the port, defined by #gpio_pin
    @return none.
*/
void pinLow(gpio_port port, gpio_pin pin);
/*! 
    @brief Set a given gpio pin on a port to a logic state
    @details Pin must be set for digital output to use this function
    @param state The state to set the pin to, defined by #gpio_state
    @param port The port of the MCU, defined by #gpio_port
    @param pin The pin of the port, defined by #gpio_pin
    @return none.
*/
void pinWrite(gpio_port port, gpio_pin pin, gpio_state state);
/*! 
    @brief Toggle the output of a given pin on a port
    @details Pin must be set for digital output to use this function
    @param port The port of the MCU, defined by #gpio_port
    @param pin The pin of the port, defined by #gpio_pin
    @return none.
*/
void pinToggle(gpio_port port, gpio_pin pin);
/*! 
    @brief Read the current digital value of an input pin
    @details Pin must be set for digital input to use this function
    @param port The port of the MCU, defined by #gpio_port
    @param pin The pin of the port, defined by #gpio_pin
    @return The value of the pin. 1 = Logic high, 0 = logic low.
*/
uint8_t pinRead(gpio_port port, gpio_pin pin);

/*! 
    @brief Setup interrupts on a gpio port / pin.
    @details The MCU can only support one external interrupt. This interrupt is called for every pin which has a mask enabled. Good stradegy is to test which pin could meet the intterupt condition in the main program. 
    @param port The port of the MCU, defined by #gpio_port
    @param pin The pin of the port, defined by #gpio_pin
    @param trigger The interrupt trigger, define by #gpio_isr
    @param handle Pointer to the function to be called on interrupt event.
    @warning Interrupts need to be enabled in the Makefile before they will work. They are disabled by default to save space.
    @return none
*/
mcu_error GPIO_ISREnable(gpio_port port, gpio_pin pin, gpio_isr trigger, void (*handle)(void));

/*! 
    @brief Disable interrupts on a gpio port / pin.
    @details This function clears all previous setting of the interrupt.
    @param pin The pin of the port, defined by #gpio_pin
    @return none
*/
mcu_error GPIO_ISRDisable(gpio_pin pin);
/**@}*/
/**@}*/

#endif
