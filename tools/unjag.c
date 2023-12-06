#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <buffer.h>
#include <jag.h>
#include <known_hash.h>

static void list_archive(FILE *);

static void
list_archive(FILE *f)
{
	struct jag_archive archive;
	struct jag_entry entry;
	uint16_t num_entries;
	const char *s;
	unsigned i;

	if (jag_unpack_file(f, &archive) != 0) {
		fprintf(stderr, "unjag: failed to unpack archive\n");
		exit(EXIT_FAILURE);
	}

	if (jag_getu16(archive.data, 0,
	    archive.unpacked_len, &num_entries) != 0) {
		fprintf(stderr, "unjag: archive has no entry number header\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < num_entries; ++i) {
		if (jag_read_entry(archive.data,
		    JAG_PER_FILE_METADATA_START + (i * JAG_ENTRY_HEADER_SIZE),
		    archive.unpacked_len, &entry) == -1) {
			break;
		}
		if ((s = jag_get_known_name(entry.name_hash)) != NULL) {
			printf("%s\n", s);
		} else {
			printf("%d\n", entry.name_hash);
		}
	}

	if (archive.must_free) {
		free(archive.data);
		archive.data = NULL;
	}
}

int
main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	FILE *f = NULL;
	int ch;
	int lflag = 0;

	while ((ch = getopt(argc, argv, "l")) != -1) {
		switch (ch) {
	    	case 'l':
			lflag = 1;
			break;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc < 1) {
		fprintf(stderr, "unjag: no file name specified\n");
		return EXIT_FAILURE;
	}

	if ((f = fopen(argv[0], "rb")) == NULL) {
		fprintf(stderr, "unjag: failed to open %s: %s\n",
		    argv[0], strerror(errno));
		return EXIT_FAILURE;
	}

	if (lflag) {
		list_archive(f);
		fclose(f);
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
