#ifndef STM32F103C8_SPI_H_
#define STM32F103C8_SPI_H_

/**
    @addtogroup STM32F103
* @{
*/

/**
    @addtogroup SPI
    @brief This module contains low level functions for the SPI bus. 
    @details 


    @author Stuart Ianna
    @version 1.0
    @date July 2018
    @copyright GNU GPLv3
    @warning None
    @bug 
    @todo

    @par Verified Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Example Usage

    @include example_spi.c

 * @{
 */

/*! @file stm32f1x3_spi.h
    @brief Header file for stm32f103 SPI
*/
#include "stm32f1x3_gpio.h"
#include "stm32f1x3_debug.h"
#include "stm32f1x3_clock.h"
#include <libopencm3/stm32/f1/spi.h>
#include <stdint.h>

#define SPI_DEFAULT_SPEED SPI_SPEED_8     //!<Serial clock rate  
#define SPI_DEFAULT_CLOCK SPI_CLOCK_HIGH    //!<Clock reset state 
#define SPI_DEFAULT_PHASE SPI_PHASE_SECOND  //!<Clock capture edge relative to clock
#define SPI_DEFAULT_DATA SPI_DATA_8         //!<Data frame size
#define SPI_DEFAULT_ENDIAN SPI_MSB          //!<Data frame order

/*! 
    @brief Funtion point typedef for void(*fp)(void)
*/
typedef uint16_t (*spi_u16_fp_v)(void);
/*! 
    @brief Funtion point typedef for void(*fp)(uint8_t)
*/
typedef void (*spi_v_fp_u16)(uint16_t);

/*! 
    @brief Peripheral SPI ports available
*/
typedef enum{

    SPI_1,                                  //!<The first SPI peripheral
    SPI_2,                                  //!<The second SPI peripheral
}spi_periph;

/*! 
    @brief Clock rates for the serial clock
    @details These rates control the clock divisor applied between the main clock
    and the serial clock. These rates are tried to met as accurately as possible, 
    but will not be exceeded.
*/
typedef enum{

    SPI_SPEED_025 = 250000,                  //!<Maximum 250KHz serial clock
    SPI_SPEED_05 = 500000,                   //!<Maximum 500KHz serial clock
    SPI_SPEED_1 = 1000000,                   //!<Maximum 1MHz serial clock
    SPI_SPEED_4 = 4000000,                   //!<Maximum 4MHz serial clock
    SPI_SPEED_8 = 8000000,                   //!<Maximum 8MHz serial clock
    SPI_SPEED_16 = 16000000,                 //!<Maximum 16MHz serial clock
    SPI_SPEED_32 = 32000000,                 //!<Maximum 32MHz serial clock

}spi_speed;

/*! 
    @brief The idle state the clock line is left with no activity
*/
typedef enum{

    SPI_CLOCK_LOW,                           //!<Clock is low when idle
    SPI_CLOCK_HIGH,                          //!<Clock is high when idle
}spi_clock;

/*! 
    @brief The number of data bits send in one transmission.
*/
typedef enum{

    SPI_DATA_8,                              //!<One data frame is 8 bits.
    SPI_DATA_16,                             //!<One data frame is 16 bits.

}spi_data;

/*! 
    @brief Byte order for send data.
*/
typedef enum{

    SPI_LSB,                                  //!<Data is sent LSB first
    SPI_MSB,                                  //!<Data is sent MSB first

}spi_endian;

/*! 
    @brief The phase at which the data is captured on the clock line.
    @details This value is relative to the clock idle line, so SPI_CLOCK_HIGH
    with SPI_PHASE_FIRST will capture data on the falling edge.
*/
typedef enum{

    SPI_PHASE_FIRST,                          //!<Captured on first transition
    SPI_PHASE_SECOND,                         //!<Captured on second transition

}spi_phase;
/*! 
    @brief The polarity of the chip select pin.
    @details This is used by SPI_write(), SPI_read(), SPI_repeatWrite() and
    SPI_repeatRead()
*/
typedef enum{

    SPI_CS_LOW,                               //!<Chip select pin is active low
    SPI_CS_HIGH,                              //!<Chip select pin is active high
}spi_csPol;

/*! 
    @brief Sets up the SPI peripheral with standard setting doneted with macro
    defines.
    @details 
    @param periph The port to set up.
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_setup(spi_periph periph);
/*! 
    @brief Sets the speed of the serial clock
    @details This function attempts to match the requested speed as close as 
    possible, without exceeding the rate specified.
    @param periph The port to modify
    @param speed The speed desired, defined by #spi_speed
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_setSpeed(spi_periph periph, spi_speed speed);
/*! 
    @brief Sets the polarity of the clock during the idle state
    @details 
    @param periph The port to modify
    @param polarity The clock polarity defined by #spi_clock
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_setClockPolarity(spi_periph periph,spi_clock polarity);
/*! 
    @brief Sets the size of the data frame for one transmission.
    @details 
    @param periph The port to modify
    @param polarity The frame size defined by #spi_data
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_setDataFrame(spi_periph periph,spi_data data);
/*! 
    @brief Sets order of the transmitted data frame
    @details 
    @param periph The port to modify
    @param polarity The frame order define by #spi_endian
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_setEndian(spi_periph periph,spi_endian endian);
/*! 
    @brief Sets the phase of the clock
    @details The works in conjuction with SPI_setClockPolarity(). Example:
    SPI_CLOCK_HIGH in combination with SPI_PHASE_SECOND will result on data
    being captured on the rising edge.
    @param periph The port to modify
    @param polarity The phase of the clock define by #spi_phase
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_setPhase(spi_periph periph,spi_phase phase);
/*! 
    @brief Get the function address of the get function for the port.
    @return Function address.
*/
spi_u16_fp_v SPI_getGetAdd(spi_periph periph);
/*! 
    @brief Get the function address of the put function for the port.
    @return Function address.
*/
spi_v_fp_u16 SPI_getPutAdd(spi_periph periph);
/*! 
    @brief Send one data frame on the data bus. (blocking)
    @details The size of the data fram is determined by SPI_setDataFrame(). Data 
    is not transmitted until the previous byte is sent.
    @param periph The port to modify
    @param data The data to send
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_put(spi_periph periph, uint16_t data);
/*! 
    @brief Recieve a byte from the recieve buffer. (blocking)
    @details The size of the data fram is determined by SPI_setDataFrame(). Data is
    not returned until a full frame is received.
    @param periph The port to modify.
    @return The data contined in the buffer.
*/
uint16_t SPI_get(spi_periph periph);
/*! 
    @brief Enable the peripheral's transmit interrupt.
    @details The given function is called when a data frame has finished being
    transfered along the serial line.
    @param periph The port to modify
    @param handler The function to call on ISR event.
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_enableTxISR(spi_periph periph, void(*handler)(void));
/*! 
    @brief Enable the peripheral's receive interrupt.
    @details The given function is called when a data frame has been received.
    @warning The Tx ISR flag can only be cleared by placing another data frame
    into the send buffer (with SPI_put()). This is not implemented in the library.
    Code execution will lock up if this is not done externally.

    @param periph The port to modify
    @param handler The function to call on ISR event.
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_enableRxISR(spi_periph periph, void(*handler)(uint16_t));
/*! 
    @brief Disable the peripheral's transmit interrupt.
    @details 
    @param periph The port to modify
    @warning The Rx ISR flag can only be cleared by reading the data frame
    in the send buffer (with SPI_get()). This is not implemented in the library.
    Code execution will lock up if this is not done externally.
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_disableTxISR(spi_periph periph);
/*! 
    @brief Disable the peripheral's receive interrupt.
    @details 
    @param periph The port to modify
    @return 
        - E_SPI_PORT if port doesn't exist.
        - E_SPI_NONE if no error.
*/
mcu_error SPI_disableRxISR(spi_periph periph);
/*! 
    @brief Write a data frame to a slave device using standard protocole.
    @param periph The port to write to.
    @param port The gpio port the chip select pin is attached to.
    @param pin The gpio pin on the port for chip select.
    @param polarity The polarity of the chip select pin given by #spi_csPol.
    @param address The address to send the data to.
    @param data The data to write to the bus.
*/
void SPI_write(spi_periph periph, gpio_port port, gpio_pin pin, 
        spi_csPol polarity, uint16_t address, uint16_t data);
/*! 
    @brief Write multiple data frames to a slave device using standard protocole.
    @param periph The port to write to.
    @param port The gpio port the chip select pin is attached to.
    @param pin The gpio pin on the port for chip select.
    @param polarity The polarity of the chip select pin given by #spi_csPol.
    @param address The address to send the data to.
    @param data Pointer to the data to write to the bus.
    @param count The number of data frames to send.
*/
void SPI_repeatWrite(spi_periph periph, gpio_port port, gpio_pin pin, 
        spi_csPol polarity, uint16_t address, uint16_t *data, uint16_t count);
/*! 
    @brief Read a data frame from the slave device using standard protocole.
    @param periph The port to read from.
    @param port The gpio port the chip select pin is attached to.
    @param pin The gpio pin on the port for chip select.
    @param polarity The polarity of the chip select pin given by #spi_csPol.
    @param address The address to read the data from.
    @return The data send from the slave device.
*/
uint16_t SPI_read(spi_periph periph, gpio_port port, gpio_pin pin, 
        spi_csPol polarity, uint16_t address);
/*! 
    @brief Read multiple data frames from the slave device using standard protocole.
    @details Data is returned in data
    @param periph The port to read from.
    @param port The gpio port the chip select pin is attached to.
    @param pin The gpio pin on the port for chip select.
    @param polarity The polarity of the chip select pin given by #spi_csPol.
    @param address The address to read the data from.
    @param data Pointer to the container to store the data.
    @param count The number of data frames to read.
*/
void SPI_repeatRead(spi_periph periph, gpio_port port, gpio_pin pin,
        spi_csPol polarity, uint16_t address, uint16_t *data, uint16_t count);

/**@}*/
#endif
