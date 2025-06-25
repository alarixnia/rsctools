#include <stdio.h>
#include <stdlib.h>

void *
read_file_full(const char *path, size_t *len)
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
