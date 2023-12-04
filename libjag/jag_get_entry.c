#include <jag.h>
#include <buffer.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

size_t
jag_get_entry(void *b, size_t offset, size_t len, const char *name)
{
	uint32_t target_hash = jag_hash_entry_name(name);

	uint32_t found_hash;
	uint32_t found_size;
	uint32_t found_size_packed;

	for (;;) {
		if (jag_getu32(b, offset, len, &found_hash) != 0) {
			break;
		}
		if (found_hash == target_hash) {
			return offset;
		}
		offset += 4;

		if (jag_getu24(b, offset, len, &found_size) != 0) {
			break;
		}
		offset += 3;

		if (jag_getu24(b, offset, len, &found_size_packed) != 0) {
			break;
		}
		offset += 3;
	}
	return SIZE_MAX;
}
