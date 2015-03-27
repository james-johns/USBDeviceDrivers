
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
	libusb_device_handle *device = openKeypadDevice(context);

	if (device == NULL) {
		perror("Device not opened");
		destroyUSBContext(context);
		return -1;
	}
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
	closeKeypadDevice(device);

	destroyUSBContext(context);
	return 0;
}
