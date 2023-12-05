#include <bzlib.h>
#include <stdlib.h>
#include "buffer.h"
#include "jag.h"

int
jag_unpack_stream(void *buf, size_t off, size_t len, struct jag_archive *out)
{
	bz_stream bz = {0};
	void *new_data = NULL;
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

	new_data = malloc(out->unpacked_len);
	if (new_data == NULL) {
		return -1;
	}
	/* TODO decompress headerless bzip2 */
	out->data = new_data;
	out->must_free = 1;
	return 0;
}
