#pragma once

#include <glfw3.h>

class Camera
{
private:
	static int width, height;

public:
	static void updateCamera(unsigned int shaderProgram);
	static void rescaling_CallBack(GLFWwindow* window, int new_width, int new_height);
};

