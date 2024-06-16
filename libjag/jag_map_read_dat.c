#include <map.h>
#include <buffer.h>

int
jag_map_read_dat(struct jag_map *out, void *b, size_t len, int version) {
	size_t offset = 0;
	uint8_t val = 0;
	uint8_t last_val = 0;
	unsigned i, j;

	/*
	 * maps.jag versions after 52 do not use run-length encoding
	 * for walls/boundaries.
	 */

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (version > 53 || val < 128) {
			out->tiles[i++].bound_horiz = val;
		} else {
			for (j = 0; j < (val - 128); ++j) {
				out->tiles[i++].bound_horiz = 0;
			}
		}
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (version > 53 || val < 128) {
			out->tiles[i++].bound_vert = val;
		} else {
			for (j = 0; j < (val - 128); ++j) {
				out->tiles[i++].bound_vert = 0;
			}
		}
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (version > 53 || val < 128) {
			out->tiles[i++].bound_diag = val;
		} else {
			for (j = 0; j < (val - 128); ++j) {
				out->tiles[i++].bound_diag = 0; 
			}
		}
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (version > 53 || val < 128) {
			if (val > 0) {
				out->tiles[i].bound_diag = val +
				    JAG_MAP_DIAG_INVERSE;
			}
			i++;
		} else {
			i += (val - 128);
		}
	}

	/* after this point the format is consistent from 2002-2003. */

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (val < 128) {
			out->tiles[i++].roof = val;
		} else {
			for (j = 0; j < (val - 128); ++j) {
				out->tiles[i++].roof = 0; 
			}
		}
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (val < 128) {
			out->tiles[i++].overlay = val;
			last_val = val;
		} else {
			for (j = 0; j < (val - 128); ++j) {
				out->tiles[i++].overlay = last_val;
			}
		}
	}

	for (i = 0; i < JAG_MAP_CHUNK_AREA;) {
		if (jag_getu8(b, offset++, len, &val) != 0) {
			return -1;
		}
		if (val < 128) {
			out->tiles[i++].loc_direction = val;
		} else {
			for (j = 0; j < (val - 128); ++j) {
				out->tiles[i++].loc_direction = 0; 
			}
		}
	}

	return 0;
}
