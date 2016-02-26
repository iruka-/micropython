include ../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include ../py/py.mk

CROSS_COMPILE = p32-

INC += -I.
INC += -I..
INC += -I/usr/share/pinguino-11/p32/include/non-free/
INC += -I$(BUILD)

#CFLAGS_CORTEX_M4 = -mthumb -mtune=cortex-m4 -mabi=aapcs-linux -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fsingle-precision-constant -Wdouble-promotion

#CFLAGS_PIC32MX = -march=24kc -mips16 -minterlink-mips16
CFLAGS_PIC32MX = -march=m4k -EL -msoft-float 
#CFLAGS_PIC32MX = -march=24kc
#CFLAGS = $(INC) -Wall -Werror -ansi -std=gnu99 -nostdlib $(CFLAGS_CORTEX_M4) $(COPT)
CFLAGS = $(INC) -Wall -Werror -ansi -std=gnu99 $(CFLAGS_PIC32MX) -mips16 -minterlink-mips16 $(COPT)

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
LIBS = -lc

SRC_C = \
	main.c \
	syscalls.c \

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
	p32-objdump -d $@ >main32.lss

include ../py/mkrules.mk

$(BUILD)/crt0.o : crt0.S
	$(CC) $(INC) $(CFLAGS_PIC32MX) -c $< -o $@

#
