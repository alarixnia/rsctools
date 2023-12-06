#include <bzlib.h>
#include <stdlib.h>
#include <string.h>
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
		goto fail;
	}

	if (jag_read_header(header, 0, JAG_HEADER_SIZE, out) == -1) {
		goto fail;
	}

	/* not using archive-wide compression */
	if (out->unpacked_len == out->packed_len) {
		if ((unpacked_data = malloc(out->unpacked_len)) == NULL) {
			goto fail;
		}

		if (fread(unpacked_data, 1,
		    out->unpacked_len, f) != out->unpacked_len) {
			goto fail;
		}
		out->data = unpacked_data;
		out->must_free = 1;
		return 0;
	}

	if ((packed_data = malloc(out->packed_len +
	    sizeof(JAG_BZIP2_MAGIC) - 1)) == NULL) {
		goto fail;
	}

	/* fake a bzip2 header to satisfy the library */
	memcpy(packed_data, JAG_BZIP2_MAGIC, sizeof(JAG_BZIP2_MAGIC) - 1);

	if (fread(packed_data + sizeof(JAG_BZIP2_MAGIC) - 1, 1,
	    out->packed_len, f) != out->packed_len) {
		goto fail;
	}

	if ((unpacked_data = malloc(out->unpacked_len)) == NULL) {
		goto fail;
	}

	bz.next_in = packed_data;
	bz.avail_in = out->packed_len + sizeof(JAG_BZIP2_MAGIC) - 1;
	bz.next_out = unpacked_data;
	bz.avail_out = out->unpacked_len;

	if (BZ2_bzDecompressInit(&bz, 0, 0) != BZ_OK) {
		goto fail;
	}

	while (bz.avail_out > 0) {
		ret = BZ2_bzDecompress(&bz);
		switch (ret) {
		case BZ_OK:
			continue;
		case BZ_STREAM_END:
			break;
		default:
			goto fail;
		}
	}

	BZ2_bzDecompressEnd(&bz);
	free(packed_data);

	out->data = unpacked_data;
	out->must_free = 1;
	return 0;
fail:
	BZ2_bzDecompressEnd(&bz);
	free(packed_data);
	free(unpacked_data);
	return -1;
}
