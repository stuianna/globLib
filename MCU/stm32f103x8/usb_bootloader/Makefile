#Name for the project goes here, this must be the name of the file with the main() function.
BINARY = usb_bootloader

##Extra source code files can be put here, with a .o extension.
##OBJS= *.o 

##Absolute path to glob_lib root directory
GLOB_LIB= /home/stuart/Dropbox/Projects/glob_lib

OPENCM3_DIR= $(GLOB_LIB)/lib/stm32f1x3/libopencm3
LIB_DIR= #$(GLOB_LIB)/lib/stm32f1x3/lib
COMMON_DIR=#$(GLOB_LIB)/lib/common
LDSCRIPT= $(OPENCM3_DIR)/lib/stm32/f1/stm32f103x8.ld


CSTD		?= -std=gnu99

#PRE-PROCESSOR FLAGS
TGT_CPPFLAGS	+= -MD
TGT_CPPFLAGS	+= -Wall -Wundef

#C FLAGS
TGT_CFLAGS	+= -Wextra -Wshadow -Wimplicit-function-declaration
TGT_CFLAGS	+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
TGT_CFLAGS	+= -fno-common -ffunction-sections -fdata-sections

#LINKER FLAGS
TGT_LDFLAGS		+= --static -nostartfiles
TGT_LDFLAGS		+= -Wl,-Map=$(*).map -Wl,--cref
TGT_LDFLAGS		+= -Wl,--gc-sections

#USED LIBRARIES
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

include $(GLOB_LIB)/lib/stm32f1x3/libopencm3.target.mk

erase:
	openocd -f interface/stlink-v2.cfg \
	-f target/stm32f1x.cfg \
	-c "init" \
	-c "halt" \
	-c "stm32f1x mass_erase 0" \
	-c "exit"
doc:
	xdg-open $(GLOB_LIB)/doc/libDoc.html

#This script resets the USB device if it exists, this saves disconnecting and reconnecting the device
flash:
	($(GLOB_LIB)/lib/stm32f1x3/usbResetstm32f1 $(GLOB_LIB))


