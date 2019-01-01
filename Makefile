#+---------------------------------------------------------------------------
#
# Copyright © 2010 Anton Gusev aka AHTOXA (HTTP://AHTOXA.NET)
#
# File: makefile
#
# Contents: makefile to build arm Cortex-M3 software with gcc
#
#----------------------------------------------------------------------------

############# program name
TARGET = main

# Processor frequency.
F_CPU = 16000000

# fuses for mega88 E:F9, H:DF, L:D6 - external quartz

MCU = atmega88
CHIP = __AVR_ATmega88__
AVRDUDE_FLAGS = -c usbasp-clone -p m88

# program version
VER_MAJOR = 0
VER_MINOR = 1

TOOL ?= /home/luser/projects_ext/avr8-gnu-toolchain-linux_x86_64/bin/avr-

# compile options

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
OPT = 2

# Debugging format.
DEBUG = 

USE_LTO = 

#defines
DEFS = -DVER_MAJOR=$(VER_MAJOR)
DEFS += -DVER_MINOR=$(VER_MINOR)
DEFS += -D__AVR_ATmega88__=1

###########################################################
# common part for all my cortex-m3 projects
###########################################################

BASE = .
CC = $(TOOL)gcc
CXX = $(TOOL)g++
LD = $(TOOL)g++
AS = $(CC) -x assembler-with-cpp
OBJCOPY = $(TOOL)objcopy
OBJDUMP = $(TOOL)objdump
SIZE = $(TOOL)size -d
RM = rm -f
RMA = rm -rf
CP = cp
MD = mkdir

# dirs
SRCDIR = $(BASE)/src
LIBDIR = $(BASE)/lib
OBJDIR = $(BASE)/obj
EXEDIR = $(BASE)/bin
LSTDIR = $(BASE)/lst
LDDIR = $(BASE)/ld
BAKDIR = $(BASE)/bak

#files
HEX = $(EXEDIR)/$(TARGET).hex
BIN = $(EXEDIR)/$(TARGET).bin
ELF = $(EXEDIR)/$(TARGET).elf
MAP = $(LSTDIR)/$(TARGET).map
LSS = $(LSTDIR)/$(TARGET).lss
OK = $(EXEDIR)/$(TARGET).ok

# scmRTOS dir
#SCMDIR = ../scmRTOS
#COMMON = ../SamplesCommon
SCMDIR=
COMMON=

# source directories (all *.c, *.cpp and *.S files included)
DIRS := $(SRCDIR)
DIRS += /home/luser/projects_ext/avr8-gnu-toolchain-linux_x86_64/avr/include/
DIRS += $(LIBDIR)
DIRS += $(LIBDIR)/pid

# includes
INCS := $(patsubst %, -I "%", $(DIRS))

# individual source files
SRCS := 

#calc obj files list
OBJS := $(SRCS)
OBJS += $(wildcard $(addsuffix /*.cpp, $(DIRS)))
OBJS += $(wildcard $(addsuffix /*.c, $(DIRS)))
OBJS += $(wildcard $(addsuffix /*.S, $(DIRS)))
OBJS := $(notdir $(OBJS))
OBJS := $(OBJS:.cpp=.o)
OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.S=.o)
OBJS := $(patsubst %, $(OBJDIR)/%, $(OBJS))

#files to archive
ARCFILES = \
$(SRCDIR) \
$(LDDIR) \
$(SCMDIR) \
$(BASE)/Makefile \
$(BASE)/.cproject \
$(BASE)/.project

# flags
FLAGS = -mmcu=$(MCU)
FLAGS += -DF_CPU=$(F_CPU)
FLAGS += $(INCS)
FLAGS += -MD
FLAGS += $(DEFS)
#FLAGS += -Wa,-adhlns=$(addprefix $(LSTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))
#FLAGS += -c -fno-common -mlittle-endian -fshort-enums
#FLAGS += -nostdlib -nostdinc
# FLAGS+=-Wl,-Ttext,0x20000000 -Wl,-e,0x20000000

ifeq ($(DEBUG),1)
 	MSG_MODE=--- debug mode
	FLAGS += -g
else 
	MSG_MODE=--- release mode
	FLAGS += -O$(OPT)
endif

# FLAGS += -Wl,-Ttext,0x20000000 -Wl,-e,0x20000000

AFLAGS = $(FLAGS)

CFLAGS = $(FLAGS)
CFLAGS += -std=c99
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wextra
#CFLAGS += -Wimplicit -Wcast-align -Wpointer-arith
#CFLAGS += -Wshadow -Wcast-align -Wnested-externs -pedantic

CXXFLAGS = $(FLAGS)
CXXFLAGS += -std=c++98
CXXFLAGS += -fno-exceptions -fno-rtti
CXXFLAGS += -ffunction-sections -fdata-sections
#CXXFLAGS += -fno-threadsafe-statics
#CXXFLAGS += -funsigned-bitfields -fshort-enums
CXXFLAGS += -Wall -Wextra
#CXXFLAGS += -Winline
#CXXFLAGS += -Wpointer-arith -Wredundant-decls
#CXXFLAGS += -Wshadow -Wcast-qual -Wcast-align -pedantic

LD_FLAGS = -mmcu=$(MCU)
#LD_FLAGS += -nostartfiles
LD_FLAGS += -L$(LDDIR)
LD_FLAGS += -Wl,-Map="$(MAP)",--cref
LD_FLAGS += -Wl,--gc-sections

ifeq ($(USE_LTO),1)
	CFLAGS += -flto
	CXXFLAGS += -flto
	LD_FLAGS += -flto $(OPTIMIZE)
endif


.SILENT :

.PHONY: all dmode start dirs build clean program archive

############# targets
all : dmode start dirs $(ELF) $(BIN) $(HEX) $(LSS) $(OK)
build: clean all


# Display compiler version information.
gccversion : 
	@$(CC) --version


dmode:
	@echo $(MSG_MODE)

start:
	@echo --- building $(TARGET)
	
$(LSS): $(ELF) Makefile
	@echo --- making asm-lst...
	@$(OBJDUMP) -dC $(ELF) > $(LSS)

$(OK): $(ELF)
	@$(SIZE) $(ELF)
	@echo "Errors: none"

$(ELF): $(OBJS) Makefile
	@echo --- linking...
	$(LD) $(OBJS) $(LIBS) $(LD_FLAGS) -o "$(ELF)"

$(HEX): $(ELF)
	@echo --- make hex...
	@$(OBJCOPY) -O ihex $(ELF) $(HEX)

$(BIN): $(ELF)
	@echo --- make binary...
	@$(OBJCOPY) -O binary $(ELF) $(BIN)

program: $(HEX)
	@echo "Programming with AVRDUDE"
	avrdude $(AVRDUDE_FLAGS) -U flash:w:$<

VPATH := $(DIRS)

$(OBJDIR)/%.o: %.cpp Makefile
	@echo --- compiling CPP files $<...
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.c Makefile
	@echo --- compiling C files $<...
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.S Makefile
	@echo --- assembling $<...
	$(AS) -c $(AFLAGS) -o $@ $<

dirs: $(OBJDIR) $(EXEDIR) $(LSTDIR) $(BAKDIR)

$(OBJDIR):
	-@$(MD) $(OBJDIR)

$(EXEDIR):
	-@$(MD) $(EXEDIR)

$(LSTDIR):
	-@$(MD) $(LSTDIR)

$(BAKDIR):
	-@$(MD) $(BAKDIR)

clean:
	-@$(RM) $(OBJDIR)/*.d 2>/dev/null
	-@$(RM) $(OBJDIR)/*.o 2>/dev/null
	-@$(RM) $(LSTDIR)/*.lst 2>/dev/null
	-@$(RM) $(ELF)
	-@$(RM) $(HEX)
	-@$(RM) $(LSS)
	-@$(RM) $(MAP)
	-@$(RMA) $(OBJDIR)
	-@$(RMA) $(EXEDIR)
	-@$(RMA) $(LSTDIR)
	-@$(RMA) $(BAKDIR)

archive:
	@echo --- archiving...
	7z a $(BAKDIR)/$(TARGET)_`date +%Y-%m-%d,%H-%M-%S` $(ARCFILES)
	@echo --- done!

# dependencies
ifeq (,$(findstring build,$(MAKECMDGOALS)))
ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring dirs,$(MAKECMDGOALS)))
-include $(wildcard $(OBJDIR)/*.d)
endif
endif
endif
