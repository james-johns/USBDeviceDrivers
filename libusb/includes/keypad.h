

#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#define VENDOR_ID 0x09ca
#define PRODUCT_ID 0x5544

libusb_context *initUSBContext(libusb_context *context);
void destroyUSBContext(libusb_context *context);

void printDeviceInfo(libusb_device *device);

void setPortDirection(libusb_device_handle *device, unsigned char port, unsigned char direction);
void getPortDirection(libusb_device_handle *device, unsigned char port, unsigned char *direction);

void setPortValue(libusb_device_handle *device, unsigned char port, unsigned char value);
void getPortValue(libusb_device_handle *device, unsigned char port, unsigned char *value);

#endif
