#include <bzlib.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "jag.h"

int
jag_unpack_stream(void *buf, size_t off, size_t len, struct jag_archive *out)
{
	bz_stream bz = {0};
	uint8_t *packed_data = NULL;
	uint8_t *unpacked_data= NULL;
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

	if ((packed_data = malloc(out->packed_len +
	    sizeof(JAG_BZIP2_MAGIC) - 1)) == NULL) {
		goto fail;
	}

	/* fake a bzip2 header to satisfy the library */
	memcpy(packed_data, JAG_BZIP2_MAGIC, sizeof(JAG_BZIP2_MAGIC) - 1);

	memcpy(packed_data + sizeof(JAG_BZIP2_MAGIC) - 1, buf + off,
	    out->packed_len);

	unpacked_data = malloc(out->unpacked_len);
	if (unpacked_data == NULL) {
		return -1;
	}

	bz.next_in = (char *)packed_data;
	bz.avail_in = out->packed_len + sizeof(JAG_BZIP2_MAGIC) - 1;
	bz.next_out = (char *)unpacked_data;
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
