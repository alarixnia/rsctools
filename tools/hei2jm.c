#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <map.h>
#include "utility.h"

#define WATER	(2)
#define BLANK	(8)

#ifndef PLANE_LEVEL_INC
#define PLANE_LEVEL_INC (944)
#endif

static int version = 63;

/*
 * used for cleaning up when converting a full list of
 * loc spawns to older land data
 */
static int locs_not_for_water[] = {
	0, 1, 21, 32, 33, 34, 37, 38, 41, 51, 55, 63, 64, 110, 111, 183,
	184, 112, 113, 118, 205, 209, 214, 273
};

static int coal_trucks_locs[] = {
	149, 150, 151, 152, 153
};

#define BED	(15)

enum bound_dir {
	BOUND_DIR_VERT		= 0,
	BOUND_DIR_HORIZ		= 1,
	BOUND_DIR_DIAG_NW_SE	= 2,
	BOUND_DIR_DIAG_NE_SW	= 3,
};

static void __dead
usage(void)     
{                       
	fputs("hei2jm [-v version] hei dat jm\n", stderr);
	exit(1);
}

static void
read_locs_txt(struct jag_map *m, const char *path, int global_x, int global_y)
{
	int ret;
	int x, y, id;
	FILE *f;

	f = fopen(path, "r");
	if (f == NULL) {
		fprintf(stderr,
		    "open loc txt failed: %s\n", strerror(errno));
		return;
	}
	for (;;) {
		int idx;
cont:
		ret = fscanf(f, "%d %d %d\n", &x, &y, &id);
		if (ret == 0 || ret == EOF) {
			break;
		}
		x -= global_x;
		y -= global_y;
		idx = y + x * JAG_MAP_CHUNK_SIZE;
		if (x >= 0 && y >= 0 &&
		    x < JAG_MAP_CHUNK_SIZE && y < JAG_MAP_CHUNK_SIZE) {
			if (version < 41 && global_y < PLANE_LEVEL_INC) {
				size_t sz;

				sz = sizeof(coal_trucks_locs) /
				    sizeof(coal_trucks_locs[0]);
				for (int i = 0; i < sz; ++i) {
					if (coal_trucks_locs[i] == id) {
						goto cont;
					}
				}
			}
			if (version < 48 && m->tiles[idx].overlay == 0 &&
			    id == BED) {
				continue;
			}
			if (version < 63 && (m->tiles[idx].overlay == WATER ||
			    (m->tiles[idx].overlay == BLANK &&
			    global_y > PLANE_LEVEL_INC))) {
				size_t sz;

				sz = sizeof(locs_not_for_water) /
				    sizeof(locs_not_for_water[0]);
				for (int i = 0; i < sz; ++i) {
					if (locs_not_for_water[i] == id) {
						goto cont;
					}
				}
			}
			if (version < 70 && global_x == 96 && global_y == 672 &&
			    id >= 98 && id <= 115) {
				/* no lumbridge swamp mine */
				goto cont;
			}
			m->tiles[idx].bound_diag = JAG_MAP_DIAG_LOC + id + 1;
		}
	}
	fclose(f);
}

static void
read_npcs_txt(struct jag_map *m, const char *path, int global_x, int global_y)
{
	char *str;
	char line[1024];
	int ret;
	int x, y, id;
	FILE *f;
	int idx;

	f = fopen(path, "r");
	if (f == NULL) {
		fprintf(stderr,
		    "open npc txt failed: %s\n", strerror(errno));
		return;
	}
	for (;;) {
		str = fgets(line, sizeof(line), f);
		if (str == NULL) {
			break;
		}
		if (line[0] == ';') {
			continue;
		}
		ret = sscanf(line, "%d %d %d\n", &id, &x, &y);
		if (ret == 0) {
			break;
		}
		x -= global_x;
		y -= global_y;
		if (x < 0 || y < 0 || x >= JAG_MAP_CHUNK_SIZE ||
		    y >= JAG_MAP_CHUNK_SIZE) {
			continue;
		}
		idx = y + x * JAG_MAP_CHUNK_SIZE;
		if (version < 63 && (m->tiles[idx].overlay == WATER ||
		    (m->tiles[idx].overlay == BLANK &&
		    global_y > PLANE_LEVEL_INC)) &&
		    m->tiles[idx].bound_horiz == 0 &&
		    m->tiles[idx].bound_vert == 0 &&
		    m->tiles[idx].bound_diag == 0) {
			continue;
		}
		m->tiles[idx].bound_diag = JAG_MAP_DIAG_NPC + id + 1;
	}
	fclose(f);
}

static void
read_objs_txt(struct jag_map *m, const char *path, int global_x, int global_y)
{
	char *str;
	char line[1024];
	int ret;
	int x, y, id, stack;
	int idx;
	FILE *f;

	f = fopen(path, "r");
	if (f == NULL) {
		fprintf(stderr,
		    "open obj txt failed: %s\n", strerror(errno));
		return;
	}
	for (;;) {
		str = fgets(line, sizeof(line), f);
		if (str == NULL) {
			break;
		}
		if (line[0] == ';') {
			continue;
		}
		ret = sscanf(line, "%d %d %d %d\n", &x, &y, &id, &stack);
		if (ret == 0) {
			break;
		}
		x -= global_x;
		y -= global_y;
		if (x < 0 || y < 0 || x >= JAG_MAP_CHUNK_SIZE ||
		    y >= JAG_MAP_CHUNK_SIZE) {
			continue;
		}
		idx = y + x * JAG_MAP_CHUNK_SIZE;
		if (version < 63 && (m->tiles[idx].overlay == WATER ||
		    (m->tiles[idx].overlay == BLANK &&
		    global_y > PLANE_LEVEL_INC))) {
			continue;
		}
		if (m->tiles[idx].bound_diag > 0) {
			y++;
		}
		m->tiles[y + x * JAG_MAP_CHUNK_SIZE].loc_direction =
		    stack;
		m->tiles[y + x * JAG_MAP_CHUNK_SIZE].bound_diag =
		    JAG_MAP_DIAG_ITEM + id + 1;
	}
	fclose(f);
}

static void
read_bounds_txt(struct jag_map *m, const char *path, int global_x, int global_y)
{
	char *str;
	char line[1024];
	int ret;
	int x, y, dir, id;
	FILE *f;

	f = fopen(path, "r");
	if (f == NULL) {
		fprintf(stderr,
		    "open bound txt failed: %s\n", strerror(errno));
		return;
	}
	for (;;) {
		str = fgets(line, sizeof(line), f);
		if (str == NULL) {
			break;
		}
		if (line[0] == ';') {
			continue;
		}
		ret = sscanf(line, "%d %d %d %d\n", &x, &y, &dir, &id);
		if (ret == 0) {
			break;
		}
		x -= global_x;
		y -= global_y;
		if (x < 0 || y < 0 || x >= JAG_MAP_CHUNK_SIZE ||
		    y >= JAG_MAP_CHUNK_SIZE) {
			continue;
		}
		int idx = y + x * JAG_MAP_CHUNK_SIZE;
		if (version < 63 && (m->tiles[idx].overlay == WATER ||
		    (m->tiles[idx].overlay == BLANK &&
		    global_y > PLANE_LEVEL_INC))) {
			continue;
		}
		switch (dir) {
		case BOUND_DIR_VERT:
			if (m->tiles[idx].bound_vert == 0) {
				m->tiles[idx].bound_vert = id + 1;
			}
			break;
		case BOUND_DIR_HORIZ:
			if (m->tiles[idx].bound_horiz == 0) {
				m->tiles[idx].bound_horiz = id + 1;
			}
			break;
		case BOUND_DIR_DIAG_NW_SE:
			if (m->tiles[idx].bound_diag == 0) {
				m->tiles[idx].bound_diag = id + 1;
			}
			break;
		case BOUND_DIR_DIAG_NE_SW:
			if (m->tiles[idx].bound_diag == 0) {
				m->tiles[idx].bound_diag =
				    id + 1 + JAG_MAP_DIAG_INVERSE;
			}
			break;
		}
	}
	fclose(f);
}

int main(int argc, char **argv)
{
	FILE *out;
	char ch;
	struct jag_map m = {0};
	int plane = 0, chunk_x = 50, chunk_y = 50;
	int global_x, global_y;
	const char *path;
	void *hei_data, *dat_data;
	uint8_t jm_data[JAG_MAP_JM_FILE_LEN];
	size_t l1, l2;
	const char *bound_path = NULL;
	const char *loc_path = NULL;
	const char *obj_path = NULL;
	const char *npc_path = NULL;
	bool no_land = false;

	while ((ch = getopt(argc, argv, "b:l:o:n:v:")) != -1) {
		switch (ch) {
		case 'b':
			bound_path = optarg;
			break;
		case 'l':
			loc_path = optarg;
			break;
		case 'o':
			obj_path = optarg;
			break;
		case 'n':
			npc_path = optarg;
			break;
		case 'v':
			errno = 0;
			version = strtol(optarg, NULL, 10);
			if (errno != 0) {
				usage();
			}
			break;
		default: 
			usage(); 
			break;
		}
	}

	argc -= optind; 
	argv += optind;

	if (argc < 3) {
		usage();
	}

	hei_data = read_file_full(argv[0], &l1);
	if (hei_data == NULL) {
		fprintf(stderr, "read land .hei failed: %s", strerror(errno));
		fprintf(stderr, " - will fill with blank\n");
		no_land = true;
	}

	dat_data = read_file_full(argv[1], &l2);
	if (dat_data == NULL) {
		fprintf(stderr, "read map .dat failed: %s\n", strerror(errno));
		return 1;
	}

	path = basename(argv[0]);
	sscanf(path, "m%1d%02d%02d.hei", &plane, &chunk_x, &chunk_y);

	global_x = (chunk_x - JAG_MAP_CHUNK_SIZE) *
	    JAG_MAP_CHUNK_SIZE;
	global_y = (plane * PLANE_LEVEL_INC) +
	    ((chunk_y - 37) * JAG_MAP_CHUNK_SIZE);

	if (!no_land && jag_map_read_hei(&m, hei_data, l1) != 0) {
		fprintf(stderr, "parse map .hei failed\n");
		return 1;
	}

	if (jag_map_read_dat(&m, dat_data, l2, version) != 0) {
		fprintf(stderr, "parse map .dat failed\n");
		return 1;
	}

	if (loc_path != NULL) {
		read_locs_txt(&m, loc_path, global_x, global_y);
	}

	if (obj_path != NULL) {
		read_objs_txt(&m, obj_path, global_x, global_y);
	}

	if (npc_path != NULL) {
		read_npcs_txt(&m, npc_path, global_x, global_y);
	}

	if (bound_path != NULL) {
		read_bounds_txt(&m, bound_path, global_x, global_y);
	}

	if (jag_map_write_jm(&m, jm_data, JAG_MAP_JM_FILE_LEN) != 0) {
		fprintf(stderr, "formatting .jm file failed\n");
		return 1;
	}

	out = fopen(argv[2], "wb");
	if (out == NULL) {
		fprintf(stderr, "failed to open output file\n");
		return 1;
	}
	fwrite(jm_data, 1, JAG_MAP_JM_FILE_LEN, out);
	fclose(out);

	return 0;
}
