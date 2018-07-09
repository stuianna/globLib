#include <stm32f1x3_core.h>

void flash(void);

int main(void)
{

    //Enable LED pin
    pinSetup(GPIO_DO,PORTC,PIN13);

    //Setup systick timeout and handler, handler must be void(*handler)(void)
    SYSTICK_setup(500000,&flash);

    //Done
	while (1);

	return 0;
}

//This gets called on systick timeout
void flash(void){

    pinToggle(PORTC,PIN13);
}

