

###
# Build environment setup
###
PROJECT:=libusbpio.a

CROSS_COMPILE?=

CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)gcc

CFLAGS+=-I./include/
CCFLAGS+=
LDFLAGS+= -lc 

Q:=-@

obj-y:=

# sub-directory makefiles
include main/Makefile

###
# main build targets
###
.PHONY:all clean FORCE

all: $(PROJECT)

clean:
	$(Q)rm $(PROJECT)
	$(Q)rm $(obj-y)

libusbpio.a: $(obj-y)
	$(Q)echo " [AR] $@"
	$(Q)$(AR) -cr $@ $<

###
# Intermediary build targets
###
.c.o:
	$(Q)echo " [CC]  $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(Q)echo " [C++] $@"
	$(Q)$(CC) $(CCFLAGS) -c -o $@ $<



