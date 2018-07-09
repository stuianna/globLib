#include <stm32f1x3_core.h>

void data(uint8_t b);

int main(void){

    //Always call this (or equivalent) first
    USB_coms(TIMER_2);

    //This is optional, but allows characters to be echoed from the terminal
    USB_setGet(&data);

    //Setup GPIO
    pinSetup(GPIO_DO,PORTC,PIN13);

    while(1){

        //Toggle pin every 500ms
        pinToggle(PORTC,PIN13);
        delayms(500);
    }
}

void data(uint8_t b){

    print((char)b);

}

