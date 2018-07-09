# Introduction

GlobLib is combination of a hardware abstraction layer and common libraries which can be 
shared across multiple microcontroller units (MCU). It is designed to provide boilerplate,
cross-MCU compatible code implementation of the most commonly used features. A typical
library contains implementations for:
-   GPIO
-   USART
-   SPI
-   I2C
-   ADC
-   Timers - PWM / Input capture / Pulse width
-   Interrupts
-   Systick
-   RTC
-   Watchdog
-   Clock configuration
-   USB
-   Flash memory / EEPROM
-   Power modes

GlobLib focuses on proving functionality and portability for the most common features 
of each peripheral. It is not necessarily the fastest or best way of implementing any 
specific feature.

This collection also contains common libraries which can be used across different MCUs.

# Supported Hardware

### MCU

-   STM32F103 (full)

### IMU

-   MPU6050
-   LSM9DS1 (SPI interface only)

# Documentation

Doxygen http://www.stack.nl/~dimitri/doxygen/index.html is used to automatically
generate extensive documentation of this collection and its features. This document
can be accessed in doc/libDoc.html. A local copy of this repository needs to
made before this documentation can be accessed.

# Installation / Usage

This whole repository should be cloned onto a local machine to function correctly.

Each supported MCU has a root directory containing a project template and specific 
instructions for its usage. These are located in MCU/.

Example applications for different features are located in lib/examples, these are
also included in the Doxygen documentation.
