#pragma once

class GLFWwindow;

class Context
{
private:
	GLFWwindow *handle;

public:
	// Open up a window in the window os
	bool init(size_t w, size_t h, const char *title = "Graphics");

	// Updating the window, determines the FPS
	bool step();

	// Tell the window to close
	bool term();
};
