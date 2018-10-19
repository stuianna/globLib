#include "project.h"

int main(void){

    //Setup gpio
    pinSetup(GPIO_DO,PORTC,PIN13);

    while(1){

        //Toggle pin every 500ms
        pinToggle(PORTC,PIN13);
        delayms(500);

    }

    return 0;
}

