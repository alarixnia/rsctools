#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <map.h>
#include "utility.h"

/* quite old code, predates libjag... */

#define MAPSTARTX	(48)
#define MAPSTARTY	(39)
#define MAPSIZE		(17)
#define TILESIZE	(3)
#define XSIZE		((MAPSIZE * JAG_MAP_CHUNK_SIZE) * TILESIZE)
#define YSIZE		((MAPSIZE * JAG_MAP_CHUNK_SIZE) * TILESIZE)
#define IMGSIZE		(XSIZE * YSIZE)

struct world {
	struct jag_map chunks[MAPSIZE][MAPSIZE];
};

static void
usage(void)
{
	fputs("jm2tga outfile.tga\n", stderr);
	exit(1);
}

static uint32_t colour_tab[256];

static int load_chunk(struct jag_map *, unsigned, unsigned, unsigned);
static uint32_t get_tile_colour(const struct jag_map *, unsigned, unsigned);
static uint32_t get_loc_colour(unsigned);
static int draw_chunk(uint32_t *, unsigned, unsigned, const struct jag_map *);
static int create_image(const struct world *, uint32_t *);

static int
load_chunk(struct jag_map *c, unsigned x, unsigned y, unsigned z)
{
	void *buf;
	size_t len;
	char name[128];

	snprintf(name, sizeof(name), "./m%1d%2d%2d.jm", z, x, y);
	buf = read_file_full(name, &len);
	if (buf == NULL) {
		for (int i = 0; i < JAG_MAP_CHUNK_AREA; ++i) {
			c->tiles[i].overlay = 2;
		}
		return 0;
	}
	jag_map_read_jm(c, buf, len);
	return 0;
}

static uint32_t
get_tile_colour(const struct jag_map *c, unsigned x, unsigned y)
{
	const unsigned pos = x + y * JAG_MAP_CHUNK_SIZE;

	switch (c->tiles[pos].overlay) {
	case 0: return colour_tab[c->tiles[pos].colour];
	case 1: return 0x40403f;
	case 2: return 0x1f3f7d;
	case 3: return 0x603001;
	case 4: return 0x603001;
	case 5: return 0x40403f;
	case 6: return 0x6b030f;
	case 7: return 0x0e2c38;
	case 8: return 0x000000;
	case 9: return 0x606260;
	case 10: return 0x000000;
	case 11: return 0x9f5000;
	case 12: return 0x613000;
	case 13: return 0x1f3f7d;
	case 14: return 0x40403f;
	case 15: return 0x241609;
	case 16: return 0x241609;
	case 17: return 0x797b7a;
	case 23: return 0x483000;
	case 21: return 0x000000;
	case 24: return 0x3f2200;
	default:
		printf("Missing overlay definition: %u\n", c->tiles[pos].overlay);
		return 0x0;
	}
}

static uint32_t
get_loc_colour(unsigned id)
{
	if (id == 0 || id == 1) {
		return 0x00a500;
	}
	return 0xb56300;
}

static int
draw_chunk(uint32_t *pixels, unsigned chunkx, unsigned chunky, const struct jag_map *c)
{
	unsigned x, y;
	unsigned ox, oy;
	unsigned dx, dy;
	unsigned pos;
	unsigned i;
	uint32_t colour;

	for (x = 0; x < JAG_MAP_CHUNK_SIZE; ++x) {
		for (y = 0; y < JAG_MAP_CHUNK_SIZE; ++y) {
			colour = get_tile_colour(c, x, y);

			ox = (((MAPSIZE - chunkx) * JAG_MAP_CHUNK_SIZE) + (JAG_MAP_CHUNK_SIZE - y)) * TILESIZE;
			oy = ((chunky * JAG_MAP_CHUNK_SIZE) + x) * TILESIZE;
			pos = x + y * JAG_MAP_CHUNK_SIZE;

			for (dx = ox; dx < (ox + TILESIZE); ++dx) {
				for (dy = oy; dy < (oy + TILESIZE); ++dy) {
					pixels[dx + dy * XSIZE] = colour;
				}
			}

			if (c->tiles[pos].bound_horiz != 0) {
				if (x > 0 && c->tiles[(x - 1) + y * JAG_MAP_CHUNK_SIZE].overlay == 0) {
					for (i = 0; i < TILESIZE; ++i) {
						pixels[(ox - (TILESIZE - 1)) + (oy + i) * XSIZE] = 0x636363;
					}
				} else if (x < JAG_MAP_CHUNK_SIZE) {
					for (i = 0; i < TILESIZE; ++i) {
						pixels[(ox + (TILESIZE - 1)) + (oy + i) * XSIZE] = 0x636363;
					}
				}
			}

			if (c->tiles[pos].bound_vert != 0) {
				for (i = 0; i < TILESIZE; ++i) {
					pixels[(ox + i) + oy * XSIZE] = 0x636363;
				}
			}

			if (c->tiles[pos].bound_diag != 0) {
				if (c->tiles[pos].bound_diag < JAG_MAP_DIAG_INVERSE) {
					for (i = (TILESIZE - 1); i != UINT_MAX; --i) {
						pixels[(ox + (TILESIZE - 1) - i) + (oy + i) * XSIZE] = 0x636363 + i;
					}
				} else if (c->tiles[pos].bound_diag < JAG_MAP_DIAG_NPC) {
					for (i = (TILESIZE - 1); i != UINT_MAX; --i) {
						pixels[(ox + i) + (oy + i) * XSIZE] = 0x636363 + i;
					}
				} else if (c->tiles[pos].bound_diag >= JAG_MAP_DIAG_LOC) {
					colour = get_loc_colour(c->tiles[pos].bound_diag - JAG_MAP_DIAG_LOC);
					pixels[ox + oy * XSIZE] = colour;
					pixels[(ox + 2) + oy * XSIZE] = colour;
					pixels[(ox + 1) + (oy - 1) * XSIZE] = colour;
					pixels[(ox + 1) + (oy + 1) * XSIZE] = colour;
					pixels[(ox + 1) + oy * XSIZE] = colour;
				}
			}
		}
	}
	return 0;
}

static int
create_image(const struct world *w, uint32_t *pixels)
{
	unsigned x, y;

	for (x = 0; x < MAPSIZE; ++x) {
		for (y = 0; y < MAPSIZE; ++y) {
			if (draw_chunk(pixels, x, y, &w->chunks[x][y]) != 0) {
				return 1;
			}
		}
	}
	return 0;
}

static int
write_tga(const char *outname, const uint32_t *pixels)
{
	unsigned i;
	FILE *f;

	if ((f = fopen(outname, "w")) == NULL) {
		return 1;
	}
	fputc(0, f); /* id len */
	fputc(0, f); /* maptype */
	fputc(2, f); /* uncompressed trucolour */
	fputc(0, f); /* map entry index */
	fputc(0, f); /* map entry index */
	fputc(0, f); /* map len */
	fputc(0, f); /* map len */
	fputc(0, f); /* map bpp */
	fputc(0, f); /* x-origin */
	fputc(0, f); /* x-origin */
	fputc(0, f); /* y-origin */
	fputc(0, f); /* y-origin */
	fputc((uint8_t)(XSIZE >> 0), f); /* width */
	fputc((uint8_t)(XSIZE >> 8), f); /* width */
	fputc((uint8_t)(YSIZE >> 0), f); /* height */
	fputc((uint8_t)(YSIZE >> 8), f); /* height */
	fputc(24, f); /* bpp */
	fputc(0x20, f); /* desc */
	for (i = 0; i < IMGSIZE; ++i) {
		fputc((uint8_t)(pixels[i] >> 0), f);
		fputc((uint8_t)(pixels[i] >> 8), f);
		fputc((uint8_t)(pixels[i] >> 16), f);
	}
	fclose(f);
	return 0;
}

int
main(int argc, char **argv)
{
	struct world *w;
	unsigned x, y;
	unsigned i;
	uint32_t *pixels;

	if (argc < 2) {
		usage();
	}

	for (i = 0; i < 64; ++i) {
		colour_tab[i] = ((255 - i * 4) << 16) |
			((255 - (int)(i * 1.75)) << 8) |
			((255 - i * 4) & 0xFF) << 0;
	}
	for (i = 0; i < 64; ++i) {
		colour_tab[i + 64] = ((i * 3) << 16) |
			(144 << 8) |
			(0 << 0);
	}
	for (i = 0; i < 64; ++i) {
		colour_tab[i + 128] = ((192 - (int)(i * 1.5)) << 16) |
			((144 - (unsigned)(i * 1.5)) << 8) |
			(0 << 0);
	}
	for (i = 0; i < 64; ++i) {
		colour_tab[i + 192] = ((96 - (int)(i * 1.5)) << 16) |
			((48 + (unsigned)(i * 1.5)) << 8) |
			(0 << 0);
	}
	for (i = 0; i < 256; ++i) {
		colour_tab[i] = colour_tab[i] >> 1 & 0x7f7f7f;
	}

	if ((w = calloc(sizeof(struct world), 1)) == NULL) {
		return 1;
	}
	for (x = 0; x < MAPSIZE; ++x) {
		for (y = 0; y < MAPSIZE; ++y) {
			if (load_chunk(&w->chunks[x][y],
				MAPSTARTX + x, MAPSTARTY + y, 0) != 0) {
					perror("load_chunk");
			}
		}
	}
	if ((pixels = calloc(IMGSIZE, sizeof(uint32_t))) == NULL) {
		perror("calloc");
		return 1;
	}
	if (create_image(w, pixels) != 0) {
		perror("create_image");
		free(pixels);
		return 1;
	}
	if (write_tga(argv[1], pixels) != 0) {
		perror("write_tga");
		free(pixels);
		return 1;
	}
	free(pixels);
	return 0;
}
