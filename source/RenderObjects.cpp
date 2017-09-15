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
	glm::vec2 t2 = v2.texCoord - v0.texCoord;

	return glm::normalize((p1*t2.y - p2*t1.y) / (t1.x*t2.y - t1.y*t2.x));
	// UV.x will follow the tangent
	// UV.y will follow the bitangent
}

void SolveTangents(Vertex *vertices, size_t vsize,
	const unsigned *idxs, size_t isize)
{
	for (int ii = 0; ii+2 < isize; ii+=3)
	{
		glm::vec4 T = CalcTangent(vertices[idxs[ii]], vertices[idxs[ii+1]], vertices[idxs[ii+2]]);

		for (int jj = 0; jj < 3; ++jj)
		{
			vertices[idxs[ii + jj]].tangent = glm::normalize(T + vertices[idxs[ii + jj]].tangent);
		}
	}

	// Get the bitangent
	for (int ii = 0; ii < vsize; ++ii)
	{
		vertices[ii].bitangent = glm::vec4(glm::cross(vertices[ii].tangent.xyz(), vertices[ii].normal.xyz()), 0);
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

Texture makeTexture(unsigned w, unsigned h, unsigned c, const void * pixels, bool isFloat)
{
	Texture retVal = { 0 };

	// External and internal format for 
	GLenum f = 0, i = 0;
	switch (c)
	{
	case 0: f = GL_DEPTH_COMPONENT; i = GL_DEPTH24_STENCIL8; break;
	case 1: f = GL_RED;  i = GL_R32F;    break;
	case 2: f = GL_RG;   i = GL_RG32F;   break;
	case 3: f = GL_RGB;  i = GL_RGB32F;  break;
	case 4: f = GL_RGBA; i = GL_RGBA32F; break;
	}

	glGenTextures(1, &retVal.handle);
	glBindTexture(GL_TEXTURE_2D, retVal.handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ((isFloat || c==0) ? i : f), w, h, 0, f, GL_UNSIGNED_BYTE, (isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE, pixels));
	glBindTexture(GL_TEXTURE_2D, 0);

	return retVal;
}

void freeTexture(Texture & t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0 };
}

Framebuffer makeFrameBuffer(unsigned w, unsigned h, unsigned c, bool hasDepth, unsigned nTargets, unsigned nFloatTargets)
{
	Framebuffer retval = { 0, w, h, 0, 0,{ 0 } };
	retval.nTargets = nTargets + nFloatTargets;

	glGenFramebuffers(1, &retval.handle);
	glBindFramebuffer(GL_FRAMEBUFFER, retval.handle);

	if (hasDepth)
	{
		retval.depthTarget = makeTexture(w, h, 0, 0, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, retval.depthTarget.handle, 0);
	}

	const GLenum attachments[8] =
	{
		GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4,GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6,GL_COLOR_ATTACHMENT7
	};

	for (int ii = 0; ii < retval.nTargets && ii < 8; ++ii)
	{
		retval.targets[ii] = makeTexture(w, h, c, 0, (ii >= nTargets));
		glFramebufferTexture(GL_FRAMEBUFFER, attachments[ii], retval.targets[ii].handle, 0);
	}

	glDrawBuffers(retval.nTargets, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return retval;
}