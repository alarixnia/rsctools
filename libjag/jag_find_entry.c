#include "jag.h"
#include "buffer.h"

int
jag_find_entry(struct jag_archive *archive, const char *name,
    struct jag_entry *out)
{
	struct jag_entry entry;
	unsigned i;
	uint32_t target_hash;
	uint16_t num_entries;
	size_t offset;

	if (jag_getu16(archive->data, 0,
	    archive->unpacked_len, &num_entries) != 0) {
		return -1;
	}
	offset = JAG_PER_FILE_METADATA_START +
	    (num_entries * JAG_ENTRY_HEADER_SIZE);
	target_hash = jag_hash_entry_name(name);
	for (i = 0; i < num_entries; ++i) {
		if (jag_read_entry(archive->data,
		    JAG_PER_FILE_METADATA_START + (JAG_ENTRY_HEADER_SIZE * i),
		    archive->unpacked_len, &entry) != 0) {
			return -1;
		}
		offset += entry.packed_len;
		if (entry.name_hash != target_hash) {
			continue;
		}
		entry.data = archive->data + offset;
		*out = entry;
		return 0;
	}
	return -1;
}
