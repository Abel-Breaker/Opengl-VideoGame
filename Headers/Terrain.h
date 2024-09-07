#pragma once

class Terrain
{
private:
	int size; //TODO: short int?
	int terrainMeshDensity; //Subdivision of each square
	short int frequency;
	short int amplitude;
	int numberOfIndices;
	float *meshPoints;
	unsigned int VAO;
	unsigned int texture;
	unsigned int shaderProgram;

	void initializeAtributes(int size, int terrainMeshDensity, short frequency, short amplitude);
	void generateMeshPoints();
	void setTextureCoordinates(int arrayIndex, int asseX, int asseZ);
	void createVao();
	void loadTexture();
	float randomGradient(int ix, int iy);
	float dotGridGradient(int ix, int iy, float x, float y);
	float interpolate(float a0, float a1, float w);
	float perlin(float x, float y);
	int getOffset(double z);

public:
	//Terrain();
	void prepareTerrain(int size, int terrainMeshDensity, short frequency, short amplitude);
	int getSize();
	double getYLocation(double x, double z);
	void draw();
};

