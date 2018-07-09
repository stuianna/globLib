#include <stm32f1x3_core.h>
#include <string11.h>

//This gets called when new data is available
void data_available(uint8_t byte);

int main(void){

    //Clock speed needs to be 48MHz or greater
    CLOCK_setSpeed(CLOCK_EX_8_OUT_72);

    //Setup the USB
    USB_setup();

    //Target received data to function
    USB_setGet(&data_available);

    //USB MUST be refreshed via a timer at at least 4KHz (higher rate = more CPU)
    SYSTICK_setup(250,&USB_update);

    //Set the string output to USB
    STRING11_setOutput(&USB_put);

    //Print message
    printl("USB test. Echo characters to terminal.");

    //Done
    while(1);

    return 0;
}

void data_available(uint8_t byte){

    print((char)byte);
}
