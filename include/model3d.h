#ifndef JAG_MODEL3D_H
#define JAG_MODEL3D_H
#include <stddef.h>
#include <stdint.h>

struct jag_vertex3d {
	int16_t x, y, z;
};

struct jag_face3d {
	uint16_t num_vertices;
	int16_t fill_front;
	int16_t fill_back;
	uint8_t face_gouraud;
	struct jag_vertex3d vertices[4];
};

struct jag_model3d {
	uint16_t num_vertices
	uint16_t num_faces;
	struct jag_vertex3d *vertices;
	struct jag_face3d *faces;
};

int jag_model_load_ob3(struct jag_model3d *, void *, size_t);

#endif
