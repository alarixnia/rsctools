#ifndef JAG_KNOWN_HASH_H
#define JAG_KNOWN_HASH_H
#include <stdint.h>

const char *jag_get_known_name(uint32_t);
uint32_t jag_get_known_hash(const char *);

#endif
