#pragma once

#include "Terrain.h"
#include "Player.h"
#include "Camera.h"
#include "Controls.h" //Se puede poner en otro sitio
#include "Grass.h"

class Manager
{
private:
	Terrain terrain;
	Grass grass;
	Player* player;

	void draw();

public:
	void loader(); //No haria falta realmente (?)

	void run();
};

