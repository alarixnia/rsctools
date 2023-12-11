#include <map.h>
#include <buffer.h>

int
jag_map_read_loc(struct jag_map *out, void *b, size_t len) {
	size_t offset = 0;
	uint8_t val = 0;
	unsigned i;

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (val < 128) {
			out->tiles[i++].bound_diag = val + JAG_MAP_DIAG_LOC;
		} else {
			i += (val - 128);
		}
	}

	return 0;
}
