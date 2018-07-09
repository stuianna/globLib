#include <stm32f1x3_core.h>
#include <coms128.h>

void up(void);
volatile uint8_t flag;

int main(void){

    //Clock setup
    CLOCK_setSpeed(CLOCK_EX_8_OUT_72);

    //Serial setup
    USART_setup(USART_2);
    USART_setBaud(USART_2,USART_BAUD_115200);
    COMS(USART_2);

    //Setup timer 3 to call function "up" at 10Hz
    TIMER_setupCount(TIMER_3,20,&up);               //Period = 50000 -> Resolution = 1us

    //Setup timer 1 for pulse width output at 1000Hz
    TIMER_setupPulse(TIMER_1,CHANNEL_1,1000,400);   //400us pulse
    TIMER_setupPulse(TIMER_1,CHANNEL_2,1000,50);    //50us pulse

    //Setup timer 2 for PWM at 10kHz
    TIMER_setupPWM(TIMER_2,CHANNEL_1,10000,50);     //50% duty cycle

    //Setup timer four for input capture
    TIMER_setupIC(TIMER_4,CHANNEL_1);
    TIMER_setupIC(TIMER_4,CHANNEL_2);
    TIMER_setupIC(TIMER_4,CHANNEL_3);
    TIMER_setupIC(TIMER_4,CHANNEL_4);

    //Change duty cycle and pulse output
    TIMER_setDuty(TIMER_1,CHANNEL_1,80);
    TIMER_setPulse(TIMER_1,CHANNEL_2,100);
    TIMER_setDuty(TIMER_1,CHANNEL_2,10);

	while (1){

        if(flag){

            flag = 0;

            //Time how long it takes to compete print function with TIMER_3
            TIMER_setCount(TIMER_3,0);

            //Print input capture values
            printt(TIMER_getIC(TIMER_4,CHANNEL_1));
            printt(TIMER_getIC(TIMER_4,CHANNEL_2));
            printt(TIMER_getIC(TIMER_4,CHANNEL_3));
            printt(TIMER_getIC(TIMER_4,CHANNEL_4));

            printl(TIMER_getCount(TIMER_3)); //Printing the time difference
        }
    }

	return 0;
}

//This is called by TIMER_3 on timeout
void up(void){

    flag = 1;
}
