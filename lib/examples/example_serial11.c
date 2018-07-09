//Change to include appropriate MCU target
#include "stm32f103c8_core.h"
#include "string11.h"

//Create an instance for the USART object -> required for demo
USART serial;

int main(void)
{

	//USART Hardware setup
	USART_setup(&serial,USART_1); 	

    //Set the string11 library to output on the USART.put function
    STRING11_setOutput(serial.put);

    //Create some variables
    uint8_t a = 12;
    int16_t b = -212;
    int32_t c = 1231234;
    float   d = -1235.23;
    char    e = '!';
    char   *f = "Good";

    //Print to terminal
	while(1){

        printt("Some Data");                //Print with a tab
        prints(a);                          //Print with a space
        prints(b);                          //Print with a space
        prints(c);                          //Print with a space
        print(d);                           //Print 
        prints(e);                          //Print with a space
        prints(123.32f);                    //Print with a space
        printc(f);                          //Print with a comma
        printl("Done");                     //Print with a line
        for(int i = 0; i < 0x2EEEE;i++);    //Wait

	}
	return 0;
}



