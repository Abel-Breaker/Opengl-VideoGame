#include "../Headers/Manager.h"
#include "../Headers/Frame.h"


void Manager::loader() {
	terrain.prepareTerrain(150, 1, 10, 8); //150 4 10 8 //freq!=size
	grass.prepareGrass(50000, &terrain);
	
	player = Player::getPlayer();
	player->setTerrain(&terrain);

	const char* path = "GameCore/Models/Zombie.dae";
	ourModel = new Model(path);
	danceAnimation = new Animation(path, ourModel);
	animator = new Animator(danceAnimation);
}

void Manager::run() {
	draw();
	player->applyGravity();
	//Camera::updateCamera();

}

void Manager::draw() {
	terrain.draw();
	
	//player->draw();


	ourModel->Draw(animator);
	grass.draw();
}