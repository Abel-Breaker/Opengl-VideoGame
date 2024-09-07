#pragma once
#include "Terrain.h"

class Grass
{
private:
	float meshPoints[30];
	int numberOfIndices;
	float *randomLocations;
	//float *terrainMesh;
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

