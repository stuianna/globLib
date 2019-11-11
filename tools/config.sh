#! /bin/bash

# This script sets the glob_lib variable in each makefile to the current working directory.
# This script is called by makefile recipe "all":

CURDIR=$(pwd)
PREFIX='GLOB_LIB='
GLOBPATH="$PREFIX$CURDIR"

sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' template/stm32f767zi/Makefile
sed -i '/'"$PREFIX"'/c\'"$GLOBPATH"'' template/stm32f767zi/.ycm_extra_conf.py


