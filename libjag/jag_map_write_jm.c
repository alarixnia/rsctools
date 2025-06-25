#include <map.h>
#include <buffer.h>

int
jag_map_write_jm(struct jag_map *in, void *out, size_t len)
{
	uint8_t *b = out;
	int prev;
	size_t offset = 0;

	if (jag_putu8(b, offset++, len,
	    in->tiles[0].height & 0xFF) == -1) {
		return -1;
	}
	prev = in->tiles[0].height;
	for (int i = 1; i < JAG_MAP_CHUNK_AREA; ++i) {
		if (jag_putu8(b, offset++, len,
		    (in->tiles[i].height - prev) & 0xFF) == -1) {
			return -1;
		}
		prev = in->tiles[i].height;
	}

	if (jag_putu8(b, offset++, len,
	    in->tiles[0].colour & 0xFF) == -1) {
		return -1;
	}
	prev = in->tiles[0].colour;
	for (int i = 1; i < JAG_MAP_CHUNK_AREA; ++i) {
		if (jag_putu8(b, offset++, len,
		    (in->tiles[i].colour - prev) & 0xFF) == -1) {
			return -1;
		}
		prev = in->tiles[i].colour;
	}

	for (int i = 0; i < JAG_MAP_CHUNK_AREA; ++i) {
		if (jag_putu8(b, offset++, len,
		    in->tiles[i].bound_horiz & 0xFF) == -1) {
			return -1;
		}
	}
	for (int i = 0; i < JAG_MAP_CHUNK_AREA; ++i) {
		if (jag_putu8(b, offset++, len,
		    in->tiles[i].bound_vert & 0xFF) == -1) {
			return -1;
		}
	}
	for (int i = 0; i < JAG_MAP_CHUNK_AREA; ++i) {
		if (jag_putu16(b, offset, len,
		    in->tiles[i].bound_diag & 0xFFFF) == -1) {
			return -1;
		}
		offset += 2;
	}
	for (int i = 0; i < JAG_MAP_CHUNK_AREA; ++i) {
		if (jag_putu8(b, offset++, len,
		    in->tiles[i].roof & 0xFF) == -1) {
			return -1;
		}
	}
	for (int i = 0; i < JAG_MAP_CHUNK_AREA; ++i) {
		if (jag_putu8(b, offset++, len,
		    in->tiles[i].overlay & 0xFF) == -1) {
			return -1;
		}
	}
	for (int i = 0; i < JAG_MAP_CHUNK_AREA; ++i) {
		if (jag_putu8(b, offset++, len,
		    in->tiles[i].loc_direction & 0xFF) == -1) {
			return -1;
		}
	}
	return 0;
}
