

#include <usbpio.h>


USBPIODevice *openDevice()
{
	FILE *device = fopen("/dev/ttyACM0", "rw");
	if (device == NULL)
		perror("openDevice");
	return device;
}
void closeDevice(USBPIODevice *device)
{
	fclose(device);
}

void setPortDirection(USBPIODevice *device, unsigned char port, unsigned char direction)
{
	unsigned int readBytes;
	char *str = "@00D000\r";
	unsigned char dirUpper = ((direction & 0xF0) >> 4);
	unsigned char dirLower = (direction & 0x0F);
	str[4] = ((port >= 0x0A) ? ('A' + (port - 10)) : ('0' + port));
	str[5] = ((dirUpper >= 0x0A) ? ('A' + (dirUpper - 10)) : ('0' + dirUpper));
	str[6] = ((dirLower >= 0x0A) ? ('A' + (dirLower - 10)) : ('0' + dirLower));
	fwrite(str, 1, 8, device); /* Send command */

	readBytes = fread(str, 1, 4, device); /* Read return status */
	if ((readBytes != 4) || (str[0] != '!')) {
		str[readBytes] = '\0';
		fprintf(stderr, "setPortDirection: Failed for unknown reason. Got \'%s\'\r\n", str);
	}
}
unsigned char getPortDirection(USBPIODevice *device, unsigned char port)
{
	unsigned int readBytes;
	unsigned char toRet = 0x00;
	char *str = "@00D0?\r";
	str[4] = ((port >= 0x0A) ? ('A' + (port - 10)) : ('0' + port));
	fwrite(str, 1, 7, device); /* Send command */

	readBytes = fread(str, 1, 6, device); /* Read return status */
	if ((readBytes != 6) || (str[0] != '?')) {
		str[readBytes] = '\0';
		fprintf(stderr, "getPortDirection: Failed for unknown reason. Got \'%s\'\r\n", str);
	}
	if (str[3] >= 'A')
		toRet |= (((10 + (str[3] - 'A')) << 4) & 0xF0);
	else
		toRet |= (((str[3] - '0') << 4) & 0xF0);
	if (str[4] >= 'A')
		toRet |= ((10 + (str[4] - 'A')) & 0x0F);
	else
		toRet |= ((str[4] - '0') & 0x0F);
	return toRet;
}

void setPortValue(USBPIODevice *device, unsigned char port, unsigned char value)
{
	unsigned int readBytes;
	char *str = "@00P000\r";
	unsigned char valUpper = ((value & 0xF0) >> 4);
	unsigned char valLower = (value & 0x0F);
	str[4] = ((port >= 0x0A) ? ('A' + (port - 10)) : ('0' + port));
	str[5] = ((valUpper >= 0x0A) ? ('A' + (valUpper - 10)) : ('0' + valUpper));
	str[6] = ((valLower >= 0x0A) ? ('A' + (valLower - 10)) : ('0' + valLower));
	fwrite(str, 1, 8, device); /* Send command */

	readBytes = fread(str, 1, 4, device); /* Read return status */
	if ((readBytes != 4) || (str[0] != '!')) {
		str[readBytes] = '\0';
		fprintf(stderr, "setPortValue: Failed for unknown reason. Got \'%s\'\r\n", str);
	}

}
unsigned char getPortValue(USBPIODevice *device, unsigned char port)
{
	unsigned int readBytes;
	unsigned char toRet = 0x00;
	char *str = "@00P0?\r";
	str[4] = ((port >= 0x0A) ? ('A' + (port - 10)) : ('0' + port));
	fwrite(str, 1, 7, device); /* Send command */

	readBytes = fread(str, 1, 6, device); /* Read return status */
	if ((readBytes != 6) || (str[0] != '?')) {
		str[readBytes] = '\0';
		fprintf(stderr, "getPortValue: Failed for unknown reason. Got \'%s\'\r\n", str);
	}
	if (str[3] >= 'A')
		toRet |= (((10 + (str[3] - 'A')) << 4) & 0xF0);
	else
		toRet |= (((str[3] - '0') << 4) & 0xF0);
	if (str[4] >= 'A')
		toRet |= ((10 + (str[4] - 'A')) & 0x0F);
	else
		toRet |= ((str[4] - '0') & 0x0F);
	return toRet;
}
