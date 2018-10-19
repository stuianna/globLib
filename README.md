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
can be accessed through [Github Pages](https://stuianna.github.io/globLib/) or by typing **make libdoc** from any of the project templates. A local copy of this repository needs to made before this documentation can be accessed.

# Installation / Usage

This whole repository should be cloned onto a local machine to function correctly.

 1. Clone this repository onto the local machine
 2. Run `chmod +x config.sh` to make the configuration file executable
 3. Execute the config file with `./config.sh`. This script sets the glob_lib variable in each Makefile to the current root directory. This should also be run if the main glob_lib folder is moved.

Each supported MCU has a templates containing a project template and specific 
instructions for its usage. These are located in under each MCU name.
