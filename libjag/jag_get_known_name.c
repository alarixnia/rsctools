#include <jag.h>
#include <known_hash.h>
#include "jag_known_hashes.c"

const char *
jag_get_known_name(uint32_t hash)
{
	unsigned i;
	const size_t num = sizeof(jag_known_hashes) /
	    sizeof(struct jag_known_hash);

	for (i = 0; i < num; ++i) {
		if (jag_known_hashes[i].hash == hash) {
			return jag_known_hashes[i].name;
		}
	}
	return NULL;
}
