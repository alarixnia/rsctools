#include <bzlib.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <jag.h>

int
jag_unpack_entry(struct jag_entry *entry)
{
	bz_stream bz = {0};
	uint8_t *unpacked_data = NULL;
	uint8_t *packed_data = NULL;
	int ret;

	if (entry->packed_len == entry->unpacked_len) {
		/* already unpacked */
		return 0;
	}

	packed_data = malloc(entry->packed_len + sizeof(JAG_BZIP2_MAGIC) - 1);
	if (packed_data == NULL) {
		assert(0);
		goto fail;
	}

	/* fake a bzip2 header to satisfy the library */
	memcpy(packed_data, JAG_BZIP2_MAGIC, sizeof(JAG_BZIP2_MAGIC) - 1);

	memcpy(packed_data + sizeof(JAG_BZIP2_MAGIC) - 1,
	    entry->data, entry->packed_len);

	unpacked_data = malloc(entry->unpacked_len);
	if (unpacked_data == NULL) {
		assert(0);
		goto fail;
	}

	bz.next_in = (char *)packed_data;
	bz.avail_in = entry->packed_len + sizeof(JAG_BZIP2_MAGIC) - 1;
	bz.next_out = (char *)unpacked_data;
	bz.avail_out = entry->unpacked_len;

	if (BZ2_bzDecompressInit(&bz, 0, 0) != BZ_OK) {
		assert(0);
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
			printf("bzip2 error %d\n", ret);
			assert(0);
			goto fail;
		}
	}

	BZ2_bzDecompressEnd(&bz);
	free(packed_data);
	packed_data = NULL;

	if (entry->must_free) {
		free(entry->data);
	}

	entry->packed_len = entry->unpacked_len;
	entry->data = unpacked_data;
	entry->must_free = 1;
	return 0;

fail:
	BZ2_bzDecompressEnd(&bz);
	free(unpacked_data);
	free(packed_data);
	return -1;
}
