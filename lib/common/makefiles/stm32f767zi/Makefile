# Directory Structure
RELPATH=../../
INCDIR=$(RELPATH)inc
LIBDIR=$(RELPATH)lib
SRCDIR=$(RELPATH)src
OBJDIR=$(RELPATH)obj
LIBMCU=stm32f767
LIBNAME=libcommon_stm32f767zi.a 

# Extra source and header for printf library
INCDIR+=$(RELPATH)printf
CSOURCES+=$(RELPATH)printf/printf.c

# Define the processor family
DEFS+= -DSTM32F767xx

# C compilation flags
CFLAGS= -Wall -Wextra -Os -fno-common -ffunction-sections -fdata-sections -std=c99 -fno-exceptions -g

# C++ compilation flags
CXXFLAGS= -Wall -Wextra -Os -fno-common -ffunction-sections -fdata-sections -std=c++11 -fno-exceptions -g

# MCU FLAGS -> These can be found by sifting through openocd makefiles
# Shouldn't need to be changed over the stm32f1xx family
MCFLAGS=-mcpu=cortex-m7 -mthumb -mlittle-endian -mfloat-abi=hard -mfpu=fpv5-sp-d16

# GNU ARM Embedded Toolchain
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
A2L=arm-none-eabi-addr2line

# Find source files
CSOURCES+=$(shell find -L $(SRCDIR) $(LIBDIR) -name '*.c')
CPPSOURCES+=$(shell find -L $(SRCDIR) $(LIBDIR) -name '*.cpp')

CSOURCES_ROOT=$(patsubst $(RELPATH)%,%,$(CSOURCES))
CPPSOURCES_ROOT=$(patsubst $(RELPATH)%,%,$(CPPSOURCES))

# Find header directories
INC=$(shell find -L $(INCDIR) -name '*.h' -exec dirname {} \; | uniq)
INC+=$(shell find -L $(INCDIR) -name '*.hpp' -exec dirname {} \; | uniq)
INCLUDES=$(INC:%=-I%)

CFLAGS += -c $(MCFLAGS) $(DEFS) $(INCLUDES)
CXXFLAGS += -c $(MCFLAGS) $(DEFS) $(INCLUDES)

AOBJECTS = $(patsubst %,obj/%,$(ASOURCES))
COBJECTS = $(patsubst %,$(RELPATH)obj/%,$(CSOURCES_ROOT))
CPPOBJECTS = $(patsubst %,$(RELPATH)obj/%,$(CPPSOURCES_ROOT))

OBJECTS=$(AOBJECTS:%.s=%.o) $(COBJECTS:%.c=%.o) $(CPPOBJECTS:%.cpp=%.o)
OUT_FILES=$(notdir $(OBJECTS))
OUT_PATH=$(patsubst %,$(OBJDIR)/$(LIBMCU)/%,$(OUT_FILES))

# Build Rules
.PHONY: all release debug clean flash erase

all: pack

pack: $(OBJECTS)
	@$(AR) -rc $(LIBDIR)/$(LIBNAME) $(OUT_PATH)
	@echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Archived:\033[0m" $(LIBDIR)/$(LIBNAME)

$(OBJDIR)/%.o: $(RELPATH)%.cpp
	@mkdir -p $(OBJDIR)/$(LIBMCU)/
	@$(CXX) $(CXXFLAGS) $< -o $(OBJDIR)/$(LIBMCU)/$(notdir $@)
	@echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Compiled:\033[0m" $<

$(OBJDIR)/%.o: $(RELPATH)%.c
	@mkdir -p $(OBJDIR)/$(LIBMCU)/
	@$(CC) $(CFLAGS) $< -o $(OBJDIR)/$(LIBMCU)/$(notdir $@)
	@echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Compiled:\033[0m" $<

clean:
	@rm -rf $(OBJDIR) $(LIBDIR)/$(LIBNAME)

print-%  : ; @echo $* = $($*)

