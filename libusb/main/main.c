
#include <stdlib.h>
#include <stdio.h>
#include <libusb.h>

#include <keypad.h>

const char digit[] = { 
0x3F, 0x06, 0x5B, 0x4F, 
0x66, 0x6D, 0x7D, 0x07, 
0x7F, 0x6F };

int main(int argc, char **argv)
{
	libusb_context *context;
	context = initUSBContext(NULL);
	libusb_device_handle *device = libusb_open_device_with_vid_pid(context, VENDOR_ID, PRODUCT_ID);
	if (device == NULL) {
		fprintf(stderr, "Error finding device\n");
		destroyUSBContext(context);
		return -1;
	}
	printDeviceInfo(libusb_get_device(device));
	if (libusb_kernel_driver_active(device, 0) == 1) {
		fprintf(stderr, "Disabling kernel driver...");
		if (libusb_detach_kernel_driver(device, 0) == 0)
			fprintf(stderr, "Success\n");
		else {
			fprintf(stderr, "Failed!\n");
			destroyUSBContext(context);
			return -2;
		}
	}
	libusb_claim_interface(device, 1);
	int i, j, k;
	unsigned char keypad;
	int count = 0;

	setPortDirection(device, 0, 0x00);
	setPortDirection(device, 1, 0x00);
	setPortDirection(device, 2, 0x00);

	for (j = 0; j < 20; j++) {
		for (k = 0; k < 60; k++) {
			/* port 0 needs to be output to drive 7 seg */
			setPortDirection(device, 0, 0x00);
			for (i = 0; i < 4; i++) {
				setPortValue(device, 0, 1 << i);
				setPortValue(device, 2, digit[count%10]);
				usleep(((1000000/60)/4));
			}

			setPortValue(device, 2, 0x00);
			/* port 0 needs to be input to read keypad */
			setPortDirection(device, 0, 0xFF);
			for (i = 0; i < 4; i++) {
				setPortValue(device, 1, 1 << i);
				getPortValue(device, 0, &keypad);
			}
		}
		count ++;
	}
	libusb_release_interface(device, 1);
	libusb_close(device);

	destroyUSBContext(context);
	return 0;
}
