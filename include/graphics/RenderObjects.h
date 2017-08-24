#pragma once

#include "glinc.h"
#include "glm\geometric.hpp"
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"

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

struct Texture
{
	unsigned handle;
};

Texture makeTexture(unsigned w, unsigned h, unsigned c, const unsigned char *pixels);

void freeTexture(Texture &t);