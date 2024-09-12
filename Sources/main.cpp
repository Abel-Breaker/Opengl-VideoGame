#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <iostream>

#include "../Headers/Manager.h"
#include "../Headers/Frame.h"

unsigned int SCR_WIDTH;
unsigned int SCR_HEIGHT;

/* ########### VARIABLES ########### */

//Actualización
double lastTime = glfwGetTime(); 
double currentTime;
double lapsoTime = 0;
int nbFrames = 0;
double lapsoDisparo = 1; //Tiempo entre disparos
double currentTimeDisparo;
double lastTimeDisparo = glfwGetTime();

/* ########### FUNCIONES ########### */
//SHADERS
extern GLuint setShaders(const char *nVertx, const char *nFrag);

//ILUMINACIÓN
void iluminacion();



void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0); //TODO: Mejor así?
	//Individual o global?
	//glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA
	//glEnable(GL_MULTISAMPLE); //Hacer disable que se quedó guardado
}

int main()
{
	
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//srand(time(NULL));
	 
	//Obtenemos la resolución de la pantalla primaria para poder hacer *PANTALLA COMPLETA CON BORDES*
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//SCR_WIDTH = mode->width;
	//SCR_HEIGHT = mode->height;
	SCR_WIDTH = 1920;
	SCR_HEIGHT = 1080;

	//Creo la ventana									
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyGame", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Camera::rescaling_CallBack);
	glfwSetCursorPosCallback(window, Controls::cursor_position_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 
	// 
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	openGlInit();

	//CARGA DE OBJETOS///////////////////////////////////////////
	Manager manager;
	manager.loader();
	
	// Lazo de la ventana
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		Controls::processInput(window);

		glClearColor(0.623f, 0.792f, 0.875f, 1.0f); //Borro el Buffer the la ventana
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		manager.run();

		Frame::tiempo(window);

		// glfw: swap 
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}


//CONFIGURACIÓN DE LA LUZ
void iluminacion() {
	//Para este juego la iluminación no tiene mucha importancia
	/* APLICAMOS INTENSIDAD de la LUZ AMBIENTE*/
	/*
	unsigned int ambientILoc = glGetUniformLocation(shaderProgram, "ambientI");
	glUniform1f(ambientILoc, 0.01);

	/* APLICAMOS COLOR de la LUZ AMBIENTE
	unsigned int colorLightLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3fv(colorLightLoc, 1, glm::value_ptr(glm::vec3(0.01, 0.01, 0.01)));*/
}


//Calculo de la hitbox
/*
void objetos_tocandose(objeto* obj1, objeto* obj2) {
	if (!((sqrt((obj1->px - obj2->px) * (obj1->px - obj2->px) + (obj1->py - obj2->py) * (obj1->py - obj2->py) + (obj1->pz - obj2->pz) * (obj1->pz - obj2->pz))) > obj1->radio + obj2->radio)) {

	}
}*/

