#include "../Headers/Terrain.h"
#include <glad.h>
#include <corecrt_malloc.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <ctime>
#include <math.h>
#include "../Headers/Camera.h"
#include "../Headers/ShaderManager.h"

extern unsigned int terrainShaderProgram;
//extern unsigned int shaderProgram;

void Terrain::prepareTerrain(int size, int terrainMeshDensity, short frequency, short amplitude) {
	shaderProgram = ShaderManager::setAndGetShader("GameCore/Shaders/terrainShader.vert", "GameCore/Shaders/terrainShader.frag");
	//glUniform1i(glGetUniformLocation(this->shader, "texture1"), 0);
	initializeAtributes(size, terrainMeshDensity, frequency, amplitude);
	generateMeshPoints();
	createVao();
	loadTexture();
}

void Terrain::initializeAtributes(int size, int terrainMeshDensity, short frequency, short amplitude) {
	this->size = size;
	this->terrainMeshDensity = terrainMeshDensity;
	this->frequency = frequency;
	this->amplitude = amplitude;
	//Number of squares multiply by points of each square
	//Multiply by meshDensity^2 (subdivision of each square in squares)
	this->numberOfIndices = ((size) * (size)) * (6 * terrainMeshDensity * terrainMeshDensity) * 3; // Cada 3 índices es una coordenada/vértice
	this->numberOfIndices += this->numberOfIndices * 2 / 3; //Texture coordinates
	this->meshPoints = (float*)malloc((this->numberOfIndices) * sizeof(float));
	this->VAO = 0;
	this->texture = 0;
}

int Terrain::getSize() {
	return this->size;
}

/*
 * La malla se organiza en un plano X-Z, donde cada cuadrado de la malla se subdivide en 
 * `terrainMeshDensity^2` cuadrados más pequeños.

 * Algoritmo:
 * - El bucle `i` itera sobre las filas (eje Z), subdividiendo cada fila según la densidad.
 * - El bucle `j` recorre las columnas (eje X).
 * - El bucle `k` subdivide cada columna, añadiendo vértices entre las posiciones enteras.
*/
void Terrain::generateMeshPoints() {
	int iFactor = this->size * this->terrainMeshDensity * (6*3 + 6*2), //6*2=textCoord
		jFactor = this->terrainMeshDensity * (6 * 3 + 6 * 2),
		kFactor = (6 * 3 + 6 * 2),
		arrayIndex;

	float relativeFrequency = (float)this->frequency / this->size;
	float asseX, asseZ;

	//Por cada Z (fila) aumenta 30 (vertices de un cuadrado) * size
	for (int i = 0; i < (this->size * this->terrainMeshDensity); i++) { //Coordenada Z (representa las filas)
		for (int j = 0; j < (this->size); j++) { //Coordenada X (representa las columnas)
			for (int k = 0; k < this->terrainMeshDensity; k++) { //Divide solo las columnas

				arrayIndex = (i * iFactor + j * jFactor + k * kFactor);

				asseX = j + ((float)k / this->terrainMeshDensity);
				asseZ = (float)i / this->terrainMeshDensity;
				this->meshPoints[arrayIndex] = asseX;
				this->meshPoints[arrayIndex + 1] = perlin(asseX*relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->meshPoints[arrayIndex + 2] = asseZ;
				setTextureCoordinates(arrayIndex + 3, 0, 0);

					
				asseX = j + ((k + 1.0f) / this->terrainMeshDensity);
				asseZ = (float)i / this->terrainMeshDensity + 1.0f/ this->terrainMeshDensity; //La siguiente posición en Z
				this->meshPoints[arrayIndex + 5] = asseX;
				this->meshPoints[arrayIndex + 6] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->meshPoints[arrayIndex + 7] = asseZ;
				setTextureCoordinates(arrayIndex + 8, 0, this->size);
						
				//asseX = j + ((k + 1.0f) / this->terrainMeshDensity);
				asseZ = (float)i / this->terrainMeshDensity;
				this->meshPoints[arrayIndex + 10] = asseX;
				this->meshPoints[arrayIndex + 11] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->meshPoints[arrayIndex + 12] = asseZ;
				setTextureCoordinates(arrayIndex + 13, this->size, this->size);
													  				
				//TRIÁNGULO 2	
				asseX = j + ((float)k / this->terrainMeshDensity);
				this->meshPoints[arrayIndex + 15] = asseX;
				this->meshPoints[arrayIndex + 16] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->meshPoints[arrayIndex + 17] = asseZ;
				setTextureCoordinates(arrayIndex + 18, 0, 0);
										
				//asseX = j + ((float)k / this->terrainMeshDensity);
				asseZ = (float)i / this->terrainMeshDensity + 1.0f / this->terrainMeshDensity;
				this->meshPoints[arrayIndex + 20] = asseX;
				this->meshPoints[arrayIndex + 21] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->meshPoints[arrayIndex + 22] = asseZ;
				setTextureCoordinates(arrayIndex + 23, 0, this->size);

				asseX = j + ((k + 1.0f) / this->terrainMeshDensity);
				asseZ = (float)i / this->terrainMeshDensity + 1.0f / this->terrainMeshDensity;
				this->meshPoints[arrayIndex + 25] = asseX;
				this->meshPoints[arrayIndex + 26] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->meshPoints[arrayIndex + 27] = asseZ;
				setTextureCoordinates(arrayIndex + 28, this->size, this->size);
			}
		}
	}
	//printf("Array index: %d, indices: %d\n", arrayIndex + 29, this->numberOfIndices);
	/*for (int i = 0; i < this->numberOfIndices; i += 3) {
		printf("%d-> x:%f, y:%f, z:%f\n", i / 3, this->meshPoints[i], this->meshPoints[i + 1], this->meshPoints[i + 2]);
	}*/
	//printf("y:%f\n", this->meshPoints[11 * iFactor + 11 * jFactor + 1 -5]);
	//printf("y:%f\n", this->meshPoints[6]);
}

void Terrain::setTextureCoordinates(int arrayIndex, int asseX, int asseZ) {
	this->meshPoints[arrayIndex] = (float)asseX/this->size;
	this->meshPoints[arrayIndex + 1] = (float)asseZ / this->size;
	//printf("%f %f\n", this->meshPoints[arrayIndex], this->meshPoints[arrayIndex + 1]);
}

void Terrain::createVao() {
	unsigned int VBO;

	/* INICIALIZAMOS VAO */
	glGenVertexArrays(1, &(this->VAO));
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first.
	glBindVertexArray(this->VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->numberOfIndices * sizeof(float), this->meshPoints, GL_STATIC_DRAW);/////////////////

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

//CARGA DE TEXTURAS
void Terrain::loadTexture() {
	// load and create a texture 
	// -------------------------

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
	unsigned char* data = stbi_load("C:/Users/miste/AAAJuego/Opengl-VideoGame/GameCore/Textures/floor.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	}
	else
	{
		printf("Error al cargar la textura %s\n", "C:/Users/miste/AAAJuego/Opengl-VideoGame/GameCore/Textures/floor.jpg");
		//std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Terrain::draw() {

	glUseProgram(shaderProgram);

	/* APLICAMOS TRANSFORMACIONES */
	glm::mat4 transform;
	transform = glm::mat4();

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	Camera::updateCamera(shaderProgram);
	
	/* APLICAMOS TEXTURA */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	/* DIBUJAMOS */
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->numberOfIndices);
	glBindVertexArray(0);

}


float Terrain::randomGradient(int ix, int iy) {
	// No precomputed gradients mean this works for any number of grid coordinates
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix, b = iy;
	a *= 3284157443;

	b ^= a << s | a >> w - s;
	b *= 1911520717;

	a ^= b << s | b >> w - s;
	a *= 2048419325;
	float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]


	//v.x = sin(random);
	//v.y = cos(random);
	//printf("%f\n", random);
	return random;
}

// Computes the dot product of the distance and gradient vectors.
float Terrain::dotGridGradient(int ix, int iy, float x, float y) {
	// Get gradient from integer coordinates
	float random = randomGradient(ix, iy);

	// Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Compute the dot-product
	//printf("%f\n", dx * sin(random) + dy * cos(random));
	return (dx * sin(random) + dy * cos(random));
}

float Terrain::interpolate(float a0, float a1, float w)
{
	return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}


// Sample Perlin noise at coordinates x, y
float Terrain::perlin(float x, float y) {

	// Determine grid cell corner coordinates
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	// Compute Interpolation weights
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Compute and interpolate top two corners
	float n0 = dotGridGradient(x0, y0, x, y);
	float n1 = dotGridGradient(x1, y0, x, y);
	float ix0 = interpolate(n0, n1, sx);

	// Compute and interpolate bottom two corners
	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	float ix1 = interpolate(n0, n1, sx);
	
	// Final step: interpolate between the two previously interpolated values, now in y
	float value = interpolate(ix0, ix1, sy);
	//printf("%f\n", ix0);
	return value;
}

//Obtains the height of the terrain (Y coordinate)
//with a bilinear interpolation
double Terrain::getYLocation(double x, double z) {
	double x1 = (int)x + ((1.0f / this->terrainMeshDensity) * getOffset(x));
	double z1 = (int)z + ((1.0f / this->terrainMeshDensity) * getOffset(z));
	double x2 = x1 + 1.0f;
	double z2 = z1 + 1.0f;

	int xIndex = (int)x;
	int zIndex = (int)z * this->terrainMeshDensity + getOffset(z);

	//printf("x1 = %f, y1 = %f and %f, x2 = %f, y2 = %f\n", x1, z1, z, x2, z2);

	double q11 = this->meshPoints[30 * this->terrainMeshDensity * (zIndex * this->size + xIndex) + 1];
	double q12 = this->meshPoints[30 * this->terrainMeshDensity * (zIndex * this->size + xIndex) + 6];
	double q21 = this->meshPoints[30 * this->terrainMeshDensity * (zIndex * this->size + xIndex) + 11];
	double q22 = this->meshPoints[30 * this->terrainMeshDensity * (zIndex * this->size + xIndex) + 26];

	return ((q11 * ((double)x2 - x) * ((double)z2 - z)) + (q21 * (x - (double)x1) * ((double)z2 - z)) + (q12 * ((double)x2 - x) * (z - (double)z1)) + (q22 * (x - (double)x1) * (z - (double)z1)));
}

//Obtiene el índice correspondiente para Z (las filas)
//en función del valor de este
int Terrain::getOffset(double z) {
	int counter = 0;
	//Eliminamos la parte entera
	double newZ = z - (int)z;

	while ((newZ - 1.0f / this->terrainMeshDensity) > std::numeric_limits<float>::epsilon()) {
		
		newZ = newZ - 1.0f / this->terrainMeshDensity;
		counter++;

	}

	return counter;
}