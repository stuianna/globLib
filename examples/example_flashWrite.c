#include <stm32f103cb_core.h>
#include <flashWrite.h>

//This example uses the STM32f1 internal flash memory.

//STM32f1 requires flash page address to write to
//See example_flash.h
#define FLASH_PAGE 120

//STM32f1 Flash input and output functions don't match requried flash format
//Use function wrappers to obtain the required types
void flashWrite(uint8_t byte, uint16_t address);
uint8_t flashRead(uint16_t address);

int main(void){


    //Set the input and output function pointers
    FLASHWRITE_setOutput(&flashWrite);
    FLASHWRITE_setInput(&flashRead);

    //Example variables
    float ex_f;
    char ex_c;
    uint16_t ex_u16;
    int32_t ex_32;

    //Current memory address location
    uint16_t addressLocation = 0;

    ex_f = 12.34;
    ex_c = 'A';
    ex_u16 = 12345;
    ex_32 = -235001;

        
    //Put the float at memory address
    //The new memory address is retured by flash put based on the size of the data written 
    addressLocation = flash_put(ex_f,addressLocation);
    addressLocation = flash_put(ex_c,addressLocation);
    addressLocation = flash_put(ex_u16,addressLocation);
    addressLocation = flash_put(ex_32,addressLocation);

    //Reset the address location and read data back
    //Address is incremented similarly to flash_put
    addressLocation = 0;
    addressLocation = flash_get(&ex_f,addressLocation);
    addressLocation = flash_get(&ex_c,addressLocation);
    addressLocation = flash_get(&ex_u16,addressLocation);
    addressLocation = flash_get(&ex_32,addressLocation);

    //Done
    while(1);

    return 0;
}

//Function wrapper for flash write function
//This is called by FLASHWRITE API when byte write operations occur
void flashWrite(uint8_t byte, uint16_t address){

    //A different method for writing to other flash EEPROM could be used here

    FLASH_write(FLASH_PAGE,address,byte);

    //Need to implement a delay here to allow for write cycles
    delayms(10);

    return;
}

//Function wrapper for flash read function
//This is called by FLAHSWRITE API when byte read operation occur
uint8_t flashRead(uint16_t address){

    //A different method for reading to other flash EEPROM could be used here

    return FLASH_read(FLASH_PAGE,address);
}
