#include "..\include\graphics\Context.h"

#ifdef _DEBUG
#include <iostream>
void APIENTRY GL_errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message,
	const void *userParam)
{
	std::cerr << message << std::endl;
}
#endif

bool Context::init(size_t w, size_t h, const char * title)
{
	glfwInit();

	handle = glfwCreateWindow(w = 800, h = 600, title = "Graphics Window", nullptr, nullptr);
	glfwMakeContextCurrent(handle);

	glewExperimental = true;
	glewInit();

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(GL_errorCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);
#endif


	return true;
}

bool Context::step()
{
	glfwPollEvents();
	glfwSwapBuffers(handle);
	return !glfwWindowShouldClose(handle);
}

bool Context::term()
{
	glfwTerminate();
	handle = nullptr;
	return true;
}

bool Context::getKey(int key)
{
	return true;// (glfwGetKey(handle, glfwGetKey) == );
}

bool Context::getMNouseButton(int button)
{
	return false;
}

void Context::getMousePosition(double & x_out, double & y_out)
{
}

double Context::getTime()
{
	return glfwGetTime();
}
