# Introduction

This library contains low level modules designed to interface with ATTINY13A perihperals.
These modules are designed to be compatible with other low level common libraries and 
microcontroller modules as demonstrated on the [mainpage](https://github.com/stuianna/globLib).

Development has been tested on Ubuntu 16.04LTS and 18.04LTS.

# Installation

## Toolchain

The [avr-gcc](https://gcc.gnu.org/wiki/avr-gcc) is used to devolop firmware for this device. It can be installed from the 
package manager using:

```
sudo apt-get update
sudo apt-get install gcc-avr
```

Installation can be checked with 

```
avr-gcc -v
```

Which should print something like this (or newer)

```
gcc version 4.9.2 (GCC)
```

Additionally, [avr-libc] (https://www.nongnu.org/avr-libc/) is required. It can be installed with:

```
sudo apt-get install avr-libc
```

## USBASP

[USBASP](https://www.fischl.de/usbasp/) provides a hardware interface between the PC and MCU.

USBASP has been found to work without any further configuration. It can be connected to the MCU as follows:

|USBASP         |ATTINY13A          |
|---------------|:-----------------:|
|MOSI (PIN1)    |MOSI (PIN 5)       |
|RST  (PIN 5)   |RST  (PIN 1)       |
|SCK  (PIN 7)   |SCK  (PIN 7)       |
|MISO (PIN 9)   |MISO (PIN 6)       |

The USBASP can provide power to the device (through pins 2 and 10) only when flashing firmware. This means that an external power supply (3.3V - 5V) should be used to
run the application.

## Dialout

The user needs to be a member of group "dialout" to access the tty ports. This can be done with

```
sudo adduser $USER dialout
```

# Usage

A project template is contained in the templates directory with instructions for
its use.

## Doxygen

Doxygen is used to generate documentation for this and all modules. It can be
viewed by typing **make libdoc** from the project tempate directory. Should 
Doxygen be used to create further project documentation it should be installed
with

``` 
sudo apt-get install doxygen
sudo apt-get install graphviz
```
