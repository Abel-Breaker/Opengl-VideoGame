#pragma once
#include "Terrain.h"

class Player
{
private:
	double position_X, position_Y, position_Z;
	float angleRotation_X, angleRotation_Y, angleRotation_Z; //TODO: Quitar ang Z?
	float FOV;
	float velocity;
	float forceGravity; //less = better smooth and less precision
	int jumping;
	Terrain *terrain;
	static Player* player;
	
	Player();
	void updateHeight(double newHeightToReach);

public:

	static Player* getPlayer();

	void applyGravity();

	void teclaA();
	void teclaD();
	void teclaS();
	void teclaW();
	void teclaSpace();
	void teclaLeftShiftRun();
	void setNormalRunVelocity();

	void sumAngleRotationX(float x);
	void sumAngleRotationY(float y);
	void setFOV(float FOV);
	void setTerrain(Terrain *terrain);

	float getPositionX();
	double getPositionY();
	float getPositionZ();
	float getAngleRotationX();
	float getAngleRotationY();
	float getFOV();
	//void setPosition(float x, float y, float z);
};

