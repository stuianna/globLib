# STLINK STM32F103CB PROJECT Template

This directory contains a project template for working with the MCU without a bootloader. If this is flashed to the MCU it will overwrite the bootloader (if installed). The bootloader can be reinstalled with instructions in directory usb_bootloader.

## Requirements

Complete installation instructions can be found in the STM32F103CB globLib main directory, or by typing **make libdoc** from this directory and navigating to STM32F103CB on the top tab bar.

The following components are required by this template.

- A local copy of [globLib](https://github.com/stuianna/globLib)
- arm-non-eabi-gcc 
- STLink-V2
- OpenOCD 
- Doxygen (Optional for document generation)

## Template Contents

- **src**  - All source files for the project go here or in recursive directories
- **doc**  - Generated Doxygen documentation, additional datasheets etc..
- **conf** - Configuration files for Doxygen
- **obj**  - Generated when compiled, contains output object files
- **bin**  - Generated when compiled, contains output program and map file.
- **licence.md** - Licence file for the project
- **readme.md** - File containing these instruction. This file can also be used as the Doxygen main-page.

## Usage

Basic usage of this template:

1. Copy this directory to the desired project location
2. Open the makefile
    - Change the name of target to the project name. This changes the output file as well as the Doxygen project documentation name.
    - Make sure the path to glob_lib is correct
3. Use the make recipes to work with the project.

## Make Recipes

- **make**      -       Build project.
- **make clean** -      Clean object and output files.
- **make flash** -      Deploy output to microcontroller with STlink.
- **make erase** -      Erase the flash memory of the stm32f1 (Removes any data and program).
- **make libdoc** -     View the GlobLib Doxygen documentation
- **make docs** -       Compile project Doxygen documentation (Requires Doxygen)
- **make viewdox** -    View project specific documentation 
- **make cleandoc** -   Remove project specific documentation

## Documentation

This template is configured to use Doxygen for document generation. The documentation contains three modules with a main-page generated using the content of readme.md (this file). The modules are as follows:

- **Project** A separate module for the project. It can be renamed by changing tags in the source file. Additional modules can be added as well. (See Doxygen documentation)
- **STM32F103CB** - A module for the microcontroller globLib documentation. This provides a reference for all library function calls used. 
- **COMMON** - A module for the common globLib documentation. This provides a reference for all library function calls used. 

The common and stm32f103cb documentation can be removed by modifying the makefile *make docs* recipe. Further modification of the generated documentation can be make by editing conf/doxy and conf/DoxyLayout files.

The documentation features of this template are optional.

