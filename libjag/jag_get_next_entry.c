#include <jag.h>
#include <buffer.h>
#include <stdint.h>
#include <limits.h>

size_t
jag_get_next_entry(void *b, size_t offset, size_t len)
{
	uint32_t hash;
	uint32_t size;
	uint32_t size_packed;

	if (jag_getu32(b, offset, len, &hash) != 0) {
		return SIZE_MAX;
	}
	offset += 4;

	if (jag_getu24(b, offset, len, &size) != 0) {
		return SIZE_MAX;
	}
	offset += 3;

	if (jag_getu24(b, offset, len, &size_packed) != 0) {
		return SIZE_MAX;
	}
	offset = offset + size_packed + 3;
	return offset;
}
