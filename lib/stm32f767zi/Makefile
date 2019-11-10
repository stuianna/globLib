# Directory Structure
INCDIR=inc
LIBDIR=lib
SRCDIR=src
OBJDIR=obj
LIBNAME=libstm32f767zi.a 

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
CSOURCES=$(shell find -L $(SRCDIR) $(LIBDIR) -name '*.c')
CPPSOURCES=$(shell find -L $(SRCDIR) $(LIBDIR) -name '*.cpp')

# Find header directories
INC=$(shell find -L $(INCDIR) -name '*.h' -exec dirname {} \; | uniq)
INC+=$(shell find -L $(INCDIR) -name '*.hpp' -exec dirname {} \; | uniq)
INCLUDES=$(INC:%=-I%)

CFLAGS += -c $(MCFLAGS) $(DEFS) $(INCLUDES)
CXXFLAGS += -c $(MCFLAGS) $(DEFS) $(INCLUDES)

AOBJECTS = $(patsubst %,obj/%,$(ASOURCES))
COBJECTS = $(patsubst %,obj/%,$(CSOURCES))
CPPOBJECTS = $(patsubst %,obj/%,$(CPPSOURCES))

OBJECTS=$(AOBJECTS:%.s=%.o) $(COBJECTS:%.c=%.o) $(CPPOBJECTS:%.cpp=%.o)

# Build Rules
.PHONY: all release debug clean flash erase

all: pack

memory: CFLAGS+=-g
memory: CXXFLAGS+=-g
memory: LDFLAGS+=-g -Wl,-Map=$(BINDIR)/$(PROJECT).map
memory:
	@echo -e "\033[0;32m[Top Memory Use]\033[0m"
	@$(NM) -A -l -C -td --reverse-sort --size-sort $(BINDIR)/$(BINELF) | head -n10 | cat -n

debug: CFLAGS+=-g3
debug: CXXFLAGS+=-g3
debug: LDFLAGS+=-g3
debug: release

pack: $(OBJECTS)
	@$(AR) -rc $(LIBDIR)/$(LIBNAME) $(OBJECTS)
	@echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Archived:\033[0m" $(LIBDIR)/$(LIBNAME)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $< -o $@
	@echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Compiled:\033[0m" $<

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $< -o $@
	@echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Compiled:\033[0m" $<

clean:
	@rm -rf $(OBJDIR) $(LIBDIR)/$(LIBNAME)

print-%  : ; @echo $* = $($*)

docs:
	@doxygen doc

viewdocs:
	$$BROWSER ./doc/html/index.html

