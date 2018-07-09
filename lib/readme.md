# Lib

This directory contains source code relevant to each module. It as organised as follows

### Common

Library modules designed to be compatible with any MCU. This folder should be
automatically included in each MCU template directory's makefile.

### Examples

Single files which contain a complete compilable version of a library feature. Each
example should demonstrate the main features of a library. A file can be copied into
a source directory, compiled and flashed to a target with minimal or no modificaiton.

The Doxygen documentation also references this directory to provide code snips for each
module.

### MCU Specific Modules

A single directory for each MCU supported. It should contain all source code,
definitions, linker scripts, common makefile elements for a given unit. This directory 
is also referenced by a given target's makefile.

Functions, enumerations and macro definitions should be almost identical between different
MCUs. This allows for integration with libraries contained in common.

**TODO**
New MCU Template
