#include <jag.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char dictionary[] = "abcdefghijklmnopqrstuvwxyz 0123456789";

static void crack(uint32_t, const char *);
static void guess_letter(uint32_t, char *, const char *,
    size_t, unsigned, size_t, int);
static void usage(void);

static int tries = 0;

static void
guess_letter(uint32_t hash, char *filename,
    const char *extension, size_t extension_len,
    unsigned name_idx, size_t max_len, int allowdigit)
{
	size_t num_letters = (sizeof(dictionary) - 1);
	size_t i, j;
	uint32_t new_hash;

	for (i = 0; i < num_letters; ++i) {
		if (isdigit((unsigned char)dictionary[i]) && !allowdigit) {
			break;
		}
		if (dictionary[i] == ' ' &&
		    (name_idx == 0 || filename[name_idx - 1] == ' ')) {
			continue;
		}
		filename[name_idx] = dictionary[i];
		memcpy(filename + name_idx + 1, extension, extension_len);
		if ((tries % 1000000) == 0) {
			fprintf(stderr,
			    "jag-crack: %d attempts %s\n", tries, filename);
		}
		tries++;
		new_hash = jag_hash_entry_name(filename);
		if (hash == new_hash) {
			printf("FOUND MATCH\n");
			printf("%u = %s\n", (unsigned int)new_hash, filename);
			exit(0);
		}
		if (name_idx > 0) {
			for (j = 0; j < name_idx; ++j) {
				guess_letter(hash, filename, extension,
				    0, j, name_idx, 0);
			}
		}
	}
}

static void
crack(uint32_t hash, const char *extension)
{
	char filename[32];
	unsigned i;
	size_t extension_len;
	size_t max_len;

	extension_len = strlen(extension);
	max_len = sizeof(filename) - 1 - extension_len;
	for (i = 0; i < max_len; ++i) {
		guess_letter(hash, filename, extension,
		    extension_len + 1, i, max_len, 1);
	}
}

static void
usage(void)
{
	fprintf(stderr, "jag-crack: usage: jag-crack hash extension\n");
	exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
	long long hash;

	if (argc < 3) {
		usage();
	}

	errno = 0;
	hash = strtoll(argv[1], NULL, 10);
	if (errno != 0) {
		usage();
	}

	crack(hash, argv[2]);
	return EXIT_SUCCESS;
}
