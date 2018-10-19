#include <stm32f103cb_core.h>
#include <coms128.h>

void data(uint8_t byte);
volatile uint8_t flag;
void callback(void);
void rising(void);
void systemInit(void);

int main(void){

    systemInit();

    //RTC runs in stop mode
    //It only gets set up once
    RTC_setup(100);
    RTC_calibrate(100);

    //Display method of reset
    if(POWER_fromStandby()){

        printl("Standby reset");

    }
    else{

        printl("Soft reset");
    }

    while(1){

        switch(flag){

            case 1:

                flag = 0;
                printl("Sleeping, wake on systick ISR");
                delayms(100);           //Delay for message to print.
                SYSTICK_setup(3000000,&callback);   //3 seconds
                POWER_sleep();
                printl("Woke from sleep");
                break;

            case 2:

                flag = 0;
                printl("Stoping, wake on EXTI ISR");
                delayms(100);           //Delay for message to print.
                RTC_enableAlarmISR(RTC_getCount() + 30,NULL);
                POWER_stop();
                printl("Woke from stop");
                break;
            case 3:

                flag = 0;
                printl("Standby, wake after 3 seconds");
                delayms(100);           //Delay for message to print.

                //Standby wakes from RTC alarm
                RTC_enableAlarmISR(RTC_getCount() + 30,NULL); 
                POWER_standby();
                break;
            default:
                //Do nothing
                break;
        }
    }
}

//Chosse which power mode to use

// 1 = Sleep
// 2 = Stop
// 3 = Standby
// 4 = Reset
void data(uint8_t byte){

    switch(byte){

        case '1':

            flag = 1;
            break;
        case '2':

            flag = 2;
            break;
        case '3':

            flag = 3;
            break;
        case '4':

            POWER_reset();
            break;
        default:

            //Echo characters
            print((char)byte);
            break;
    }

}

//Systick callback to wake from sleep mode
void callback(void){

    //Disable sleep mode
    //If not disabled sleep mode begins again after ISR
    POWER_noSleep();

    //Stop the timer
    SYSTICK_stop();
}

//Rising edge on port A pin 0
void rising(void){

    if(POWER_fromStop()){

        systemInit();
    }
}

void systemInit(void){

    USART_setup(USART_1);
    USART_setBaud(USART_1,USART_BAUD_115200);
    USART_setRxISR(USART_1,&data);
    COMS(USART_1);
    GPIO_ISREnable(PORTA,PIN0,GPIO_RISING,&rising);
}
