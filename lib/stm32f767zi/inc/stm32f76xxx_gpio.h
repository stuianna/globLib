#ifndef LIB_STM32F76XXX_GPIO_H
#define LIB_STM32F76XXX_GPIO_H

/**
  @addtogroup STM32F76XXX
  @{
*/

/*! 
    @addtogroup GPIO
    @brief Module for low level GPIO access.
    @author Stuart Ianna
    @date November 2019
    @warning Partly tested, see repository readme for details. 
    @todo
        - Interrupt priorities
        - GPIO locking

    @par LED blink (output) example.
    @include example_gpio_blink.cpp

    @par GPIO input example.
    @include example_gpio_input.cpp

    @par ISR Rising edge example
    @include example_gpio_isr.cpp
  @{
*/

#include "stm32f76xxx_core.h"

/*! 
    @brief GPIO setup options
    @details These values are used when setting up the pin for use. Hexidecimal values used
    are memory address offsets to access the port.
*/
typedef enum{
    GPIO_DI = 0x00,             //!<Setup the port for digital input
    GPIO_DO = 0x01,             //!<Setup the port for digital output
    GPIO_UART_TX = 0x02,     //!<Setup the port for USART transmit
    GPIO_ADC = 0x03,            //!<Setup the port for ADC
    GPIO_UART_RX = 0x04,     //!<Setup the port for USART receive
   // GPIO_I2C = 0xF,         //!<Setup the port for I2C
   // GPIO_PWM = 0xB,         //!<Setup the port for output PWM
   // GPIO_IC = 0x4,          //!<Setup the port for input capture
   // GPIO_SPI_OUT = 0xB,     //!<Setup the port for output SPI 
   // GPIO_SPI_IN = 0x4,      //!<Setup the port for input SPI
}GPIO_Mode;

/*! 
    @brief GPIO interrupt options for pins 
    @details 
*/
typedef enum{
    GPIO_RISING,            //!<Interrupt occurs on rising edge detection
    GPIO_FALLING,           //!<Interrupt occurs on falling edge detection
    GPIO_BOTH,              //!<Interrupt occurs on rising and falling edge detection
}GPIO_Isr;

/*! 
    @brief GPIO output speed types.
    @details The port should be set to output for this to work.
*/
typedef enum{
    GPIO_LOW,               //!<Set pin to low speed.
    GPIO_MEDIUM,            //!<Set pin to medium speed.
    GPIO_HIGH,              //!<Set pin to high speed.
    GPIO_VERY_HIGH,         //!<Set pin to very high speed.
}GPIO_Speed;

/*! 
    @brief GPIO input pull-up or pull-down selection.
    @details The port should be set to input for this to work.
*/
typedef enum{
    GPIO_NO_PULL,           //!<Set pin to have no pull-up or pull-down.
    GPIO_PULL_UP,           //!<Set pin to have input pull-up enabled.
    GPIO_PULL_DOWN,         //!<Set pin to have input pull-down enabled.
}GPIO_Pupd;

/*! 
    @brief GPIO output type.
    @details The port should be set to output for this to work.
*/
typedef enum{
    GPIO_PUSH_PULL,         //!<Set pin for output push-pull (default).
    GPIO_OPEN_DRAIN,        //!<Set pin for output open-drain.
}GPIO_Output_Type;

/*! 
    @brief Setup a GPIO pin for a given function
    @details This function is also employed by other modules to configure pins and ports.
    @param mode The mode for the pin to operate, defined by #GPIO_Mode
    @param port The port of the MCU, GPIOA, GPIOB etc..
    @param pin The numbered pin of the port.
    @return none.
*/
bool GPIO_pinSetup(GPIO_Mode mode, GPIO_TypeDef *port, uint8_t pin);       
/*! 
    @brief Set a given gpio pin on a port to logic high
    @details Pin must be set for digital output to use this function
    @param port The port of the MCU, GPIOA, GPIOB etc.
    @param pin The numbered pin of the port.
    @return none.
*/
void GPIO_pinHigh(GPIO_TypeDef *port, uint8_t pin);
/*! 
    @brief Set a given gpio pin on a port to logic low
    @details Pin must be set for digital output to use this function
    @param port The port of the MCU, GPIOA, GPIOB etc.
    @param pin The numbered pin of the port.
    @return none.
*/
void GPIO_pinLow(GPIO_TypeDef *port, uint8_t pin);
/*! 
    @brief Set a given gpio pin on a port to a logic state
    @details Pin must be set for digital output to use this function
    @param state The state to set the pin to, true = High, false = low.
    @param port The port of the MCU, GPIOA, GPIOB etc.
    @param pin The numbered pin of the port.
    @return none.
*/
void GPIO_pinWrite(GPIO_TypeDef *port, uint8_t pin, bool state);
/*! 
    @brief Toggle the output of a given pin on a port
    @details Pin must be set for digital output to use this function
    @param port The port of the MCU, GPIOA, GPIOB etc.
    @param pin The numbered pin of the port.
    @return none.
*/
void GPIO_pinToggle(GPIO_TypeDef *port, uint8_t pin);
/*! 
    @brief Read the current digital value of an input pin
    @details Pin must be set for digital input to use this function
    @param port The port of the MCU, GPIOA, GPIOB etc.
    @param pin The numbered pin of the port.
    @return The value of the pin. 1 = Logic high, 0 = logic low.
*/
bool GPIO_pinRead(GPIO_TypeDef *port, uint8_t pin);
/*! 
    @brief Set the speed of a pin in input/output mode.
    @details The defined speeds are set in the product datasheet and dependent
    on Vdd and loading.
    @param port The port of the MCU, GPIOA, GPIOB etc.
    @param pin The numbered pin of the port.
    @param speed Output speed defined by #GPIO_Speed
    @return SUCCESS or ERROR.
*/
bool GPIO_speed(GPIO_TypeDef *port, uint8_t pin, GPIO_Speed speed);
/*! 
    @brief Set the input configuration of the pin, pull-up, pull-down or none.
    @details Default state is no pull-up or pull-down.
    @param port The port of the MCU, GPIOA, GPIOB etc.
    @param pin The numbered pin of the port.
    @param type Input type defined by #GPIO_Pupd.
    @return SUCCESS or ERROR.
*/
bool GPIO_pupd(GPIO_TypeDef *port, uint8_t pin, GPIO_Pupd type);
/*! 
    @brief Set output type for the GPIO.
    @details Default state is push-pull.
    @param port The port of the MCU, GPIOA, GPIOB etc.
    @param pin The numbered pin of the port.
    @param type output type defined by #GPIO_Output_Type.
    @return SUCCESS or ERROR.
*/
bool GPIO_oType(GPIO_TypeDef *port, uint8_t pin, GPIO_Output_Type type);
/*! 
    @brief Set the function called when an ISR occurs on a given pin
    @details Interrupts must have been previously enabled on the pin with function #GPIO_ISREnable
    @param pin The numbered pin of the port.
    @param handler The function to call on interrupt occurance.
    @return none
*/
bool GPIO_setHandler(uint8_t pin, void (*handler)(void));
/*! 
    @brief Setup interrupts on a gpio port / pin.
    @details The MCU can only support one external interrupt per pin number. For example,
    pin 2 cannot have an interrupt on port A and B, false is return if this is attempted.
    @param port The port of the MCU, GPIOA, GPIOB etc.
    @param pin The numbered pin of the port.
    @param trigger The interrupt trigger, define by #GPIO_Isr
    @param handle Pointer to the function to be called on interrupt event.
    @return none
*/
bool GPIO_ISREnable(GPIO_TypeDef *port, uint8_t pin, GPIO_Isr trigger, void (*handle)(void));
/*! 
    @brief Disable interrupts on a gpio port / pin.
    @details This function clears all previous setting of the interrupt. Note
    used is irrelevent, as the interrupt can only occur on a unique pin
    number accross all ports.
    @param pin The numbered pin of the port.
    @return none
*/
bool GPIO_ISRDisable(GPIO_TypeDef* ,uint8_t  pin);

/**@}*/
/**@}*/

#endif
