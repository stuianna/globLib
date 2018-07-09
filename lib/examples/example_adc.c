#include <stm32f1x3_core.h>
#include <coms128.h>

void up(void);
volatile uint8_t flag;

int main(void){

    //Clock setup
    CLOCK_setSpeed(CLOCK_EX_8_OUT_72);

    //Serial setup
    USART_setup(USART_1);
    USART_setBaud(USART_1,USART_BAUD_115200);
    COMS(USART_1);

    //Setup systick for update at 20ms on function up()
    SYSTICK_setup(20000,&up);

    //Setup the ADC
    ADC_setup(PORTA,PIN0);

    //Add an additional channel
    ADC_addChannel(PORTA,PIN1);

	while (1){

        if(flag){

            flag = 0;
            printt(ADC_sample(PORTA,PIN0));     //12-Bit sample
            printl(ADC_volts(PORTA,PIN1));      //voltage sample
        }
    }

	return 0;
}

//This is called by systick on timeout
void up(void){

    flag = 1;
}
