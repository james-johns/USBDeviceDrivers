

###
# Build environment setup
###
PROJECT:=usbdriver

CROSS_COMPILE?=

CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)gcc

CFLAGS+=-I/usr/include/libusb-1.0 -I./includes/
CCFLAGS+=
LDFLAGS+=-lc -lusb-1.0

Q:=-@

obj-y:=
keypad-obj-y:=

# sub-directory makefiles
include main/Makefile
include keypad/Makefile

###
# main build targets
###
.PHONY:all clean FORCE

all: $(PROJECT)

clean:
	$(Q)rm $(PROJECT)
	$(Q)rm $(obj-y)

$(PROJECT): $(obj-y) libkeypad.a FORCE
	$(Q)echo " [LD]  $@"
	$(Q)$(LD) -o $(PROJECT) $(obj-y) $(LDFLAGS) -L./ -lkeypad

libkeypad.a:$(keypad-obj-y) FORCE
	$(Q)echo " [AR]  $@"
	$(Q)$(AR) cr $@ $(keypad-obj-y)

###
# Intermediary build targets
###
.c.o:
	$(Q)echo " [CC]  $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(Q)echo " [C++] $@"
	$(Q)$(CC) $(CCFLAGS) -c -o $@ $<
