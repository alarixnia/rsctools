#include <buffer.h>

int
jag_gets8(void *b, size_t offset, size_t buflen, int8_t *out)
{
	uint8_t *buffer = b;
	if (offset > (SIZE_MAX - 1) ||
	    ((ptrdiff_t)buflen - (ptrdiff_t)offset) < 1) {
		return -1;
	}
	*out = (int8_t)buffer[offset];
	return 0;
}
