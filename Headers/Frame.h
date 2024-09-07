#pragma once
#include <glfw3.h>

class Frame
{
private:
	static double lapsoTime;
	static double currentTime;
	static int nbFrames;
	static double lastTime;

public:
	static void tiempo(GLFWwindow* window);
	static double getLapsoTime();
};

