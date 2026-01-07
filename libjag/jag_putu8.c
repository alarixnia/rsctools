#include <buffer.h>
#include <limits.h>
#include <stdio.h>

int
jag_putu8(void *b, size_t offset, size_t buflen, uint8_t in)
{
	uint8_t *buffer = b;
	if (offset > (INTPTR_MAX - 1) ||
	    ((intptr_t)buflen - (intptr_t)offset) < 1) {
		return -1;
	}
	buffer[offset] = in;
	return 0;
}
