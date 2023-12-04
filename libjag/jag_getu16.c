#include <buffer.h>

int
jag_getu16(void *b, size_t offset, size_t buflen, uint16_t *out)
{
	uint8_t *buffer = b;  
	if (offset > (SIZE_MAX - 2) || (buflen - offset) < 2) { 
		return -1; 
	}
	*out = ((buffer[offset] & 0xff) << 8) |
		(buffer[offset + 1] & 0xff);
	return 0;
}
