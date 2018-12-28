#include <stm32f103cb_core.h>
#include <ucconfig.h>
#include "variables.h"

/******************************************************************************/
    //Example usage of UCConfig module
    //UC Config can write data to flash EEPROM modules from PC at runtime.
    //This avoids the need for recompilation when changing program parameters
    //
    //The example uses the USB Driver for the STM32F1, this functionallity
    //can be changed to USART by passing appropriate function pointers.
/******************************************************************************/

//For this example the inital flash page used is 120
#define FLASH_PAGE 120

//Function to be called when new serial data is available to be read.
void serialReceived(uint8_t b);

//Function to write a single byte at a given address to internal flash.
void flashWrite(uint8_t byte,uint16_t address);

//Function to read a single byte at a given address from internal flash.
uint8_t flashRead(uint16_t address);

//Function which is called when the first write operation occurs
void onFirstWrite(void);

//Function which is called when config mode is exited.
//This function is used to load the values for all below variables
void exitingConfigMode(void);

//Bunch of variable for demonstration
uint8_t u8;
int8_t s8;
uint16_t u16;
int16_t s16;
uint32_t u32;
int32_t s32;
float f;
char c;

int main(void){

    //Always need setup the USB callbacks when using USB
    //Equivalently, USART would be set up here
    USB_coms(SYSTICK);

    //Set serial Received to be called when new data is available through USB
    USB_setGet(&serialReceived);

    //Setup GPIO for some flashing magic
    //The pin flashes when the config module is activly reading or writing
    pinSetup(GPIO_DO,PORTC,PIN13);

    //If an external EEPROM was used, it would need to be set up here first,
    //along with I2C or whichever peripheral it requires.
    //Internal flash is used here so no setup is required.

    //Setup UCCONFIG module, pass the three function pointers
    UCCONFIG_setup(&flashRead,&flashWrite,&USB_put);

    //Optionally, set the memory offset if flash which is used. No offset needed 
    //here so 0 is passed
    UCCONFIG_setAddressOffset(0);

    //Optionally, a function can be called when config mode is entered
    UCCONFIG_setOnFirstWrite(&onFirstWrite);

    //Optionally, a function can be called when config mode is entered
    UCCONFIG_setOnExit(&exitingConfigMode);

    //Set inital pin state high (LED off)
    pinHigh(PORTC,PIN13);

    //Exit config mode sets all the variables, call it here to populate themn
    //from flash
    exitingConfigMode();


    while(1){

        delayms(2000);
        //The loop is what 'enters' config mode, it should be somewhere in the main loop or
        //on a timer callback
        UCCONFIG_loop();

        prints("U8 =");
        printl(u8);

        prints("S8 =");
        printl(s8);
        delayms(100);

        prints("U16 =");
        printl(u16);

        prints("S16 =");
        printl(s16);
        delayms(100);

        prints("U32 =");
        printl(u32);

        prints("S32 =");
        printl(s32);
        delayms(100);

        prints("F =");
        printl(f);

        prints("C =");
        printl(c);

        printl("");
    }
}

//Repopulate all variable from values in flash
//This is called on inital setup and when config mode is exited.
void exitingConfigMode(void){

    UCCONFIG_get(&u8,U8);
    UCCONFIG_get(&s8,S8);
    UCCONFIG_get(&u16,U16);
    UCCONFIG_get(&s16,S16);
    UCCONFIG_get(&u32,U32);
    UCCONFIG_get(&s32,S32);
    UCCONFIG_get(&f,F);
    UCCONFIG_get(&c,C);
}

//This is called when data is received.
void serialReceived(uint8_t b){

    //Need to call UCCONFIG_listen() here. So the module can intercept and
    //become active when the correct key is received via serial.
    UCCONFIG_listen(b);

}

//Function to write to flash
void flashWrite(uint8_t byte,uint16_t address){

    //Notice a delay is provided here to ensure there is enough time
    //for the flash memory to be written

    //A fancy flash (active low) is also demonstrated
    pinLow(PORTC,PIN13);
    FLASH_write(FLASH_PAGE,address,byte);
    delayms(10);
    pinHigh(PORTC,PIN13);
    return;
}

//Function to read from flash
uint8_t flashRead(uint16_t address){

    return FLASH_read(FLASH_PAGE,address);
}

void onFirstWrite(void){

    //The internal flash memeory on stm32f1 can only be set once before it needs to be erased
    //Erase it here when first byte operation occurs
    //More pages could be cleared if more flash was needed.
    FLASH_clearPage(FLASH_PAGE);
}

