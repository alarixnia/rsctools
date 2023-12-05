#include <bzlib.h>
#include <stdlib.h>
#include <assert.h>
#include "buffer.h"
#include "jag.h"

#define READSZ	(4096)

int
jag_unpack_file(FILE *f, struct jag_archive *out)
{
	bz_stream bz = {0};
	uint8_t header[JAG_HEADER_SIZE];
	uint8_t *packed_data = NULL;
	uint8_t *unpacked_data = NULL;
	int ret;

	if (fread(header, 1, JAG_HEADER_SIZE, f) != JAG_HEADER_SIZE) {
		assert(0);
		goto fail;
	}

	if (jag_read_header(header, 0, JAG_HEADER_SIZE, out) == -1) {
		assert(0);
		goto fail;
	}

	if ((packed_data = malloc(out->packed_len)) == NULL) {
		assert(0);
		goto fail;
	}

	if (fread(packed_data, 1, out->packed_len, f) != out->packed_len) {
		assert(0);
		goto fail;
	}

	/* not using archive-wide compression */
	if (out->unpacked_len == out->packed_len) {
		out->data = packed_data;
		out->must_free = 1;
		return 0;
	}

	if ((unpacked_data = malloc(out->unpacked_len)) == NULL) {
		assert(0);
		goto fail;
	}

	/* TODO decompress headerless bzip2 */
	out->data = unpacked_data;
	out->must_free = 1;
	return 0;
fail:
	free(packed_data);
	free(unpacked_data);
	return -1;
}
