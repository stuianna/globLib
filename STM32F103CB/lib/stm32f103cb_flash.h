#ifndef STM32F103CB_FLASH_H_
#define STM32F103CB_FLASH_H_

/**
    @addtogroup STM32F103CB 
* @{
*/

/**
    @addtogroup FLASH
    @brief This module contains functions for modifing internal flash memory.
    @details 

    The STM32f1x3 has 128kb of programmable flash memory. This space can also
    be used to store user values at runtime. It has the following restrictions

    - Bits can only be set from 1 to 0. This means each address can only be used
    once before it needs to be erased.
    - Erasing is done at a page level, for this device each page is 1kb
    - The read and write operations block the CPU. 
    - If a new uploaded program can overwrite existing flash data.

    It can be benificial to occasionally clear all flash memory, this ensures the data
    occupying the flash memory is being used. This can be acheived with "make erase" 
    from program directory.

    Flash memory can only be changed from 1 to 0. This means each location needs to be
    reset with FLASH_clearPage() before it is writted again.

    This module provides access to the flash memory. Typically pages toward the
    module (pages 120 - 127) should be used. The exact location of the free
    portion not used by code can be found by looking at the "text" section when the program
    is compiled.

    Each page contains 256 words (32 bit) segments which can be used.

    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @bug None
    @todo
    @warning Take note of where data is written. It is possible to overwrite
    the application code.

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

    @par Working example. 
    Notice every time the MCU is flashed or reset the first empty page is incremented
    by one. This occurs as the program only erases the data needed for the binary. 
    Use "make erase" to clear enitre
    flash memory of MCU
    @include example_flash.c

 * @{
 */

/*! @file stm32f103cb_flash.h
    @brief Header file for stm32f103cb FLASH 
*/

#include "stm32f103cb_debug.h"
#include <libopencm3/stm32/flash.h>
#include <stdint.h>


/*! 
    @brief Clear a page (1kb) of flash
    @details This needs to be used if the same memory location is used multiple times.

    Flash memory can only be changed from 1 to 0. This means each location needs to be
    reset with FLASH_clearPage() before it is written again.

    @param pageNumber The page to clear (0-127)
    @warning It is possible to remove application code with this function.
    @return Error number as defined in #mcu_error, E_FLASH_NOERROR if no error,
    E_FLASH_PAGE if page is out of range
*/
mcu_error FLASH_clearPage(uint32_t pageNumber);
/*! 
    @brief Write a single word to flash
    @details A word can only be written to a location once before FLASH_clearPage() 
    is used to reset the memory location

    It is possible to use a word address greater than 255. This means that the data will
    be located in the next page.

    @param pageNumber The page to write to (0-127)
    @param address The word in that page (0-255)
    @param data The data to write to the location
    @warning It is possible to remove application code with this function.
    @return Error number as defined in #mcu_error, E_FLASH_NOERROR if no error,
    E_FLASH_PAGE if page is out of range
*/
mcu_error FLASH_write(uint32_t pageNumber, uint32_t address, uint32_t data);
/*! 
    @brief Write multiple words to flash, address incremented automatically
    @details A word can only be written to a location once before FLASH_clearPage() 
    is used to reset the memory location

    It is possible to use a word address greater than 255. This means that the data will
    be located in the next page.

    @param pageNumber The page to write to (0-127)
    @param address The word in that page (0-255)
    @param data Pointer to the data to write to the location
    @param size The size of parameter data
    @warning It is possible to remove application code with this function.
    @warning If size is larger than data, a memeory fault will occur
    @return Error number as defined in #mcu_error, E_FLASH_NOERROR if no error,
    E_FLASH_PAGE if page is out of range
*/
mcu_error FLASH_writes(uint32_t pageNumber, uint32_t address, uint32_t *data, uint16_t size);
/*! 
    @brief Read a single word from flash memory
    @details 

    @param pageNumber The page to read from. (0-127)
    @param address The word in that page (0-255)
    @return The data at the given address
*/
uint32_t FLASH_read(uint32_t pageNumber, uint32_t address);
/*! 
    @brief Read multiple words from flash
    @details 

    @param pageNumber The page to read from. (0-127)
    @param address The word in that page to start from (0-255)
    @param data Pointer to a memory container large enough to hold the data
    @param size The size of parameter data
    @warning If size is larger than data, a memeory fault will occur
    @return Error number as defined in #mcu_error, E_FLASH_NOERROR if no error,
    E_FLASH_PAGE if page is out of range
*/
mcu_error FLASH_reads(uint32_t pageNumber, uint32_t address, uint32_t *data, uint16_t size);
/*! 
    @brief Read an entire page from flash
    @details 

    @param pageNumber The page to read from. (0-127)
    @param data Pointer to a memory container large enough to hold the data
    @warning The size of data must be greater than 256 words ie. uint32_t data[256]
    @return Error number as defined in #mcu_error, E_FLASH_NOERROR if no error,
    E_FLASH_PAGE if page is out of range
*/
mcu_error FLASH_readPage(uint32_t pageNumber, uint32_t *data);
/*! 
    @brief Determine if a given page is empty
    @details 

    @param pageNumber The page to check. (0-127)
    @warning This function doesn't check page bounds
    @return 
    - 1 if page is empty
    - 0 if page containes data
*/
uint8_t FLASH_pageEmpty(uint32_t pageNumber);
/*! 
    @brief Get the first empty page is flash
    @details 
    @warning This doesn't work in conjuction with the USB bootloader, as if
    the entire contents were erased, the bootloader would be destroyed.

    @return The first empty page which can be used, returns 0
    if no pages are empty.
*/
uint8_t FLASH_firstEmptyPage(void);

//Read page
//Write page

/**@}*/
/**@}*/
#endif
