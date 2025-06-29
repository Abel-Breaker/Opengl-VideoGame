#pragma once
#include "Terrain.h"

class Grass
{
private:
	float meshPoints[42];
	int numberOfInstances;
	float *randomLocations;
	int maximumSizeToSpawn;
	unsigned int VAO;
	unsigned int texture;
	unsigned int shaderProgram;
	Terrain *terrain;

	void generateMeshPoints();
	void generateRandomLocations();
	void storeRandomLocations();
	void createVao();
	void loadTexture();
	void bindBufferGrassLocation();
public:
	void prepareGrass(int numberOfInstances, Terrain* terrain);
	void draw();
};

