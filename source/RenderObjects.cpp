#define GLM_FORCE_SWIZZLE

#include "..\include\graphics\Vertex.h"
#include "..\include\graphics\RenderObjects.h"
#ifdef _DEBUG
#include <iostream>
#endif // DEBUG


// Not defined in header b/c it isd a helper fn for SolveTangents
glm::vec4 CalcTangent(const Vertex &v0, const Vertex &v1, const Vertex &v2)
{
	glm::vec4 p1 = v1.position - v0.position;
	glm::vec4 p2 = v2.position - v0.position;

	glm::vec2 t1 = v1.texCoord - v0.texCoord;
	glm::vec2 t2 = v1.texCoord - v0.texCoord;

	return glm::normalize((p1*t2.y - p2*t1.y) / (t1.x*t2.y - t1.y*t2.x));
	// UV.x will follow the tangent
	// UV.y will follow the bitangent
}

void SolveTangents(Vertex *v, size_t vsize,
	const unsigned *idxs, size_t isize)
{
	int jj = 0;
	for (int ii = 0; ii < vsize; ii+=3)
	{
		glm::vec4 T = CalcTangent(v[idxs[ii]], v[idxs[ii+1]], v[idxs[ii+2]]);

		for (int jj = 0; jj < vsize; ++jj)
		{
			v[idxs[ii + jj]].tangent = glm::normalize(T + v[idxs[ii + jj]].tangent);
		}
	}

	// Get the bitangent
	for (int ii = 0; ii < vsize; ++ii)
	{
		v[ii].bitangent = glm::vec4(glm::cross(v[ii].tangent.xyz(), v[ii].normal.xyz()), 0);
	}
}

Geometry MakeGeometry(const Vertex *vertices, size_t vsize, 
	const unsigned * indices, size_t isize)
{
	Geometry retVal = { 0, 0, 0, isize };

	glGenBuffers(1, &retVal.vbo);
	glGenBuffers(1, &retVal.ibo);
	glGenVertexArrays(1, &retVal.handle);

	glBindVertexArray(retVal.handle);
	glBindBuffer(GL_ARRAY_BUFFER, retVal.vbo);
	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retVal.ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize * sizeof(unsigned), indices, GL_STATIC_DRAW);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);

	// texCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// normals
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)40);

	// tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)56);

	// bitangent
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)72);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return retVal;
}

void freeGeometry(Geometry & g)
{
	glDeleteBuffers(1, &g.vbo);
	glDeleteBuffers(1, &g.ibo);
	glDeleteVertexArrays(1, &g.handle);
	g = { 0, 0, 0, 0 };
}

Shader makeShader(const char * vsource, const char * fsource)
{

	Shader retVal = { 0 };

	retVal.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsource, 0);
	glShaderSource(fs, 1, &fsource, 0);
	glCompileShader(vs);
#ifdef _DEBUG
	GLint vsuccess = GL_FALSE;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &vsuccess);
	if (vsuccess == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(vs, length, 0, log);
		std::cerr << log << std::endl;
		delete[] log;
	}
#endif // DEBUG

	glCompileShader(fs);
#ifdef _DEBUG
	GLint fsuccess = GL_FALSE;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &fsuccess);
	if (fsuccess == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(fs, length, 0, log);
		std::cerr << log << std::endl;
		delete [] log;
	}
#endif // DEBUG

	glAttachShader(retVal.handle, vs);
	glAttachShader(retVal.handle, fs);
	glLinkProgram(retVal.handle);
#ifdef _DEBUG
	GLint success = GL_FALSE;
	glGetProgramiv(retVal.handle, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(retVal.handle, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetProgramInfoLog(retVal.handle, length, 0, log);
		std::cerr << log << std::endl;
		delete [] log;
	}
#endif // DEBUG

	glDeleteShader(vs);
	glDeleteShader(fs);

	return retVal;
}

void freeShader(Shader &s)
{
	glDeleteProgram(s.handle);
	s = {0};
}

Texture makeTexture(unsigned w, unsigned h, unsigned c, const unsigned char * pixels)
{
	Texture retVal = { 0 };

	unsigned f = 0;
	switch (c)
	{
	case 1: f = GL_RED; break;
	case 2: f = GL_RG; break;
	case 3: f = GL_RGB; break;
	case 4: f = GL_RGBA; break;
	}

	glGenTextures(1, &retVal.handle);
	glBindTexture(GL_TEXTURE_2D, retVal.handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, f, w, h, 0, f, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	return retVal;
}

void freeTexture(Texture & t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0 };
}

