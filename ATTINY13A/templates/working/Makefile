#Name of the project
TARGET = main

#Uncomment config options to include features
#GPIO_ISR = -DGPIO_ISR 	#Interrupts for GPIO


CFLAGS =  -std=gnu11 -Wall -ffunction-sections -fdata-sections -Os -fshort-enums 
LFLAGS = --gc-sections #Linker arguments should be comma separated

GLOB_LIB = /home/stuart/Dropbox/git/globLib

### 
#Shouldn't need to change from here down
###

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
LIB_DIR= $(GLOB_LIB)/ATTINY13A/lib
#COMMON_DIR=$(GLOB_LIB)/lib/common

DEVICE      = attiny13a
AVR_DEVICE = t13
CLOCK      = 250000
PROGRAMMER = -c usbasp

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS += $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS)) $(patsubst %.c,%.o,$(wildcard $(COMMON_DIR)/*.c)) $(patsubst %.c,%.o,$(wildcard $(LIB_DIR)/*.c))
INC = -I$(COMMON_DIR)/ -I$(LIB_DIR)/

# for ATTiny13a
# see http://www.engbedded.com/fusecalc/
# If the clock settings fuse is changed, the ... should be updated so clock timings are correct
# Current settings:
# 	9.6MHz clock
# 	Internally divide clock by 8 (Can be modified by software)	
# 	Preserve EEPROM through chip erase
# 	Serial programming enables (MUST BE LIKE THIS)
# 	Reset pin enabled
# 	No Brown out detection
# 	Debug wire disabled
# 	Self programming disabled
FUSES   = -U lfuse:w:0x2a:m -U hfuse:w:0xff:m

AVRDUDE = avrdude $(PROGRAMMER) -p $(AVR_DEVICE)
COMPILE = avr-gcc $(CFLAGS) $(GPIO_ISR) -Wl,$(LFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)


# symbolic targets:
all:	$(TARGET).hex

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(COMPILE) $(INC) -c $< -o $@

.c.o:
	@$(COMPILE) -c $< -o $@

.S.o:
	@$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	@$(COMPILE) -S $< -o $@

flash:	all
	@$(AVRDUDE) -U flash:w:$(BIN_DIR)/$(TARGET).hex:i

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(OBJ_DIR)
	@rm -f  $(OBJECTS)

# file targets:
$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	@$(COMPILE) $(INC) -o $(BIN_DIR)/$(TARGET).elf $(OBJECTS) 

$(TARGET).hex: $(TARGET).elf
	@rm -f $(BIN_DIR)/$(TARGET).hex
	@avr-objcopy -j .text -j .data -O ihex $(BIN_DIR)/$(TARGET).elf $(BIN_DIR)/$(TARGET).hex
	@avr-size --format=avr --mcu=$(DEVICE) $(BIN_DIR)/$(TARGET).elf

# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	$(BIN_DIR)/$(TARGET).elf
	avr-objdump -d $(BIN_DIR)/$(TARGET).elf

doco:
	@rm -f doc/index.html
	@doxygen conf/doxy
	@ln -sfr doc/html/index.html doc/index.html

view:
	xdg-open doc/index.html
