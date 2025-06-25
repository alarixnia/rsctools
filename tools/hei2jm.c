#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <map.h>

static void *
read_file_full_bin(const char *path, size_t *len)
{
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	uint8_t *buffer = malloc(length);
	if (buffer == NULL) {
		fclose(file);
		return NULL;
	}

	fread(buffer, 1, length, file);
	fclose(file);

	if (len != NULL) {
	    *len = (size_t)length;
	}

	return buffer;
}

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
	void *hei_data, *dat_data;
	uint8_t jm_data[JAG_MAP_JM_FILE_LEN];
	size_t l1, l2;

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

	if (argc < 3) {
		usage();
	}

	hei_data = read_file_full_bin(argv[0], &l1);
	if (hei_data == NULL) {
		fprintf(stderr, "read map .hei failed: %s\n", strerror(errno));
		return 1;
	}

	dat_data = read_file_full_bin(argv[1], &l2);
	if (dat_data == NULL) {
		fprintf(stderr, "read map .dat failed: %s\n", strerror(errno));
		return 1;
	}

	if (jag_map_read_hei(&m, hei_data, l1) != 0) {
		fprintf(stderr, "parse map .hei failed\n");
		return 1;
	}

	if (jag_map_read_dat(&m, dat_data, l2, version) != 0) {
		fprintf(stderr, "parse map .dat failed\n");
		return 1;
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
