/*!**************************************************************************
    @file stm32f103cb_flash.c
    @brief Source file for stm32f103cb flash 
    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
******************************************************************************/

#include "stm32f103cb_flash.h"

#define FLASH_PAGE_SIZE 255         //!<The size of each page in words, used internally
#define FLASH_PAGE_BASE 0x8000000   //!<The flash starting address, used internally

mcu_error FLASH_clearPage(uint32_t pageNumber){

    if(pageNumber > 127){

        //Page out of bounds
        if(MCU_debugIsEnabled()){

            MCU_printError(E_FLASH_PAGE);
        }
        return E_FLASH_PAGE;
    }

    flash_unlock();
    flash_erase_page(FLASH_PAGE_BASE + (pageNumber<<10));
    flash_lock();

    return E_FLASH_NOERROR;
}

mcu_error FLASH_write(uint32_t pageNumber, uint32_t address, uint32_t data){

    //Check if page is within bounds
    if(pageNumber > 127){

        //Page out of bounds
        if(MCU_debugIsEnabled()){

            MCU_printError(E_FLASH_PAGE);
        }
        return E_FLASH_PAGE;
    }

    flash_unlock();
    flash_program_word(FLASH_PAGE_BASE + (pageNumber<<10) + (address<<2), data);
    flash_lock();

    return E_FLASH_NOERROR;
}

mcu_error FLASH_writes(uint32_t pageNumber, uint32_t address, uint32_t *data, uint16_t size){

    //Check if page is within bounds
    if(pageNumber > 127){

        //Page out of bounds
        if(MCU_debugIsEnabled()){

            MCU_printError(E_FLASH_PAGE);
        }
        return E_FLASH_PAGE;
    }
    
    flash_unlock();

    pageNumber = FLASH_PAGE_BASE + (pageNumber<<10) + (address << 2);

    for(int i = 0; i < size; i++){

        flash_program_word(pageNumber + (i<<2), *(data++));
    }

    flash_lock();

    return E_FLASH_NOERROR;
}

uint32_t FLASH_read(uint32_t pageNumber, uint32_t address){

    //No error checking here, shouldn't matter as it is just reading a memory address
    
    return *((uint32_t*)(FLASH_PAGE_BASE + (pageNumber<<10) + (address<<2)));

}

mcu_error FLASH_reads(uint32_t pageNumber, uint32_t address, uint32_t *container, uint16_t size){

    //Check if page is within bounds
    if(pageNumber > 127){

        //Page out of bounds
        if(MCU_debugIsEnabled()){

            MCU_printError(E_FLASH_PAGE);
        }
        return E_FLASH_PAGE;
    }

    pageNumber = FLASH_PAGE_BASE + (pageNumber<<10) + (address << 2);

    for(uint16_t i = 0; i < size; i ++){

        *(container++) = *((uint32_t*)( pageNumber + (i<<2)));
    }
    return E_FLASH_NOERROR;

}

mcu_error FLASH_readPage(uint32_t pageNumber, uint32_t *container){

    //Check if page is within bounds
    if(pageNumber > 127){

        //Page out of bounds
        if(MCU_debugIsEnabled()){

            MCU_printError(E_FLASH_PAGE);
        }
        return E_FLASH_PAGE;
    }

    pageNumber = FLASH_PAGE_BASE + (pageNumber << 10);

    for(uint16_t i = 0; i < FLASH_PAGE_SIZE; i ++){

        *(container++) = *((uint32_t*)(pageNumber + (i<<2)));
    }
    return E_FLASH_NOERROR;

}

uint8_t FLASH_pageEmpty(uint32_t pageNumber){

    pageNumber = FLASH_PAGE_BASE + (pageNumber << 10);

    for(uint16_t i = 0; i < FLASH_PAGE_SIZE; i++){

        //Check if each location is zero
        if((*((uint32_t*)( pageNumber + (i<<2)))) != 0xFFFFFFFF){

            return 0;
        }

    }
    return 1;
}

uint8_t FLASH_firstEmptyPage(void){

    for(uint8_t i = 0; i < 127; i++){

        if(FLASH_pageEmpty(i)){

            return i;
        }
    }

    //Return zero if no empty pages found.
    return 0;

}
