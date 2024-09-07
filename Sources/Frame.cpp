#include "../Headers/Frame.h"
#include <string>

double Frame::lapsoTime = 0;
double Frame::currentTime = 0;
int Frame::nbFrames = 0;
double Frame::lastTime = 0;

//FUNCIONES ACTUALIZACIÓN (TIEMPO) Y FPS
void Frame::tiempo(GLFWwindow* window) {
	static float unSegundo = 0;
	currentTime = glfwGetTime();
	nbFrames++;
	lapsoTime = currentTime - lastTime;
	unSegundo += lapsoTime;
	if (unSegundo >= 1.0) {
		std::string newTitle = "MyGame - FPS: " + std::to_string((double)nbFrames);
		glfwSetWindowTitle(window, newTitle.c_str());
		//printf("%f frames/s\n", double(nbFrames));
		nbFrames = 0;
		unSegundo = 0;
	}
	lastTime = currentTime; 
}

double Frame::getLapsoTime() {
	return lapsoTime;
}