#include <jag.h>
#include <known_hash.h>
#include <string.h>
#include "jag_known_hashes.c"

uint32_t
jag_get_known_hash(const char *name)
{
	unsigned i;
	const size_t num = sizeof(jag_known_hashes) /
	    sizeof(struct jag_known_hash);

	for (i = 0; i < num; ++i) {
		if (strcmp(jag_known_hashes[i].name, name) == 0) {
			return jag_known_hashes[i].hash;
		}
	}
	return 0;
}
