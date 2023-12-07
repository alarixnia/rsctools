#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <buffer.h>
#include <jag.h>
#include <known_hash.h>

static int lflag = 0;
static int vflag = 0;
static char *target_path = NULL;

static void extract_archive(FILE *);

static void
extract_archive(FILE *f)
{
	struct jag_archive archive;
	struct jag_entry entry;
	uint16_t num_entries;
	const char *s;
	char filename[PATH_MAX];
	unsigned i;
	size_t packed_off;

	if (jag_unpack_file(f, &archive) != 0) {
		fprintf(stderr, "unjag: failed to unpack archive\n");
		exit(EXIT_FAILURE);
	}

	if (vflag) {
		if (archive.packed_len == archive.unpacked_len) {
			fprintf(stderr,
			    "unjag: looks like an uncompressed archive\n");
		} else {
			fprintf(stderr,
			    "unjag: looks like a compressed archive\n");
		}
	}

	if (jag_getu16(archive.data, 0,
	    archive.unpacked_len, &num_entries) != 0) {
		fprintf(stderr, "unjag: archive has no entry number header\n");
		exit(EXIT_FAILURE);
	}

	if (vflag) {
		fprintf(stderr,
		    "unjag: archive has %d entries\n", num_entries);
	}

	packed_off = JAG_PER_FILE_METADATA_START +
	    (num_entries * JAG_ENTRY_HEADER_SIZE);

	for (i = 0; i < num_entries; ++i) {
		if (jag_read_entry(archive.data,
		    JAG_PER_FILE_METADATA_START +
			(i * JAG_ENTRY_HEADER_SIZE),
		    archive.unpacked_len, &entry) == -1) {
			break;
		}
		s = jag_get_known_name(entry.name_hash);

		if (lflag) {
			if (s != NULL) {
				printf("%s\n", s);
			} else {
				printf("%u\n", entry.name_hash);
			}
			continue;
		}

		if (target_path != NULL) {
			if (s == NULL) {
				snprintf(filename, sizeof(filename),
				    "%s/%d", target_path, entry.name_hash);
			} else {
				snprintf(filename, sizeof(filename),
				    "%s/%s", target_path, s);
			}
		} else {
			if (s == NULL) {
				snprintf(filename, sizeof(filename),
				    "%d", entry.name_hash);
			} else {
				snprintf(filename, sizeof(filename),
				    "%s", s);
			}
		}

		if (vflag) {
			fprintf(stderr, "unjag: unpacking %s\n", filename);
		}

		entry.data = archive.data + packed_off;
		entry.must_free = 0;

		packed_off += entry.packed_len;

		if (jag_unpack_entry(&entry) == -1) {
			fprintf(stderr,
			    "unjag: failed to unpack entry %d\n", i);
			exit(EXIT_FAILURE);
			break;
		}

		FILE *out = fopen(filename, "wb");
		if (out == NULL) {
			fprintf(stderr,
			    "unjag: failed to open %s for writing: %s\n",
			    filename, strerror(errno));
			exit(EXIT_FAILURE);
		}

		fwrite(entry.data, 1, entry.unpacked_len, out);
		if (entry.must_free) {
			free(entry.data);
			entry.data = NULL;
		}
		fclose(out);
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
	char ch;
	int fd;
	mode_t mode;

	while ((ch = getopt(argc, argv, "d:lv")) != -1) {
		switch (ch) {
		case 'd':
			target_path = optarg;
			fd = open(target_path, O_RDWR);
			if (fd == -1) {
				mode = (S_IRWXU | S_IRWXG | S_IRWXO) & ~umask(0);
				mode = mode | S_IWUSR | S_IXUSR;
				if (mkdir(target_path, mode) == -1) {
					fprintf(stderr,
					    "unjag: failed to create %s: %s\n",
					    target_path, strerror(errno));
					return EXIT_FAILURE;
				}
			} else {
				close(fd);
			}
			break;
	    	case 'l':
			lflag = 1;
			break;
		case 'v':
			vflag = 1;
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

	extract_archive(f);
	fclose(f);
	return EXIT_SUCCESS;
}
