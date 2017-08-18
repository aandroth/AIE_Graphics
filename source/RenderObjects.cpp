
#include "..\include\graphics\Vertex.h"
#include "..\include\graphics\RenderObjects.h"
#include "glinc.h"
#ifdef _DEBUG
#include <iostream>
#endif // DEBUG

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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);

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

