#ifndef STM32F103CB_USART_H_
#define STM32F103CB_USART_H_

/**
    @addtogroup STM32F103CB 
* @{
*/

/**
    @addtogroup USART
    @brief This module contains low level functions for USART interaction.
    @details The USART peripherals are mapped to the following pins for the ports

    - USART_1:
             + RX1 - PA10
             + TX1 - PA9
    - USART_2:
             + RX2 - PA3
             + TX2 - PA2
    - USART_3:
             + RX3 - PB11
             + TX3 - PB10

    Any USART data structure created needs to be initialized with USART_setup(),
    this function initializes the given usart peripheral with the following parameters

    - 9600bps baud
    - 1 Stop bit
    - 8 bit data frame
    - No parity


    @author Stuart Ianna
    @version 0.1
    @date May 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo
        - Add function for interrupt priority configuration


    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Minimum working example (blocking)
    @include example_usart_min.c

    @par Minimum working example (interrupts)
    @include example_usart_int.c

    @par Full example
    example_usart.c

 * @{
 */

/*! @file stm32f103cb_usart.h
    @brief Header file for stm32f103cb USART
*/


#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include "stm32f103cb_gpio.h"
#include "stm32f103cb_debug.h"
#include <stdint.h>

/*! 
    @brief Function pointer typedef for void function with uint8_t parameter
*/
typedef void(*v_fp_u8)(uint8_t);

/*! 
    @brief Peripheral ports available on the MCU.
    @details These values are used with function USART_setup()
*/
typedef enum{

    USART_1,        //!<First peripheral port.
    USART_2,        //!<Second peripheral port.
    USART_3,        //!<Third peripheral port.
}usart_periph;

/*! 
    @brief Parity option available for the port.
    @details 
    These values are utilised with function USART_setParity()
*/
typedef enum{

    USART_NONE,  //!<No parity for the port
    USART_ODD,   //!<Odd parity for the port
    USART_EVEN,  //!<Even parity for the port
}usart_parity;

/*! 
    @brief Stop bit options available for the port.
    @details These values are utilised with function USART_setStop()
*/
typedef enum{

    USART_STOP_ONE,     //!<One stop bit for the port.
    USART_STOP_TWO,     //!<Two stop bits for the port.
}usart_stop;

/*! 
    @brief Data frame lengths for the port.
    @details These values are utilised with function USART_setData()
*/
typedef enum{

    USART_DATA_EIGHT,   //!<Eight bit data frame for the port.
    USART_DATA_NINE,    //!<Nine bit data frame for the port.
}usart_data;

/*! 
    @brief Baud rate options for the port.
    @details These value are utilised with function USART_setBaud()
*/
typedef enum{

    USART_BAUD_2400,    //!<2400bps baud rate
    USART_BAUD_4800,    //!<4800bps buad rate
    USART_BAUD_9600,    //!<9600bps baud rate
    USART_BAUD_19200,   //!<19200bps baud rate
    USART_BAUD_38400,   //!<38400bps baud rate
    USART_BAUD_57600,   //!<57600bps baud rate
    USART_BAUD_115200,  //!<115200bps baud rate
}usart_baud;

/*! 
    @brief ISR values for the port.
    @details These values are utilised with function USART_enableISR() and USART_disableISR()
*/
typedef enum{

    USART_TX,           //!<Transmit ISR
    USART_RX,           //!<Recieve ISR
}usart_isr;

//Default option for the usart peripheral, set by USART_setup()
#define USART_DEFAULT_BAUD USART_BAUD_9600      //!<The default baud rate used by setup function
#define USART_DEFAULT_STOP USART_STOP_ONE       //!<The default stop bits used by setup function
#define USART_DEFAULT_DATA USART_DATA_EIGHT     //!<The default data frame used by setup function
#define USART_DEFAULT_PARITY USART_PARITY_NONE  //!<The default parity used by setup function

/*! 
    @brief Initialize USART port.
    @details This must be called before any other feature is used.

	This sets up the given USART peripheral to the default macro defined settings.
    @param peripheral The USART peripheral to modify.
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_setup(usart_periph peripheral);
/*! 
    @brief Set the baud rate for the port.
    @details 
    @param peripheral The USART peripheral to modify.
    @param baud Baud rate defined by #usart_baud to be used.
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_setBaud(usart_periph peripheral, usart_baud baud);
/*! 
    @brief Return the address of the peripheral put function.
    @details 
    @param peripheral The USART peripheral to get.
    @return Error number as defined in #mcu_error.
*/
v_fp_u8 USART_add_put(usart_periph peripheral);
/*! 
    @brief Send a byte on the given peripheral.
    @details 
    @param peripheral The usart number to send on.
    @param byte The byte to send.
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_put(usart_periph peripheral, uint8_t byte);
/*! 
    @brief Get a byte on the given peripheral (blocking).
    @details 
    @param peripheral The usart number to send on.
    @return The byte recieved on the port.
*/
uint8_t USART_get(usart_periph peripheral);
/*! 
    @brief Set the number of stop bits for the port.
    @details 
    @param peripheral The USART peripheral to modify.
    @param stopBits Number of stop bits to use defined in #usart_stop.
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_setStop(usart_periph peripheral, usart_stop stopBits);
/*! 
    @brief Set the parity for the port.
    @details 
    @param peripheral The USART peripheral to modify.
    @param parity Parity option defined in #usart_parity.
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_setParity(usart_periph peripheral, usart_parity parity);
/*! 
    @brief Set the data frame size for the port.
    @details 
    @param peripheral The USART peripheral to modify.
    @param data Data frame size as define in in #usart_data.
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_setData(usart_periph peripheral, usart_data data);
/*! 
    @brief Sets the target function called when recieve ISR is triggered.
    @details The recieve ISR is called when a byte is ready to be read in the receive buffer.

    By default, the ISR flag is cleared first then the function specified by this pointer is called.

    If no function has been specified this pointed to NULL.

    The function specified must be of this type.

    @param peripheral The USART peripheral to modify.
    @param new_handler Address of the function for the ISR to point to.
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_setRxISR (usart_periph peripheral, void (*new_handler)(uint8_t received));
/*! 
    @brief Sets the target function called when transmit ISR is triggered.
    @details The transmit ISR is called when a byte has finished being sent from the buffer.

    By default, the ISR flag is cleared first then the function specified by this pointer is called.

    If no function has been specified this pointed to NULL.

    The function specified must be of this type.
    @param peripheral The USART peripheral to modify.
    @param new_handler Address of the function for the ISR to point to.
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_setTxISR (usart_periph peripheral, void (*new_handler)(void));
/*! 
    @brief Enables the specified ISR function.
    @details 
    @param peripheral The USART peripheral to modify.
    @param isr_type The type of ISR to change, define by #usart_isr
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_enableISR(usart_periph peripheral, usart_isr isr_type);
/*! 
    @brief Disables the specified ISR function.
    @details By default, ISRs are enabled. This can be used to disable them.
    @param peripheral The USART peripheral to modify.
    @param isr_type The type of ISR to change, define by #usart_isr
    @return Error number as defined in #mcu_error.
*/
mcu_error USART_disableISR(usart_periph peripheral, usart_isr isr_type);

/**@}*/
/**@}*/
#endif
