#pragma once

#include "RenderObjects.h"

struct GLFWwindow;

class Context
{
private:
	GLFWwindow *handle;

public:
	// Open up a window in the window os
	bool init(size_t w = 800, size_t h = 600, const char *title = "Graphics");

	// Updating the window, determines the FPS
	bool step();

	// Tell the window to close
	bool term();

	bool getKey(int key);
	bool getMouseButtonLeft();
	void getMousePosition(double &x_out, double &y_out);
	double getTime();
};
