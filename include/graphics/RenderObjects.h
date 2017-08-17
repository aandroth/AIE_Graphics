#pragma once

#include "glm\glm.hpp"

struct Geometry
{
	unsigned handle, vbo, ibo, size;
};

Geometry MakeGeometry(const Vertex *vertices, size_t vsize,
	const unsigned *indices, size_t isize);

void freeGeometry(Geometry &g);