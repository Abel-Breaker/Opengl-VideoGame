#include "../Headers/Grass.h"
#include <glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include "../Headers/ShaderManager.h"
#include "../Headers/Camera.h"

void Grass::prepareGrass(int numberOfInstances, Terrain *terrain) {
	this->shaderProgram = ShaderManager::setAndGetShader("C:/Users/miste/AAAJuego/Opengl-VideoGame/GameCore/Shaders/grassShader.vert", "C:/Users/miste/AAAJuego/Opengl-VideoGame/GameCore/Shaders/grassShader.frag");
	this->numberOfIndices = numberOfInstances * 3; //*3 pq son 3 ejes
	this->randomLocations = (float *)malloc(numberOfIndices * sizeof(float)); 
	this->maximumSizeToSpawn = terrain->getSize() - 1; //-1 Ya que para acceder al array con los mesh points si es 150 va de 0 a 149 los indices
	this->terrain = terrain;
	generateMeshPoints();
	generateRandomLocations();
	createVao();
	bindBufferGrassLocation();
	loadTexture();
}

void Grass::generateRandomLocations() {
	srand(time(NULL));

	for (int i = 0; i < this->numberOfIndices/3; i ++) {
		this->randomLocations[i*3] = ((float)rand() / RAND_MAX) * this->maximumSizeToSpawn;
		this->randomLocations[i * 3 + 2] = ((float)rand() / RAND_MAX) * this->maximumSizeToSpawn;
		this->randomLocations[i * 3 + 1] = this->terrain->getYLocation(this->randomLocations[i * 3], this->randomLocations[i * 3 + 2]);
	}
}

void Grass::generateMeshPoints() {
	this->meshPoints[0] = 0.0f;
	this->meshPoints[1] = 0.0f;
	this->meshPoints[2] = -0.5f;
	this->meshPoints[3] = 0.0f;
	this->meshPoints[4] = 0.0f;

	this->meshPoints[5] = 0.0f;
	this->meshPoints[6] = 0.0f;
	this->meshPoints[7] = 0.5f;
	this->meshPoints[8] = 1.0f;
	this->meshPoints[9] = 0.0f;

	this->meshPoints[10] = 0.0f;
	this->meshPoints[11] = 1.0f;
	this->meshPoints[12] = 0.5f;
	this->meshPoints[13] = 1.0f;
	this->meshPoints[14] = 1.0f;

	this->meshPoints[15] = 0.0f;
	this->meshPoints[16] = 0.0f;
	this->meshPoints[17] = -0.5f;
	this->meshPoints[18] = 0.0f;
	this->meshPoints[19] = 0.0f;

	this->meshPoints[20] = 0.0f;
	this->meshPoints[21] = 1.0f;
	this->meshPoints[22] = 0.5f;
	this->meshPoints[23] = 1.0f;
	this->meshPoints[24] = 1.0f;

	this->meshPoints[25] = 0.0f;
	this->meshPoints[26] = 1.0f;
	this->meshPoints[27] = -0.5f;
	this->meshPoints[28] = 0.0f;
	this->meshPoints[29] = 1.0f;
}

void Grass::createVao() {
	unsigned int VBO;

	/* INICIALIZAMOS VAO */
	glGenVertexArrays(1, &(this->VAO));
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first.
	glBindVertexArray(this->VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->meshPoints), this->meshPoints, GL_STATIC_DRAW);

	//Normales
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/* ELIMINAMOS VAOs*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void Grass::bindBufferGrassLocation() {
	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->numberOfIndices * sizeof(float), this->randomLocations, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

}

//CARGA DE TEXTURAS
void Grass::loadTexture() {
	// load and create a texture 
	// -------------------------
	stbi_set_flip_vertically_on_load(true);  // Invertir la imagen al cargar

	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("C:/Users/miste/AAAJuego/Opengl-VideoGame/GameCore/Textures/grass.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	}
	else
	{
		printf("Error al cargar la textura %s\n", "C:/Users/miste/AAAJuego/Opengl-VideoGame/GameCore/Textures/grass.png");
		//std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Grass::draw() {
	glDisable(GL_CULL_FACE);

	glUseProgram(this->shaderProgram);

	Camera::updateCamera(shaderProgram);

	/* APLICAMOS TEXTURA */
	glUniform1i(glGetUniformLocation(this->shaderProgram, "texture1"), 0);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	/* DIBUJAMOS */
	glBindVertexArray(this->VAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 30, this->numberOfIndices); //Aprovechamos el *3 de los ejes para que en cada hierba dibuje 3
	glBindVertexArray(0);

}