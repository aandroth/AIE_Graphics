#include "..\include\graphics\Context.h"
#include "glinc.h"

bool Context::init(size_t w, size_t h, const char * title)
{
	glfwInit();

	handle = glfwCreateWindow(w = 800, h = 600, title = "Graphics Window", nullptr, nullptr);
	glfwMakeContextCurrent(handle);

	glewExperimental = true;
	glewInit();

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
