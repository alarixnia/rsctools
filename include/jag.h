#ifndef JAG_H
#define JAG_H
#include <stdint.h>
#include <stddef.h>

/* low level functions */
uint32_t jag_hash_entry_name(const char *);
size_t jag_get_entry(void *, size_t, size_t, const char *);
size_t jag_get_next_entry(void *, size_t, size_t);

#endif
