#! /bin/bash

# This script sets the glob_lib variable in each makefile to the current working directory.
# This script is called by makefile recipe "all":

CURDIR=$(pwd)
PREFIX='GLOB_LIB='
GLOBPATH="$PREFIX$CURDIR"

sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' template/stm32f767zi/bare/Makefile
#sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' template/stm32f767zi/bare/.ycm_extra_conf.py

sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' template/stm32f767zi/freeRTOS/Makefile


