#include "../Headers/Manager.h"

void Manager::loader() {
	terrain.prepareTerrain(150, 1, 10, 8); //150 4 10 8 //freq!=size
	grass.prepareGrass(50000, &terrain);
	
	player = Player::getPlayer();
	player->setTerrain(&terrain);
}

void Manager::run() {
	draw();
	player->applyGravity();
	//Camera::updateCamera();

}

void Manager::draw() {
	terrain.draw();
	grass.draw();
	
	//player->draw();
}