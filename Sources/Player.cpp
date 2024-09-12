#include "../Headers/Player.h"
#include <cmath>
#include <corecrt_math_defines.h>
#include <limits>
#include "../Headers/Frame.h"
#define HEIGHTJUMP 7
#define SPEEDJUMP 20
#define DEFAULTGRAVITY 40 //NO MUY ALTA
#define DEFAULTSPEED 10
#define SPRINTINGTSPEED 2*DEFAULTSPEED

Player* Player::player = nullptr;


Player* Player::getPlayer() {
	if (player == nullptr) {
		player = new Player();
	}
	return player;
}
Player::Player() {
	this->position_X = this->position_Z = 0;
	this->position_Y = 3;
	this->angleRotation_X = this->angleRotation_Y = this->angleRotation_Z = 0;
	this->FOV = 120.0f;
	this->velocity = DEFAULTSPEED;
	this->forceGravity = DEFAULTGRAVITY;
	this->jumping = 0;
	this->terrain = NULL;
}

void Player::applyGravity() { //TODO: ajustar el salto manualmente en vez de reducir la gravedad( this->position_Y +=1;); o hacer que a mas cerca del suelo mas gravedad
	static double oldHeight = 0.0; // Almacena la posición solo la primera vez que jumping es 1
	static bool firstTime = true;  // Variable que controla si ya se capturó oldHeight
	double lastLapsoTime = 0;

	if (this->jumping == 1) {
		if (firstTime) {
			oldHeight = this->position_Y; // Captura la posición la primera vez que jumping es 1
			firstTime = false;
		}

		lastLapsoTime = Frame::getLapsoTime();
		this->position_Y += SPEEDJUMP * lastLapsoTime;

		if ((this->position_Y - oldHeight > HEIGHTJUMP)) {
			this->jumping = 2;
		}
	}
	else if (this->jumping == 2) {
		if ((abs(this->position_Y - 3.0 - this->terrain->getYLocation(this->position_X, this->position_Z)) > 0.4)) {
			lastLapsoTime = Frame::getLapsoTime();
			this->position_Y -= SPEEDJUMP * lastLapsoTime;
		}
		else {
			this->forceGravity = DEFAULTGRAVITY;
			this->position_Y = this->terrain->getYLocation(this->position_X, this->position_Z) + 3.0;
			this->jumping = 0;
			firstTime = true; // Resetea firstTime cuando el salto termina
		}
	}
	else if (this->jumping == 0) {
		updateHeight(this->terrain->getYLocation(this->position_X, this->position_Z));
	}
}

void Player::updateHeight(double newHeightToReach) {
	this->position_Y -= 3;
	this->position_Y = (this->position_Y + this->forceGravity * Frame::getLapsoTime() * (newHeightToReach - this->position_Y)) + 3.0;
}


void Player::teclaA() {
	this->position_Z -= this->velocity * Frame::getLapsoTime() * sin(M_PI / 180 * this->angleRotation_X + M_PI / 180 * 90);
	this->position_X -= this->velocity * Frame::getLapsoTime() * cos(M_PI / 180 * this->angleRotation_X + M_PI / 180 * 90);
}

void Player::teclaD() {
	this->position_Z += this->velocity * Frame::getLapsoTime() * sin(M_PI / 180 * this->angleRotation_X + M_PI / 180 * 90);
	this->position_X += this->velocity * Frame::getLapsoTime() * cos(M_PI / 180 * this->angleRotation_X + M_PI / 180 * 90);
}

void Player::teclaS() {
	this->position_Z -= this->velocity * Frame::getLapsoTime() * sin(M_PI / 180 * this->angleRotation_X);
	this->position_X -= this->velocity * Frame::getLapsoTime() * cos(M_PI / 180 * this->angleRotation_X);
}

void Player::teclaW() {
	this->position_Z += this->velocity * Frame::getLapsoTime() * sin(M_PI / 180 * this->angleRotation_X);
	this->position_X += this->velocity * Frame::getLapsoTime() * cos(M_PI / 180 * this->angleRotation_X);
}

void Player::teclaSpace() {
	if (this->jumping == 0) {
		this->forceGravity = 7; //Adjust force of the gravity while jumping
		this->velocity = 0.05; //do nothing
		this->jumping = 1;
	}
}

void Player::sumAngleRotationX(float x) {
	this->angleRotation_X += x;
}
void Player::sumAngleRotationY(float y) {
	this->angleRotation_Y += y;
	if (this->angleRotation_Y > 89.9f)
		this->angleRotation_Y = 89.9f;
	if (this->angleRotation_Y < -89.9f)
		this->angleRotation_Y = -89.9f;
}
void Player::setFOV(float FOV) {
	this->FOV = FOV;
}
void Player::setTerrain(Terrain* terrain) {
	this->terrain = terrain;
}

void Player::teclaLeftShiftRun() {
	this->velocity = 20;
	this->FOV = 125;
}
void Player::setNormalRunVelocity() {
	this->velocity = DEFAULTSPEED;
	this->FOV = 120;
}

float Player::getPositionX() {
	return this->position_X;
}
double Player::getPositionY() {
	return this->position_Y;
}
float Player::getPositionZ() {
	return this->position_Z;
}
float Player::getAngleRotationX() {
	return this->angleRotation_X;
}
float Player::getAngleRotationY() {
	return this->angleRotation_Y;
}
float Player::getFOV() {
	return this->FOV;
}

/*
void Player::setPosition(float x, float y, float z) {
	this->position_X = x;
	this->position_Y = y;
	this->position_Z = z;
}*/