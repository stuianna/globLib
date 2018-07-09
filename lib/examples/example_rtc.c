#include <stm32f1x3_core.h>

void alarm(void);
void resolution(void);
void overflow(void);

int main(void){

    //Coms for terminal info
    USB_coms(TIMER_2);

    //The value passed here is meaningless as RTC_calibrate is called next.
    RTC_setup(500);

    //This uses systick!
    //This takes 500ms to complete! 
    //(smaller resolution, shorter time, less accurate)
    RTC_calibrate(500);
    //Systick could be used here again if needed
   
    //Good to reset the count after so everything starts at 0
    RTC_setCount(0);

    //Call alarm after 2 resultion counts (1 second)
    RTC_enableAlarmISR(2,&alarm);

    //Call resolution every period. (500ms)
    RTC_enableResolutionISR(&resolution);

    //Call this after a couple of years.
    RTC_enableOverflowISR(&overflow);

    //Done
    while(1);
    return 0;
}

void alarm(void){

    printl("Alarm");

    //The clock continues to count.
    //Set the alarm again after 2 counts (1 second)
    RTC_enableAlarmISR(RTC_getCount() + 2,&alarm);
}

void resolution(void){

    printl("Resolution");
} 

void overflow(void){

    printl("Overflow");

}
