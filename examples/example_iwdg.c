#include <stm32f103cb_core.h>
#include <coms128.h>

volatile int cap = 10;
int count;

int main(void){

    //Serial setup
    USART_setup(USART_1);
    USART_setBaud(USART_1,USART_BAUD_115200);
    COMS(USART_1);

    if(IWDG_resetCheck()){

        //Ideally this should be first part of program.
        //Located here to demonstrate example with terminal
        printl("Reset from IWDG");
        
    }
    else{

        printl("Normal reset");
    }

    //Setup watchdog for reset after 1 second if not fed.
    IWDG_setup(1000);

	while (1){

        //Feed the dog
        prints("Feeding no");
        printl(count++);
        IWDG_feed();
        for(volatile int i = 0; i < cap; i++);

        //Each time the loop gets longer
        cap *= 10;
    }

	return 0;
}

