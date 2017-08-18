#pragma once

#include "glm\glm.hpp"

struct Vertex;
struct Geometry
{
	unsigned handle, vbo, ibo, size;
};

Geometry MakeGeometry(const Vertex *vertices, size_t vsize,
	const unsigned *indices, size_t isize);

void freeGeometry(Geometry &g);

struct Shader
{
	unsigned handle;
};

Shader makeShader(const char *vsource, const char *fsource);
void freeShader(Shader &s);

struct Framebuffer
{
	unsigned handle;
	unsigned width, height;
};