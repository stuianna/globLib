This folder contains a template for working with the MCU without the bootloader.
This requires the stlink to be connected to the MCU.

If this is flashed to the MCU it will overwrite the bootloader (if installed). The
bootloader can be reinstalled with instructions in directory usb_bootloader

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

