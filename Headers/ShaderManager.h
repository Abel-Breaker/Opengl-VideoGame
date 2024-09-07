#pragma once

#include <glad.h>
//#include <string>

class ShaderManager
{
private:
	static char* textFileRead(const char* fn);
	static void printInfoLog(GLuint obj);
public:
	
	static GLuint setAndGetShader(const char* vertexPath, const char* fragmentPath);
	//static GLuint getProgramShader();
	//static std::string readFile(const char* filePath);
};

