#pragma once
#include <vector>
/*
struct Vertex {
	float x, y, z;    // posición
	//float nx, ny, nz; // normales
	float u, v;       // coordenadas UV
};

struct TerrainChunk {
	std::vector<Vertex> vertices;  // vértices del chunk
	int lodLevel;                  // nivel de detalle actual

	// Métodos para cargar, actualizar LOD, dibujar, etc
	//void updateLOD(const Camera& cam);
	//void draw();
};*/

class Terrain
{
private:

	//std::vector<TerrainChunk> chunks;
	int size; //TODO: short int?
	int terrainMeshDensity; //Subdivision of each square
	short int frequency;
	short int amplitude;
	int numberOfVertexs;
	float *vertex;
	int *indexForVertexs;
	unsigned int VAO;
	unsigned int texture;
	unsigned int shaderProgram;

	void initializeAtributes(int size, int terrainMeshDensity, short frequency, short amplitude);
	void generateMaxLODMeshPoints();
	void setTextureCoordinates(int arrayIndex, int asseX, int asseZ);
	void createVao();
	void loadTexture();
	float randomGradient(int ix, int iy);
	float dotGridGradient(int ix, int iy, float x, float y);
	float interpolate(float a0, float a1, float w);
	float perlin(float x, float y);

	void getVertices(int width, int height);

public:
	//Terrain();
	void prepareTerrain(int size, int terrainMeshDensity, short frequency, short amplitude);
	int getSize();
	double getYLocation(double x, double z);
	void draw();
};

