# STM32F103 USB Bootloader

The USB bootloader is designed to upload new firmware on STM32F1 devices. It works
in conjunction with [dfu-util](http://dfu-util.sourceforge.net/) to send new programs to the microcontroller. The bootloader
jumps to the main application should none of the following conditions are met.

- Holding Port A pin 1 high and cycle the power to the device.
- Sending 8 null characters ( \0 ) sequentially through the USB port.
- An occurrence of a hard fault
- A valid program not found at the application start address (0x08003000)

The bootloader checks for these conditions using the first data register
in the backup domain. If this register is used for another function then the bootloader
may not function as expected.

The majority of this bootloader is adapted from libopencm3 examples.
Modifications have been made to trigger the events above.

The main application should be built with the [bootloader_project_template](www.). This 
template incorporates a linker script to offset the application start address as well as
the USB peripheral setup.

Complete installation instructions can be found in the STM32F103CB globLib main directory, or by typing **make libdoc** from this directory and navigating to STM32F103CB on the top tab bar.

The following components are required by this template.

- A local copy of [globLib](https://github.com/stuianna/globLib)
- arm-non-eabi-gcc 
- STLink-V2 (To flash the original bootloader)
- OpenOCD 
- Doxygen (Optional for document generation)

## Project Contents

- **src**  - All source files for the project go here or in recursive directories
- **doc**  - Generated Doxygen documentation, additional datasheets etc..
- **conf** - Configuration files for Doxygen
- **obj**  - Generated when compiled, contains output object files
- **bin**  - Generated when compiled, contains output program and map file.
- **licence.md** - Licence file for the project
- **readme.md** - File containing these instruction. This file can also be used as the Doxygen main-page.

## Usage

 1. Clone the repository.
 2. Configure the makefile so the path to GlobLib is correct.
 3. Connect the MCU and STLink.
 4. Run **make all** to build the program.
 5. Run **make flash** to flash the program to the MCU.
 6. Done.

Future programs can now be flashed to the MCU with the USB peripheral and the USB bootloader template program [link](www.google.com)

## Make Recipes

- **make**      -       Build project.
- **make clean** -      Clean object and output files.
- **make flash** -      Deploy program to microcontroller with STlink.
- **make erase** -      Erase the flash memory of the STM32F1 (Removes any data and program).
- **make libdoc** -     View the GlobLib Doxygen documentation
- **make docs** -       Compile project Doxygen documentation (Requires Doxygen)
- **make viewdoc** -    View USB bootloader specific documentation 
- **make cleandoc** -   Remove project specific documentation

## Documentation

The documentation for the project can be viewed at doc/html/index.html of by running **make libdoc** from the project root directory.

