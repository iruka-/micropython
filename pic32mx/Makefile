include ../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include ../py/py.mk

CROSS_COMPILE = p32-
OBJCOPY	 = p32-objcopy


#PINGUINO_HOME = C:\pinguino-11
PINGUINO_HOME  = /usr/share/pinguino-11
PINGUINO_INC   = $(PINGUINO_HOME)/p32/include

INC += -I.
INC += -I..
INC += -I$(PINGUINO_INC)/pinguino/core/
INC += -I$(PINGUINO_INC)/non-free/
INC += -I$(BUILD)

PROC			= 32MX250F128B
DEFINES		    = -D__PIC32MX__ -D__$(PROC)__ -DGENERIC32MX250F128

CFLAGS_PIC32MX  = -march=m4k -EL -msoft-float 

CFLAGS  = $(INC) -Wall -Werror -ansi -std=gnu99 $(CFLAGS_PIC32MX)
CFLAGS += -mips16 -minterlink-mips16 $(COPT) $(DEFINES)
#DEBUG = 1

#Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -Os -DNDEBUG
endif

#LDFLAGS = -nostdlib -T stm32f405.ld -Map=$@.map --cref
LDFLAGS  = -L/usr/share/pinguino-11/compilers/p32/mips-elf/lib/soft-float/el/
LDFLAGS += -nostdlib -T 32MX270F256B.ld -T elf32pic32mx.x
LDFLAGS += -Map=$@.map --cref


LIBS  = $(PINGUINO_HOME)/p32/obj/non-free/$(PROC).o
LIBS += -lc

SRC_C = \
	main.c \
	syscalls.c \
	led.c \
	util.c \
	fifo.c \
	serial1.c \
	interrupt.c \


#	printf.c \
	string0.c \
	malloc0.c \
	gccollect.c \


SRC_S = \
	crt0.s \
#	startup_stm32f40xx.s \
#	gchelper.s \

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o) $(SRC_S:.s=.o))

all: $(BUILD)/firmware.elf

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@
	$(OBJCOPY) -O ihex $@ build/main32.hex
	p32-objdump -d $@ > build/main32.lss

dump:
	hex2dump build/main32.hex > build/main32.dump

include ../py/mkrules.mk

$(BUILD)/crt0.o : crt0.S
	$(CC) $(INC) $(CFLAGS_PIC32MX) -c $< -o $@

#
