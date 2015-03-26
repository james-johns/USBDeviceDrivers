
#include <stdio.h>
#include <stdlib.h>
#include <libusb.h>

#include <keypad.h>



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

void setPortDirection(libusb_device_handle *device, unsigned char port, unsigned char direction)
{
	char *command = malloc(sizeof(char)*9);
	snprintf(command, 9, "@00D%1X%02X\r", (unsigned int)port, (unsigned int)direction);
	int written = 0;
	libusb_bulk_transfer(device, (1 | LIBUSB_ENDPOINT_OUT), command, 8, &written, 0);
	if (written != 8)
		fprintf(stderr, "Failed to send command\n");
	libusb_bulk_transfer(device, (2 | LIBUSB_ENDPOINT_IN), command, 4, &written, 0);
	if (written != 4)
		fprintf(stderr, "No return value\n");
	free(command);
}

void getPortDirection(libusb_device_handle *device, unsigned char port, unsigned char *direction)
{
	char *command = malloc(sizeof(char)*8);
	snprintf(command, 8, "@00D%1X?\r", (unsigned int)port);
	int written = 0;
	libusb_bulk_transfer(device, (1 | LIBUSB_ENDPOINT_OUT), command, 7, &written, 0);
	if (written != 7)
		fprintf(stderr, "Failed to send command\n");
	libusb_bulk_transfer(device, (2 | LIBUSB_ENDPOINT_IN), command, 6, &written, 0);
	if (written != 6)
		fprintf(stderr, "No return value\n");
	else
		fprintf(stderr, "Returned %s\n", command);

	unsigned int input;
	sscanf(command, "!00%2X\r", &input);
	*direction = (input & 0x000000FF);

	free(command);
}

void setPortValue(libusb_device_handle *device, unsigned char port, unsigned char value)
{
	char *command = malloc(sizeof(char)*9);
	snprintf(command, 9, "@00P%1X%02X\r", (unsigned int)port, (unsigned int)value);
	int written = 0;
	libusb_bulk_transfer(device, (1 | LIBUSB_ENDPOINT_OUT), command, 8, &written, 0);
	if (written != 8)
		fprintf(stderr, "Failed to send command\n");
	libusb_bulk_transfer(device, (2 | LIBUSB_ENDPOINT_IN), command, 4, &written, 0);
	if (written != 4)
		fprintf(stderr, "No return value\n");
	free(command);
}

void getPortValue(libusb_device_handle *device, unsigned char port, unsigned char *value)
{
	char *command = malloc(sizeof(char)*8);
	snprintf(command, 8, "@00P%1X?\r", (unsigned int)port);
	int written = 0;
	libusb_bulk_transfer(device, (1 | LIBUSB_ENDPOINT_OUT), command, 7, &written, 0);
	if (written != 7)
		fprintf(stderr, "Failed to send command\n");
	libusb_bulk_transfer(device, (2 | LIBUSB_ENDPOINT_IN), command, 6, &written, 0);
	if (written != 6)
		fprintf(stderr, "No return value\n");
	else
		fprintf(stderr, "Returned %s\n", command);

	unsigned int input;
	sscanf(command, "!00%2X\r", &input);
	*value = (input & 0x000000FF);

	free(command);
}

