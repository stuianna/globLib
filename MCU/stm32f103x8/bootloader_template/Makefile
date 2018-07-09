#Name for the project goes here, this must be the name of the file with the main() function.
BINARY = blink

##Extra source code files can be put here, with a .o extension.
##OBJS= *.o 

##Absolute path to glob_lib root directory
GLOB_LIB= /home/stuart/Dropbox/Projects/glob_lib

BOOTLOADER='' #Load with usb

OPENCM3_DIR= $(GLOB_LIB)/lib/stm32f1x3/libopencm3
LIB_DIR= $(GLOB_LIB)/lib/stm32f1x3/lib
COMMON_DIR=$(GLOB_LIB)/lib/common

#This differes for loaded with the usb_bootloader or stlink
LDSCRIPT= $(OPENCM3_DIR)/lib/stm32/f1/stm32f103xb_usb.ld#Uncomment for USB flash
#LDSCRIPT= $(OPENCM3_DIR)/lib/stm32/f1/stm32f103xb.ld	#Uncomment for STLINK Flash


CSTD		?= -std=c99

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

ifneq ($(BOOTLOADER),) #This option is called for the USB bootloader
flash:
	($(GLOB_LIB)/lib/stm32f1x3/stm32f1_resetToBoot) #Set the device in boot mode
	dfu-util -a 0 -d 0483:df11 -s 0x08002000:leave -D $(BINARY).bin #Load new program
#	($(GLOB_LIB)/lib/stm32f1x3/usbResetstm32f1_dev $(GLOB_LIB))
endif

usbReset:
#This script resets the USB device if it exists, this saves disconnecting and reconnecting the device
	($(GLOB_LIB)/lib/stm32f1x3/usbResetstm32f1 $(GLOB_LIB))
	($(GLOB_LIB)/lib/stm32f1x3/usbResetstm32f1_dev $(GLOB_LIB))
