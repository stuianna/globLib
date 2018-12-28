/**
    @addtogroup COMMON
* @{
*/

/**
    @addtogroup COMS128
    @brief Initialization routines for FIF08, USART and STRING11
    @details This module connects the STRING11 module to FIF8 and the passed USART object.

    This provides serial communication to a terminal with a software buffer.

    Sets up FIFO8 with the following
    - #FIFO8_AUTO -> Empties buffer automatically
    - Output function is linked to .put() of the serial object
    - Fifo buffer is contained statically
    - Fifo size define by #TX_SIZE
    - FIFO8 object held statically

    Sets up STRING11 to output into FIFO8 object

    @author Stuart Ianna
    @version 1.0
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug None
    @todo
        

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Example - Typical usage.
    @include example_coms128.c

 * @{
 */

/*! @file coms128.h
    @brief Header file for COMS128.
*/
#ifndef COM128_H_
#define COM128_H_

#include "fifo8.h"
#include "string11.h"

/*! 
    @brief Size of the memory buffer used by FIFO8.
*/
#define TX_SIZE 128
/*! 
    @brief Macro function for simpler initialization
    @param X The serial USART number
*/
#define COMS(X) USART_setTxISR(X,COMOUT_con128(USART_add_put(X)))

/*! 
    @brief Return function pointer type for COMOUT_con128
*/
typedef void (*ret_point)(void);

/*! 
    @brief Initializes FIFO8 and STRING11
    @details Sets up FIFO8 with the following
    - #FIFO8_AUTO -> Empties buffer automatically
    - Output function is linked to .put() of the serial object
    - Fifo buffer is contained statically
    - Fifo size define by #TX_SIZE
    - FIFO8 object held statically

    Sets up STRING11 to output into FIFO8 object

    @param put Pointer to the USART put() function.
    @return Pointer to the ISR callback function to trigger FIFO8_get()
*/
ret_point COMOUT_con128(void (*put)(uint8_t));

/**@}*/
/**@}*/
#endif

