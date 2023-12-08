#ifndef JAG_BUF_H
#define JAG_BUF_H

#include <stdint.h>
#include <stddef.h>

int jag_getu8(void *, size_t, size_t, uint8_t *);

int jag_getu16(void *, size_t, size_t, uint16_t *);

int jag_getu24(void *, size_t, size_t, uint32_t *);

int jag_getu32(void *, size_t, size_t, uint32_t *);

int jag_putu8(void *, size_t, size_t, uint8_t);

int jag_putu16(void *, size_t, size_t, uint16_t);

int jag_putu24(void *, size_t, size_t, uint32_t);

int jag_putu32(void *, size_t, size_t, uint32_t);

#endif
