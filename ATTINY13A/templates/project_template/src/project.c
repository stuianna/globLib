#include "project.h"

int main(void){

    //Setup gpio
    pinSetup(GPIO_DO,PORTB,PIN4);
    

    while(1){

        //Toggle pin every 500ms
        pinToggle(PORTB,PIN4);
        delayms(500);
    }

    return 0;
}

