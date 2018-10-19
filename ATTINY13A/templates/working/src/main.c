#include <attiny13a_core.h>

int main(void)
{

    pinSetup(GPIO_DO,PORTB,PIN4);
    

    while (1) {

    pinToggle(PORTB,PIN4);
    delayms(500);

    }

    return 0;
}

