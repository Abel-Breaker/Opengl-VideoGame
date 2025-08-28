#pragma once
#include "Terrain.h"

class Grass
{
private:
	float maxLODmeshPoints[66];
	int numberOfVertexs;
	float *randomLocations;
	int maximumSizeToSpawn;
	unsigned int VAO;
	unsigned int texture;
	unsigned int shaderProgram;
	Terrain *terrain;

	void generateMaxLODMeshPoints();
	void generateMidLODMeshPoints();
	void generateLowLODMeshPoints();
	void generateRandomLocations();
	void storeRandomLocations();
	void createVao();
	void loadTexture();
	void bindBufferGrassLocation();
public:
	void prepareGrass(int numberOfVertexs, Terrain* terrain);
	void draw();
};

