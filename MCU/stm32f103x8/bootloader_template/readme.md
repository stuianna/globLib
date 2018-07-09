This template is designed to be used with the usb_bootloader.

It is requred that the bootloader is flashed onto the microcontroller first
with the stlink tool. This can be done in the usb_bootloader directory.

It is important that the usb port is enabled in every version of software 
downloaded to flash. In the simplest for this can be acheived with calling
USB_coms(TIMER_1) at the start of the application. (see template).

If the software is locked and cannot enter bootmode the this can be forced by
holding port a pin 0 high which the power is cycled on the MCU.

To use this template:

    1. Copy this directory to another location
    2. Change the name of blink.c to "new project name"
    3. Open the makefile
    4. Change BINARY = blink to BINARY = "new project name"
    5. Make sure the path to glob_lib is correct
    6. From the terminal in the new project directory run
        - make all
        - make flash
    7. Done

