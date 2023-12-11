#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "jag.h"

/*
 * Read the header of a .jag archive and perform whole-archive
 * extraction if necessary.
 */
int
jag_unpack_stream(void *buf, size_t off, size_t len, struct jag_archive *out)
{
	uint8_t *unpacked_data = NULL;
	int ret;

	if ((ret = jag_read_header(buf, off, len, out)) == -1) {
		return -1;
	}
	off += ret;

	/* not using archive-wide compression */
	if (out->unpacked_len == out->packed_len) {
		out->data = buf + off;
		out->must_free = 0;
		return 0;
	}

	unpacked_data = jag_bzip2_decompress(buf + off,
	    out->packed_len, out->unpacked_len);
	if (unpacked_data != NULL) {
		out->data = unpacked_data;
		out->must_free = 1;
		return 0;
	}
	return -1;
}
