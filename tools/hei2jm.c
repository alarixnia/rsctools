#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <map.h>
#include "utility.h"

#ifndef PLANE_LEVEL_INC
#define PLANE_LEVEL_INC (944)
#endif

static void __dead
usage(void)     
{                       
	fputs("hei2jm [-v version] hei dat jm\n", stderr);
	exit(1);
}

int main(int argc, char **argv)
{
	FILE *out;
	char ch;
	int version = 63;
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
		fprintf(stderr, "read map .hei failed: %s\n", strerror(errno));
		return 1;
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

	if (jag_map_read_hei(&m, hei_data, l1) != 0) {
		fprintf(stderr, "parse map .hei failed\n");
		return 1;
	}

	if (jag_map_read_dat(&m, dat_data, l2, version) != 0) {
		fprintf(stderr, "parse map .dat failed\n");
		return 1;
	}

	if (loc_path != NULL) {
		int ret;
		int x, y, id;
		FILE *f;

		f = fopen(loc_path, "r");
		if (f == NULL) {
			fprintf(stderr,
			    "open loc txt failed: %s\n", strerror(errno));
			return 1;
		}
		for (;;) {
			ret = fscanf(f, "%d %d %d\n", &x, &y, &id);
			if (ret == 0 || ret == EOF) {
				break;
			}
			x -= global_x;
			y -= global_y;
			if (x > 0 && y > 0 &&
			    x < JAG_MAP_CHUNK_SIZE && y < JAG_MAP_CHUNK_SIZE) {
				m.tiles[y + x * JAG_MAP_CHUNK_SIZE].bound_diag =
				    JAG_MAP_DIAG_LOC + id + 1;
			}
		}
		fclose(f);
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
