#include <map.h>
#include <buffer.h>

int
jag_map_read_hei(struct jag_map *out, void *b, size_t len) {
	size_t offset = 0;
	int prev = 0;
	uint8_t val = 0;
	unsigned i, j;
	unsigned x, y;

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (val < 128) {
			out->tiles[i++].height = val;
			prev = val;
		} else {
			for (j = 0; j < (val - 128); ++j) {
				out->tiles[i++].height = prev & 0xff;
			}
		}
	}

	prev = 64;

	for (y = 0; y < JAG_MAP_CHUNK_SIZE; ++y) {
		for (x = 0; x < JAG_MAP_CHUNK_SIZE; ++x) {
			i = x * JAG_MAP_CHUNK_SIZE + y;

			prev = (out->tiles[i].height + prev) & 127;
			out->tiles[i].height = (prev * 2) & 0xFF;
		}
	}

	prev = 0;

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (val < 128) {
			out->tiles[i++].colour = val;
			prev = val;
		} else {
			for (j = 0; j < (val - 128); ++j) {
				out->tiles[i++].colour = prev & 0xff;
			}
		}
	}

	prev = 35;

	for (y = 0; y < JAG_MAP_CHUNK_SIZE; ++y) {
		for (x = 0; x < JAG_MAP_CHUNK_SIZE; ++x) {
			i = x * JAG_MAP_CHUNK_SIZE + y;

			prev = (out->tiles[i].colour + prev) & 127;
			out->tiles[i].colour = (prev * 2) & 0xFF;
		}
	}

	return 0;
}
