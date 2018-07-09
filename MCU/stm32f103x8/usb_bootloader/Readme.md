The USB bootloader is designed to upload new firmware on STM32F1 devices.

  The bootloader can always be entered by:

    - Holding Port A pin 1 high and cycle the power to the device.

   Additionally the bootloader can be entered through software in the main 
   application if the USB port is active. This is achieved by
    
    - Sending 8 null characters ( \0 ) sequentially through the USB port.

   The bootloader is also triggered (once) per power cycle from the following
   events

    - An occurance of a hard fault 
    - Reset from independant watchdog timer.

    These events are triggered by the value stored in the first data register
    of the backup domain. If this register is used for another function the 
    bootloader may not trigger from software as expected.

    The majority of this bootloader is adapted from libopencm3 examples.
    Modifications have been made to trigger the events above.

Installing the bootloader:

    The stlink-v2 device is needed to flash the bootloader to the device. This
    can be achieved in this directory by.
    
        1. Clean the flash memory on the device

            make erase      

        2. Compile the bootloader

            make all

        3. Deploy the bootloader
        
            make flash  
    
    The stm32f1 can now be flashed with the USB port. Use bootloader_template to 
    test.
