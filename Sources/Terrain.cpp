#include "../Headers/Terrain.h"
#include <glad.h>
#include <corecrt_malloc.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include "../Headers/Camera.h"
#include "../Headers/ShaderManager.h"

#define CHUNK_SIZE 20




void Terrain::prepareTerrain(int size, int terrainMeshDensity, short frequency, short amplitude) {
	shaderProgram = ShaderManager::setAndGetShader("GameCore/Shaders/terrainShader.vert", "GameCore/Shaders/terrainShader.frag");
	initializeAtributes(size, terrainMeshDensity, frequency, amplitude);
	getVertices(size, size);
	generateMaxLODMeshPoints();
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
	//this->numberOfVertexs = ((size) * (size)) * (6 * terrainMeshDensity * terrainMeshDensity) * 3; // Cada 3 índices es una coordenada/vértice
	//this->numberOfVertexs += this->numberOfVertexs * 2 / 3; //Texture coordinates
	this->numberOfVertexs = size * size * 3;
	this->vertex = (float*)malloc((this->numberOfVertexs) * sizeof(float));
	int numberOfIndices = (size * size) + (size - 1) * (size - 2);
	this->indexForVertexs = (int*)malloc(numberOfIndices * sizeof(int));
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

void Terrain::generateMaxLODMeshPoints() {
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
				this->indexForVertexs[arrayIndex] = asseX;
				this->indexForVertexs[arrayIndex + 1] = perlin(asseX*relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->indexForVertexs[arrayIndex + 2] = asseZ;
				setTextureCoordinates(arrayIndex + 3, 0, 0);

					
				asseX = j + ((k + 1.0f) / this->terrainMeshDensity);
				asseZ = (float)i / this->terrainMeshDensity + 1.0f/ this->terrainMeshDensity; //La siguiente posición en Z
				this->indexForVertexs[arrayIndex + 5] = asseX;
				this->indexForVertexs[arrayIndex + 6] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->indexForVertexs[arrayIndex + 7] = asseZ;
				setTextureCoordinates(arrayIndex + 8, 0, this->size);
						
				//asseX = j + ((k + 1.0f) / this->terrainMeshDensity);
				asseZ = (float)i / this->terrainMeshDensity;
				this->indexForVertexs[arrayIndex + 10] = asseX;
				this->indexForVertexs[arrayIndex + 11] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->indexForVertexs[arrayIndex + 12] = asseZ;
				setTextureCoordinates(arrayIndex + 13, this->size, this->size);
													  				
				//TRIÁNGULO 2	
				asseX = j + ((float)k / this->terrainMeshDensity);
				this->indexForVertexs[arrayIndex + 15] = asseX;
				this->indexForVertexs[arrayIndex + 16] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->indexForVertexs[arrayIndex + 17] = asseZ;
				setTextureCoordinates(arrayIndex + 18, 0, 0);
										
				//asseX = j + ((float)k / this->terrainMeshDensity);
				asseZ = (float)i / this->terrainMeshDensity + 1.0f / this->terrainMeshDensity;
				this->indexForVertexs[arrayIndex + 20] = asseX;
				this->indexForVertexs[arrayIndex + 21] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->indexForVertexs[arrayIndex + 22] = asseZ;
				setTextureCoordinates(arrayIndex + 23, 0, this->size);
				
				asseX = j + ((k + 1.0f) / this->terrainMeshDensity);
				asseZ = (float)i / this->terrainMeshDensity + 1.0f / this->terrainMeshDensity;
				this->indexForVertexs[arrayIndex + 25] = asseX;
				this->indexForVertexs[arrayIndex + 26] = perlin((asseX) * relativeFrequency, asseZ * relativeFrequency) * this->amplitude;
				this->indexForVertexs[arrayIndex + 27] = asseZ;
				setTextureCoordinates(arrayIndex + 28, this->size, this->size);
			}
		}
	}
}*/


void Terrain::getVertices(int width, int height) {

	float relativeFrequency = (float)this->frequency / this->size;
	int i = 0;

	for (int row = 0; row < this->size; row++) {
		for (int col = 0; col < this->size; col++) {
			this->vertex[i++] = (float)col;
			this->vertex[i++] = perlin((float)col * relativeFrequency, (float)row * relativeFrequency) * this->amplitude;
			this->vertex[i++] = (float)row;
		}
	}
}

void Terrain::generateMaxLODMeshPoints() {

	int i = 0;

	for (int row = 0; row < this->size - 1; row++) {
		if ((row & 1) == 0) {  // filas pares
			for (int col = 0; col < this->size; col++) {
				this->indexForVertexs[i++] = col + row * this->size;
				this->indexForVertexs[i++] = col + (row + 1) * this->size;
			}
		}
		else {  // filas impares
			for (int col = this->size - 1; col > 0; col--) {
				this->indexForVertexs[i++] = col + (row + 1) * this->size;
				this->indexForVertexs[i++] = col - 1 + row * this->size;
			}
		}
	}

	// Caso especial para mHeight impar y > 2 (no está muy claro, parece un error)


}


//Texture coordinates for EACH square
void Terrain::setTextureCoordinates(int arrayIndex, int asseX, int asseZ) {
	this->indexForVertexs[arrayIndex] = (float)asseX/this->size;
	this->indexForVertexs[arrayIndex + 1] = (float)asseZ / this->size;
}

void Terrain::createVao() {
	unsigned int VBO, EBO;

	/* INICIALIZAMOS VAO */
	glGenVertexArrays(1, &(this->VAO));
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(this->VAO);

	// Bind vertexs
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numberOfVertexs * sizeof(float), vertex, GL_STATIC_DRAW);

	// Bind index vertexs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ((size * size) + (size - 1) * (size - 2)) * sizeof(int), indexForVertexs, GL_STATIC_DRAW);


	//Normales
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Textura
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	/* ELIMINAMOS VAOs*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glDeleteBuffers(1, &VBO);
}

//CARGA DE TEXTURAS
void Terrain::loadTexture() 
{

	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
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
	glEnable(GL_CULL_FACE);

	glUseProgram(shaderProgram);

	Camera::updateCamera(shaderProgram);
	
	/* APLICAMOS TEXTURA */
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, this->texture);

	/* DIBUJAMOS */
	glBindVertexArray(this->VAO);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, (this->size * this->size) + (this->size - 1) * (this->size - 2));
	glDrawElements(GL_TRIANGLE_STRIP, (size * size) + (size - 1) * (size - 2), GL_UNSIGNED_INT, 0);
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
/*
double Terrain::getYLocation(double x, double z) {
	double x1 = (int)x +((1.0f / this->terrainMeshDensity) * getOffset(x));
	double z1 = (int)z +((1.0f / this->terrainMeshDensity) * getOffset(z));
	double x2 = x1 + 1.0f;
	double z2 = z1 + 1.0f;

	int xIndex = (int)x;
	int zIndex = (int)z * this->terrainMeshDensity + getOffset(z);

	//printf("x1 = %f, y1 = %f and %f, x2 = %f, y2 = %f\n", x1, z1, z, x2, z2);

	double q11 = this->indexForVertexs[30 * this->terrainMeshDensity * (zIndex * this->size + xIndex) + 1];
	double q12 = this->indexForVertexs[30 * this->terrainMeshDensity * (zIndex * this->size + xIndex) + 6];
	double q21 = this->indexForVertexs[30 * this->terrainMeshDensity * (zIndex * this->size + xIndex) + 11];
	double q22 = this->indexForVertexs[30 * this->terrainMeshDensity * (zIndex * this->size + xIndex) + 26];

	return ((q11 * ((double)x2 - x) * ((double)z2 - z)) + (q21 * (x - (double)x1) * ((double)z2 - z)) + (q12 * ((double)x2 - x) * (z - (double)z1)) + (q22 * (x - (double)x1) * (z - (double)z1)));
}*/

//Obtains the height of the terrain (Y coordinate)
//with a bilinear interpolation
double Terrain::getYLocation(double x, double z) {

	// Obtain the 4 points of the grid of the square where you are
	double x1 = (int)x;
	double z1 = (int)z;
	double x2 = x1 + 1.0f;
	double z2 = z1 + 1.0f;

	int xIndex = (int)x;
	int zIndex = (int)z;

	//printf("x1 = %f, y1 = %f and %f, x2 = %f, y2 = %f\n", x1, z1, z, x2, z2);

	// Get the height of each point of the square (Y coordinate)
	double q11 = this->vertex[(zIndex * this->size + xIndex) * 3 + 1];
	double q12 = this->vertex[(zIndex * this->size + xIndex+1) * 3 + 1];
	double q21 = this->vertex[((zIndex+1)*this->size + xIndex) * 3 + 1];
	double q22 = this->vertex[((zIndex + 1) * this->size + xIndex + 1) * 3 + 1];

	// Interpolación bilineal
	double fx = x - x1;
	double fz = z - z1;

	return (
		q11 * (1 - fx) * (1 - fz) +
		q21 * fx * (1 - fz) +
		q12 * (1 - fx) * fz +
		q22 * fx * fz
		);
}