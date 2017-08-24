#include "..\include\graphics\load.h"
#include "..\include\graphics\Vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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

	size_t vsize = attrib.vertices.size() / 3;
	Vertex *verts = new Vertex[vsize];

	for (size_t ii = 0; ii < vsize; ++ii)
	{
		const float *p = &attrib.vertices[ii * 3];
		verts[ii].position = { p[0], p[1], p[2], 1 };
	}

	size_t isize = shapes[0].mesh.indices.size();
	size_t *indices = new unsigned[isize];
	for (size_t ii = 0; ii < isize; ++ii)
		indices[ii] = shapes[0].mesh.indices[ii].vertex_index;

	retval = MakeGeometry(verts, vsize, indices, isize);

	delete[] verts;
	delete[] indices;

	return retval;
}

//Shader loadShader(const char *vpath, const char *fpath)
//{
//	Shader retval = { 0 };
//
//	//const char *vsource;
//	//const char *fsource;
//
//	//retval = makeShader(vsource, fsource);
//
//	return retval;
//}