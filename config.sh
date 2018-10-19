#! /bin/bash

CURDIR=$(pwd)
PREFIX='GLOB_LIB='
GLOBPATH="$PREFIX $CURDIR"

sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' ./ATTINY13A/templates/project_template/Makefile
sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' ./STM32F103CB/templates/stlink_project_template/Makefile
sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' ./STM32F103CB/templates/bootloader_project_template/Makefile


