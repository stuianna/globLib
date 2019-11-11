# Introduction

GlobLib is combination of a hardware abstraction layer and common libraries which can be 
shared across multiple microcontroller units (MCU). It is designed to provide boilerplate,
cross-MCU compatible code implementation of the most commonly used features. This project 
was created to both support code reuseability and as an education exercise in multi-architecture firmware develop for myself.

GlobLib focuses on proving functionality and portability for the most common features 
of each peripheral. 

# Supported Hardware

### MCU

|MCU            |STM32F103CB        |ATTINY13A      | 
|---------------|:-----------------:|:-------------:|
|GPIO           |Full               |Full           |
|USART          |Full               |Pending        |
|SPI            |Full               |Pending        |
|I2C            |Full               |Pending        |
|CAN            |Pending            |n/a            |
|ETHERNET       |n/a                |n/a            |
|USB            |Full               |n/a            |
|ADC            |Full               |Pending        |
|DMA            |Pending            |n/a            |
|TIMERS         |Full               |Pending        |
|SYSTICK        |Full               |n/a            |
|RTC            |Full               |n/a            |
|WATCHDOG       |Full               |Pending        |
|CLOCK CONFIG   |Full               |Full           |
|POWER MODES    |Full               |Pending        |
|EEPROM         |Full               |Pending        |
|BOOTLOADER     |Yes (USB)          |n/a            |

### IMU

-   MPU6050
-   LSM9DS1 (SPI interface only)

## Software Modules

Additional software modules are included in COMMON with complete Doxygen generated
documentation.

# Documentation

[Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html) is used to automatically
generate extensive documentation of this collection and its features. This document
can be accessed through [Github Pages](https://stuianna.github.io/globLib/doc/doxygen/html/index.html) or by typing **make libdoc** from any of the project templates. A local copy of this repository needs to made before this documentation can be accessed.

# Installation / Usage

This whole repository should be cloned onto a local machine to function correctly.

 1. Clone this repository onto the local machine
 2. Run `chmod +x config.sh` to make the configuration file executable
 3. Execute the config file with `./config.sh`. This script sets the glob_lib variable in each Makefile to the current root directory. This should also be run if the main glob_lib folder is moved.

Each supported MCU has a templates containing a project template and specific 
instructions for its usage. These are located in under each MCU name.

# Adding a new MCU

There is alot of housekeeping which needs to be performed when adding a MCU. The overall structure of the project is as follows:

1. Each MCU contains a template, with a bare minimum example of a flashing LED. Located in templates/mcuname
2. The MCU library is located in lib/mcuname. This library is compiled by a local makefile which creates a statically linked library. The directory structure can be copied from lib/stm32f767zi/. Relevent documentation for develpment can be found in lib/mcuname/doc.

## Documentation

Doxygen is used for documentation. The setup process for a new MCU is a pain, but it works well when done.

1. Copy *doxy* and *DoxygenLayout.xml* from lib/stm32f767zi/doc/doxygen, place in new MCU directory with same structure.
2. Assuming the new MCU has the same directory sturcture the *doxy* file can remain the same.
3. Open *DoxygenLayout.xml*, there are lines which define the tabs visable at the top of the page
```
<tab type="user" url="../../../../common/doc/doxygen/html/index.html" visible="yes" title="Commo    n"/>
```
There needs to be one of lines for each of the MCU *except* the current MCU directory which containes the file. Update this file as neccessary. **This needs to be repeated for each MCU DoxyLayout.xml file, the common DoxyLayout and main Doxylayout** The total number of files updated should equal the number of MCU's + 2.

4. Change the index.html link to be the title of the MCU libray.

```
<tab type="user" url="index.html" title="STM32F767ZI"/>
```

## Template Directory

The template directory Makefile should contain a variable `GLOB_LIB`, this variable is to be updated automatically by the config scripts. It can be placed in this makefile as a placeholder. See the stm32f767zi template for example.

The same variable name is located in `.ycm_extra_conf.py`. This should also be added / updated. The include paths, src pathes may also need to be changed.

## Common Libraries

The common libraries should be able to be compiled for each of the target MCUs. This means that a new Makefile recipe should be added to the makefile, which gets appended to the all recipe. This recipe should compile the common libraries to a static library.

## Tools
Two scripts, `tools/config.sh' and `tools/noconfig.sh' need to be updated. These scripts update the `GLOB_LIB` variables. Follow the examples in the directory.

## Main Makefile

The main makefile should compile all static libries for all MCU, generate all documentation, and compile the template programs. This means making new clean and build targets for the MCU, adding the documentation build and clean and adding to the templates. The new MCU targets need to be added to build and clean recipes.

# Testing

This document outlines which modules components have and haven't been tested.

## STM32F767

Currently being developed on a STM32Nucleo144 board.

### GPIO

Tested:

- GPIO_pinSetup(), pinWrite(), pinToggle() and pinSet() for digital output on
    - PB0, PB7 and PB14
- GPIO_pinSetup(), pinRead() for digital input on
    - PC13
- GPIO_pinSetup() for USART on: 
   - PD8 and PD9.
- Rising edge and falling edge interrupts on PC13

### USART

- USART_setup(), USART_put() and USART_get() on:
    - USART3C (PD8 and PD9)

### RCC

- RCC_clockEnable() for: 
    - GPIOB, GPIOC GPIOD, USART3, SYSCFG.
