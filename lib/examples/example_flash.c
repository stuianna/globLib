#include <stm32f1x3_core.h>
#include <coms128.h>

uint32_t readData[3];
uint32_t writeData[2];

int main(void){

    uint8_t pageBase;

    //Serial setup
    USART_setup(USART_1);
    USART_setBaud(USART_1,USART_BAUD_115200);
    COMS(USART_1);

    //Some random data
    writeData[0] = 456;
    writeData[1] = 789;

    //Get the first empty page to use
    pageBase = FLASH_firstEmptyPage();

    //Should check device is not full
    if(pageBase == 0){

        printl("No free pages");

        //Halt here
        while (1);
    }

    //Print some data to word zero of that page
    FLASH_write(pageBase,0,123);

    //Print some more data to that page, starting at word 1 
    FLASH_writes(pageBase,1,writeData,sizeof(writeData));

    //Read the data written, starting from word zero
    FLASH_reads(pageBase,0,readData,sizeof(readData));

    //Print out info
    prints("First empty page is page");
    printl(pageBase);

    printl("Data read");
    prints(readData[0]);
    prints(readData[1]);
    printl(readData[2]);
    
    printl("Done");

	while (1);
	return 0;
}

