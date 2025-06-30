#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <map.h>
#include "utility.h"

#ifndef PLANE_LEVEL_INC
#define PLANE_LEVEL_INC (944)
#endif

static int version = 63;

enum bound_dir {
	BOUND_DIR_VERT		= 0,
	BOUND_DIR_HORIZ		= 1,
	BOUND_DIR_DIAG_NW_SE	= 2,
	BOUND_DIR_DIAG_NE_SW	= 3,
};

static void __dead
usage(void)     
{                       
	fputs("mapdat2txt [-v version] dat\n", stderr);
	exit(1);
}

int
main(int argc, char **argv)
{
	char ch;
	struct jag_map m = {0};
	int plane = 0, chunk_x = 50, chunk_y = 50;
	int global_x, global_y;
	const char *path;
	void *dat_data;
	size_t l1;

	while ((ch = getopt(argc, argv, "v:")) != -1) {
		switch (ch) {
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

	if (argc < 1) {
		usage();
	}

	dat_data = read_file_full(argv[0], &l1);
	if (dat_data == NULL) {
		fprintf(stderr, "read map .dat failed: %s\n", strerror(errno));
		return 1;
	}

	path = basename(argv[0]);
	sscanf(path, "m%1d%02d%02d.dat", &plane, &chunk_x, &chunk_y);

	global_x = (chunk_x - JAG_MAP_CHUNK_SIZE) *
	    JAG_MAP_CHUNK_SIZE;
	global_y = (plane * PLANE_LEVEL_INC) +
	    ((chunk_y - 37) * JAG_MAP_CHUNK_SIZE);

	if (jag_map_read_dat(&m, dat_data, l1, version) != 0) {
		fprintf(stderr, "parse map .dat failed\n");
		return 1;
	}

	for (int x = 0; x < JAG_MAP_CHUNK_SIZE; ++x) {
		for (int y = 0; y < JAG_MAP_CHUNK_SIZE; ++y) {
			int idx = y + x * JAG_MAP_CHUNK_SIZE;
			if (m.tiles[idx].bound_vert > 0) {
				printf("%d %d bound type %d dir 0\n",
					x + global_x,
					y + global_y,
					m.tiles[idx].bound_vert - 1);
			}
			if (m.tiles[idx].bound_horiz > 0) {
				printf("%d %d bound type %d dir 1\n",
					x + global_x,
					y + global_y,
					m.tiles[idx].bound_horiz - 1);
			}
			if (m.tiles[idx].bound_diag > 0 &&
			    m.tiles[idx].bound_diag < JAG_MAP_DIAG_INVERSE) {
				printf("%d %d bound type %d dir 2\n",
					x + global_x,
					y + global_y,
					m.tiles[idx].bound_diag - 1);
			}
			if (m.tiles[idx].bound_diag > JAG_MAP_DIAG_INVERSE) {
				printf("%d %d bound type %d dir 3\n",
					x + global_x,
					y + global_y,
					m.tiles[idx].bound_diag -
					    JAG_MAP_DIAG_INVERSE - 1);
			}
			if (m.tiles[idx].roof > 0) {
				printf("%d %d roof type %d\n",
					x + global_x,
					y + global_y,
					m.tiles[idx].roof - 1);
			}
			if (m.tiles[idx].overlay > 0) {
				printf("%d %d floor type %d\n",
					x + global_x,
					y + global_y,
					m.tiles[idx].overlay - 1);
			}
		}
	}
	for (int x = 0; x < JAG_MAP_CHUNK_SIZE; ++x) {
		for (int y = 0; y < JAG_MAP_CHUNK_SIZE; ++y) {
			int idx = y + x * JAG_MAP_CHUNK_SIZE;
			printf("%d %d metadata %d\n",
				x + global_x,
				y + global_y,
				m.tiles[idx].loc_direction);
		}
	}
	return 0;
}
