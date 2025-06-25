#include <buffer.h>
#include <limits.h>
#include <stdio.h>

int
jag_putu24(void *b, size_t offset, size_t buflen, uint32_t in)
{
	uint8_t *buffer = b + offset;
	if (offset > (SSIZE_MAX - 3) ||
	    ((ssize_t)buflen - (ssize_t)offset) < 3) {
		return -1;
	}
	*(buffer++) = (uint8_t)(in >> 16);
	*(buffer++) = (uint8_t)(in >> 8);
	*(buffer++) = (uint8_t)in;
	return 0;
}
