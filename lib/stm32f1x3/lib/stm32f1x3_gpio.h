#ifndef STM32F103C8_GPIO_H_
#define STM32F103C8_GPIO_H_

/**
    @addtogroup STM32F103
* @{
*/

#include <stdio.h>
#include "stm32f1x3_debug.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

/**
    @addtogroup GPIO
    @brief This module contains low level functions GPIO access.
    @details This header file is included in other modules. It acts as a bridge between peripheral 
    modules such as USART and the cmsis libraries they use. 

    @author Stuart Ianna
    @version 1.0
    @date May 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo
        - Interrupt priorities

    @details

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
    

    @par Minimum working example (blinky)
    @include example_gpio_min.c

    @par Minimum working example (Pin input)
    @include example_gpio_input.c

    @par Minimum working example (interrupt)
    @include example_gpio_int.c

 * @{
 */

/*! @file stm32f1x3_gpio.h
    @brief Header file for stm32f103 GPIO 
*/

/*! 
    @brief GPIO setup options
    @details These values are used when setting up the pin for use. Hexidecimal values used
    are memory address offsets to access the port.
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
    @details These values are used to access the hardware pins for each port. Each hexidecimal number
    corresponds with the memory address offset used.
*/
typedef enum{
    PORTA = 0x0,    //!<Port A of the MCU
    PORTB = 0x400,  //!<Port B of the MCU
    PORTC = 0x800,  //!<Port C of the MCU
}gpio_port;

/*! 
    @brief GPIO interrupt options for pins 
    @details 
    corresponds with the memory address offset used.
*/
typedef enum{
    GPIO_RISING,    //!<Interrupt occurs on rising edge detection
    GPIO_FALLING,   //!<Interrupt occurs on falling edge detection
    GPIO_BOTH,      //!<Interrupt occurs on rising and falling edge detection
}gpio_isr;

/*! 
    @brief GPIO pins available for each port
    @details These macros are used to access the hardware pins for each port. Each hexidecimal number
    corresponds with the memory address offset used.
*/
typedef enum{

    PIN0 = 0x01,    //!<Pin 0 of the port
    PIN1 = 0x02,    //!<Pin 1 of the port
    PIN2 = 0x04,    //!<Pin 2 of the port
    PIN3 = 0x08,    //!<Pin 3 of the port
    PIN4 = 0x010,   //!<Pin 4 of the port
    PIN5 = 0x020,   //!<Pin 5 of the port
    PIN6 = 0x040,   //!<Pin 6 of the port
    PIN7 = 0x080,   //!<Pin 7 of the port
    PIN8 = 0x0100,  //!<Pin 8 of the port
    PIN9 = 0x200,   //!<Pin 9 of the port
    PIN10 = 0x400,  //!<Pin 10 of the port
    PIN11 = 0x800,  //!<Pin 11 of the port
    PIN12 = 0x1000, //!<Pin 12 of the port
    PIN13 = 0x2000, //!<Pin 13 of the port
    PIN14 = 0x4000, //!<Pin 14 of the port
    PIN15 = 0x8000, //!<Pin 15 of the port
}gpio_pin;

/*! 
    @brief GPIO memory address base and offsets
    @detailsThese macros are used intenally to access GPIO configuration ports
*/
typedef enum{

    GPIO_BASE = 0x40010800, //!<Base memory address for GPIO peripheral
    CRL_OFFSET = 0x00,      //!<Control register low memory offset
    CRH_OFFSET = 0x04,      //!<Control register high memory offset
    IDR_OFFSET = 0x08,      //!<Port input data register
    ODR_OFFSET = 0x0C,      //!<Port output data register
    BSRR_OFFSET = 0x10,     //!<Port bit set/reset register
    BRR_OFFSET = 0x14,      //!<Port bit reset register
    LCKR_OFFSET = 0x18,     //!<Port configuration lock register
}gpio_mem;

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
    @details The MCU can only support one external interrupt per pin number. For example,
    pin 2 cannot have an interrupt on port A and B, #E_GPIO_ISR is return if this is attempted.
    @param port The port of the MCU, defined by #gpio_port
    @param pin The pin of the port, defined by #gpio_pin
    @param trigger The interrupt trigger, define by #gpio_isr
    @param handle Pointer to the function to be called on interrupt event.
    @return none
*/
mcu_error GPIO_ISREnable(gpio_port port, gpio_pin pin, gpio_isr trigger, void (*handle)(void));

/*! 
    @brief Disable interrupts on a gpio port / pin.
    @details This function clears all previous setting of the interrupt.
    @param port The port of the MCU, defined by #gpio_port
    @param pin The pin of the port, defined by #gpio_pin
    @return error code defined by #mcu_error
*/
mcu_error GPIO_ISRDisable(gpio_pin pin);
/**@}*/

#endif
