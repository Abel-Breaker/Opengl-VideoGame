#pragma once

#include <glfw3.h>

class Controls
{
public:
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void processInput(GLFWwindow* window);
};

