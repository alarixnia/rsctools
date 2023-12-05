#include <buffer.h>

int
jag_getu8(void *b, size_t offset, size_t buflen, uint8_t *out)
{
	uint8_t *buffer = b;
	if (offset > (SIZE_MAX - 1) ||
	    ((ptrdiff_t)buflen - (ptrdiff_t)offset) < 1) {
		return -1;
	}
	*out = buffer[offset] & 0xff;
	return 0;
}
