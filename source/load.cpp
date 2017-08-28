#include "..\include\graphics\load.h"
#include "..\include\graphics\Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <random>

glm::vec4 randColor()
{
	return{
		rand() / (float)RAND_MAX,
		rand() / (float)RAND_MAX,
		rand() / (float)RAND_MAX, 1 };
}

Texture loadTexture(const char *path)
{
	Texture retval;

	stbi_set_flip_vertically_on_load(true);

	int w, h, c;
	unsigned char *pixels;
	pixels = stbi_load(path, &w, &h, &c, STBI_default);

	retval = makeTexture(w, h, c, pixels);
	stbi_image_free(pixels);

	return retval;
}

Geometry loadGeometry(const char *path)
{
	Geometry retval = { 0, 0, 0, 0 };
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

	// We have to interleave our data
	// To do this, we have to change [pppp][nnnn][tttt]
	// to this, [pnt][pnt][pnt][pnt] into the vertices

	size_t isize = shapes[0].mesh.indices.size();
	size_t *indices = new unsigned[isize];

	size_t vsize = isize;
	Vertex *verts = new Vertex[vsize];


	for (size_t ii = 0; ii < vsize; ++ii)
	{
		indices[ii] = ii;
		
		int pi = shapes[0].mesh.indices[ii].vertex_index;
		int ni = shapes[0].mesh.indices[ii].normal_index;
		int ti = shapes[0].mesh.indices[ii].texcoord_index;

		const float *p = &attrib.vertices[pi * 3];
		const float *n = &attrib.normals[ni * 3];
		const float *t = &attrib.texcoords[ti * 2];

		verts[ii].position = { p[0], p[1], p[2], 1 };
		verts[ii].texCoord = { t[0], t[1] };
		verts[ii].normal   = { n[0], n[1], n[2], 0 };

		verts[ii].color = randColor();
	}

	retval = MakeGeometry(verts, vsize, indices, isize);

	delete[] verts;
	delete[] indices;

	return retval;
}

std::string readFile(const char *path)
{
	std::string content = "";
	std::ifstream ifStream(path, std::ios::in);

	if (!ifStream.is_open())
	{
		std::cerr << "Could not read the file. Either the file is unreadable or does not exist.\n";
		return "";
	}

	std::string line;
	while(!ifStream.eof())
	{
		std::getline(ifStream, line);
		content.append(line + "\n");
	}

	return content;
}

Shader loadShader(const char *vpath, const char *fpath)
{
	Shader retval = { 0 };

	std::string vstring = readFile(vpath);
	std::string fstring = readFile(fpath);
	const char *vsource = vstring.c_str();
	const char *fsource = fstring.c_str();

	retval = makeShader(vsource, fsource);

	return retval;
}