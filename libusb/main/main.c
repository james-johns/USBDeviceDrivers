
#include <stdlib.h>
#include <stdio.h>
#include <libusb.h>

#define VENDOR_ID 0x09ca
#define PRODUCT_ID 0x5544

const char digit[] = { 
0x3F, 0x06, 0x5B, 0x4F, 
0x66, 0x6D, 0x7D, 0x07, 
0x7F, 0x6F };

libusb_context *initUSBContext(libusb_context *context)
{
	int error = libusb_init(&context);
	if (error < 0) {
		fprintf(stderr, "Error: libusb_init returned %d", error);
		return NULL;
	}
	libusb_set_debug(context, 3);
	return context;
}

void destroyUSBContext(libusb_context *context)
{
	libusb_exit(context);
}

void printDeviceInfo(libusb_device *device)
{
	int error;
	struct libusb_device_descriptor desc;
	error = libusb_get_device_descriptor(device, &desc);
	if (error < 0) {
		fprintf(stderr, "libusb_get_device_descriptor returned %d\n", error);
		return;
	}


	printf("Number of confirgurations: %d\n", desc.bNumConfigurations);
	printf("Device Class: %d\n", desc.bDeviceClass);
	printf("VendorID: %d\n", desc.idVendor);
	printf("ProductID: %d\n", desc.idProduct);

	struct libusb_config_descriptor *config = NULL;
	libusb_get_config_descriptor(device, 0, &config);
	printf("Interfaces: %d\n", config->bNumInterfaces);

	const struct libusb_interface *interface = NULL;
	const struct libusb_interface_descriptor *interfaceDesc = NULL;
	const struct libusb_endpoint_descriptor *endpoint = NULL;
	int i, j, k;
	for (i = 0; i < config->bNumInterfaces; i++) {
		printf("\nInterface %d\n", i);
		interface = &config->interface[i];
		printf("\tAlternate Settings: %d\n", interface->num_altsetting);
		for (j = 0; j < interface->num_altsetting; j++) {
			interfaceDesc = &interface->altsetting[j];
			printf("\t\tInterface number %d\n", interfaceDesc->bInterfaceNumber);
			printf("\t\tNumber of End Points %d\n", interfaceDesc->bNumEndpoints);
			for (k = 0; k < interfaceDesc->bNumEndpoints; k++) {
				endpoint = &interfaceDesc->endpoint[k];
				printf("\t\t\tDescriptor Type %d\n", endpoint->bDescriptorType);
				printf("\t\t\tEndPoint Address %d\n", endpoint->bEndpointAddress);
			}
		}
        }
}

void setPortValue(libusb_device_handle *device, unsigned char port, unsigned char value)
{
	char *command = malloc(sizeof(char)*9);
	snprintf(command, 9, "@00P%1X%02X\r", (unsigned int)port, (unsigned int)value);
	int written = 0;
	libusb_bulk_transfer(device, (1 | LIBUSB_ENDPOINT_OUT), command, 8, &written, 0);
	if (written == 8)
		fprintf(stderr, "Sent command %s\n", command);
	else
		fprintf(stderr, "Failed to send command\n");
	libusb_bulk_transfer(device, (2 | LIBUSB_ENDPOINT_IN), command, 4, &written, 0);
	if (written == 4)
		fprintf(stderr, "Returned %s\n", command);
	else
		fprintf(stderr, "No return value\n");
	free(command);
}

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
	int count = 0;
	for (j = 0; j < 10000; j++) {
		for (k = 0; k < 60; k++) {
			for (i = 0; i < 4; i++) {
				setPortValue(device, 0, 1 << i);
				setPortValue(device, 2, digit[count%10]);
				usleep(((1000000/60)/4));
			}
		}
		count ++;
	}
	libusb_release_interface(device, 1);
	libusb_close(device);

	destroyUSBContext(context);
	return 0;
}
