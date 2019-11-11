#! /bin/bash

# This script resets the GLOB_LIB directory in relevent makefiles. It is called by
# main makefile recipe make clean.

PREFIX='GLOB_LIB='

sed -i '/'"$PREFIX"'/c\'"$PREFIX"'' template/stm32f767zi/Makefile
