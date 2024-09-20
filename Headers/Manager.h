#pragma once

#include "Terrain.h"
#include "Player.h"
#include "Camera.h"
#include "Controls.h" //Se puede poner en otro sitio
#include "Grass.h"
#include "../Headers/Animator.h"
#include "../Headers/Animation.h"
#include "../Headers/Model.h"



class Manager
{
private:
	Terrain terrain;
	Grass grass;
	Player* player;

	Model *ourModel;
	Animation *danceAnimation;
	Animator *animator;
	//Model wolf;

	void draw();

public:
	void loader(); //No haria falta realmente (?)

	void run();
};

