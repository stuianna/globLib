##Basic Project Configuration

#Name of the project
TARGET = STM32F1_USB_bootloader

#Absolute path the the globLib root directory
GLOB_LIB= 

##Compiler options

#C FLAGS
TGT_CFLAGS	+= -Wall -Wextra -Wl,--gc-sections,--start-group,--end-group -lc -lgcc -lnosys
TGT_CFLAGS	+= -Os -fno-common -ffunction-sections -fdata-sections --static -nostartfiles 
CSTD		+= -std=gnu99

#Linker flags (comma seperated)
TGT_LDFLAGS		+= -Wl,--gc-sections -ffunction-sections -fdata-sections -fno-common

###############################################################################
##Shouldn't need to change below here
###############################################################################

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

OPENCM3_DIR= $(GLOB_LIB)/STM32F103CB/libopencm3
LIB_DIR= $(GLOB_LIB)/STM32F103CB/lib
COMMON_DIR=$(GLOB_LIB)/common/lib
OOCD = openocd
OOCD_TARGET = stm32f1x
OOCD_INTERFACE = stlink-v2
LDSCRIPT= $(OPENCM3_DIR)/lib/stm32/f1/stm32f103xb.ld
CC = arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size
FP_FLAGS    ?= -msoft-float
ARCH_FLAGS  = -mthumb -mcpu=cortex-m3 $(FP_FLAGS) -mfix-cortex-m3-ldrd
TGT_CFLAGS	+= $(CSTD) $(ARCH_FLAGS)

SRCS := $(shell find $(SRC_DIR) -name '*.c') 
OBJS += $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS)) $(patsubst %.c,%.o,$(wildcard $(COMMON_DIR)/*.c)) $(patsubst %.c,%.o,$(wildcard $(LIB_DIR)/*.c))
INC = -I$(OPENCM3_DIR)/include -I$(COMMON_DIR)/ -I$(LIB_DIR)/ -I$(SRC_DIR)/ -DSTM32F1
LINC = -L$(OPENCM3_DIR)/lib -L$(COMMON_DIR)/ -L$(LIB_DIR)/ -L$(OBJ_DIR)

#Additional linker flahs
TGT_LDFLAGS		+= --static -nostartfiles 
TGT_LDFLAGS		+= -Wl,-Map=$(OBJ_DIR)/$(TARGET).map,--cref
LD_SC     += -T$(LDSCRIPT) $(ARCH_FLAGS) -ggdb3

#Used libraries
LDFLAGS     += $(LINC)
LIBNAME     = opencm3_stm32f1
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
LDLIBS      += -l$(LIBNAME)

###############################################################################
#Recipes
###############################################################################

#Helper macro to print out variable values -> use make print-EXAMPLE
print-%:
	@echo $*=$($*)

all: $(TARGET).elf $(TARGET).bin 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(INC) $(TGT_CFLAGS) -c $< -o $@

.c.o:
	@$(CC) $(INC) $(TGT_CFLAGS) -c $< -o $@

$(TARGET).elf $(TARGET).map : $(OBJS) $(LDSCRIPT)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(TGT_LDFLAGS) $(LD_SC) $(LDFLAGS)  $(OBJS) $(LDLIBS) -o $(BIN_DIR)/$(TARGET).elf 
	@$(SIZE) $(BIN_DIR)/$(TARGET).elf

$(TARGET).bin : $(BIN_DIR)/$(TARGET).elf
	@$(OBJCOPY) -Obinary $(BIN_DIR)/$(TARGET).elf $(BIN_DIR)/$(TARGET).bin

clean:
	@$(Q)$(RM) *.o *.d *.elf *.bin *.hex *.srec *.list *.map generated.* ${OBJS} ${OBJS:%.o:%.d}
	 @$(RM) $(patsubst %.c,%.d,$(wildcard $(COMMON_DIR)/*.c))
	 @$(RM) $(patsubst %.c,%.d,$(wildcard $(LIB_DIR)/*.c))
	 @$(RM) -rf $(OBJ_DIR)
	 @$(RM) -rf $(BIN_DIR)

flash: $(TARGET).flash

$(TARGET).flash: $(BIN_DIR)/$(TARGET).elf
	 @(echo "halt; program $(realpath $(*).elf) verify reset" | nc -4 localhost 4444 2>/dev/null) || \
	 	$(OOCD) -f interface/$(OOCD_INTERFACE).cfg \
		-f target/$(OOCD_TARGET).cfg \
		-c "program $(BIN_DIR)/$(TARGET).elf verify reset exit" \
		2> /dev/null
erase:
	@openocd -f interface/stlink-v2.cfg \
	-f target/stm32f1x.cfg \
	-c "init" \
	-c "halt" \
	-c "stm32f1x mass_erase 0" \
	-c "exit"

libdoc:
	@xdg-open $(GLOB_LIB)/STM32F103CB/doc/doxygen/html/index.html

#The following 'echo' lines pass variables to the doxygen config file when compiled.
#More can be passed using the same example

docs:
	@rm -f doc/index.html
	@(cat conf/doxy; \
	   	echo "PROJECT_NAME=$(TARGET)"; \
	   	echo "INPUT=$(LIB_DIR) $(COMMON_DIR) $(SRC_DIR) ."; \
	   	echo "EXAMPLE_PATH=$(GLOB_LIB)/examples") \
	   	| doxygen -
	@ln -sfr doc/html/index.html doc/index.html

viewdoc:
	@xdg-open doc/index.html

cleandoc:
	@rm -rf doc/html
	@rm -r doc/index.html

usbReset:
#This script resets the USB device if it exists, this saves disconnecting and reconnecting the device
	($(GLOB_LIB)/STM32F103CB/tools/usbResetstm32f1 $(GLOB_LIB))
