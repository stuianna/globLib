#! /bin/bash

#This script sets the glob_lib variable in each makefile to the current working directory.

CURDIR=$(pwd)
PREFIX='GLOB_LIB='
GLOBPATH="$PREFIX $CURDIR"

sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' ./ATTINY13A/templates/project_template/Makefile
sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' ./STM32F103CB/templates/stlink_project_template/Makefile
sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' ./STM32F103CB/templates/bootloader_project_template/Makefile
sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' ./STM32F103CB/usb_bootloader/Makefile


