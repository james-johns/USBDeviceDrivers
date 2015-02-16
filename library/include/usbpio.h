

#ifndef _USBPIO_H_
#define _USBPIO_H_

#include <stdio.h>

typedef FILE USBPIODevice;

USBPIODevice *openDevice();
void closeDevice(USBPIODevice *device);

void setPortDirection(USBPIODevice *device, unsigned char port, unsigned char direction);
unsigned char getPortDirection(USBPIODevice *device, unsigned char port);

void setPortValue(USBPIODevice *device, unsigned char port, unsigned char value);
unsigned char getPortValue(USBPIODevice *device, unsigned char port);

#endif

