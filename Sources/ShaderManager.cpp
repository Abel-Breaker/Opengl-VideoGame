#include "../Headers/ShaderManager.h"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS //Evitamos alguno errores de funciones no seguras
#endif
#pragma warning(disable:4996) //Evitamos alguno errores de funciones no seguras

#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>



char* ShaderManager::textFileRead(const char* fn) {

	FILE* fichero;
	char* contenido = NULL;

	int count = 0;

	if (fn != NULL) {
		fichero = fopen(fn, "rt");

		if (fichero != NULL) {

			fseek(fichero, 0, SEEK_END);
			count = ftell(fichero);
			rewind(fichero);

			if (count > 0) {
				contenido = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(contenido, sizeof(char), count, fichero);
				contenido[count] = '\0';
			}
			fclose(fichero);
		}
		else {

			printf("Error al abrir el fichero %s\n", fn);
			return NULL;
		}
	}

	return contenido;
}

// Nos indica e imprime por pantalla si hay algun error al crear el shader o el program

void ShaderManager::printInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}



GLuint ShaderManager::setAndGetShader(const char* vertexPath, const char* fragmentPath) {

	GLuint vertexShader;
	GLuint fragmentShader; // Definicion  vertex y fragment shader
	GLuint progShader;
	
	char* ficherovs = NULL;
	char* ficherofs = NULL;
	const char* codigovs = NULL;
	const char* codigofs = NULL;

	//Creo el vertexShader y el FragmentShader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Leo el codigo del ficheo y hay error devuelve un NULL
	ficherovs = textFileRead(vertexPath);
	if (ficherovs == NULL) exit(0);
	ficherofs = textFileRead(fragmentPath);
	if (ficherofs == NULL) exit(0);

	//Lo igual al puntero para cargarlos
	codigovs = ficherovs;
	codigofs = ficherofs;
	//Los cargo
	glShaderSource(vertexShader, 1, &codigovs, NULL);
	glShaderSource(fragmentShader, 1, &codigofs, NULL);

	//Libero los ficheros
	free(ficherovs); free(ficherofs);

	// Copio vertex y Fragment
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//Miro si hay algun error
	printInfoLog(vertexShader);
	printInfoLog(fragmentShader);

	//Creo el programa asociado
	progShader = glCreateProgram();

	// Le attacheo shaders al programa
	glAttachShader(progShader, vertexShader);
	glAttachShader(progShader, fragmentShader);

	// Lo linko
	glLinkProgram(progShader);
	// A ver si hay errores
	printInfoLog(progShader);

	// Limpieza de shaders, ya que están vinculados al programa
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);

	return (progShader);

}
/*
std::string ShaderManager::readFile(const char* filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error: No se pudo abrir el archivo " << filePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}*/
/*
GLuint ShaderManager::getProgramShader() {
	return this->progShader;
}*/