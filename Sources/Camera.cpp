#include <glad.h>
#include "../Headers/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Headers/Player.h"


int Camera::width = 1920;
int Camera::height = 1080;

//extern unsigned int shaderProgram;

void Camera::updateCamera(unsigned int shaderProgram) {
	/*
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glm::mat4 view; //Matriz de vista
	view = glm::mat4();
	view = glm::lookAt(glm::vec3(character.getPX(), character.getPY(), character.getPZ()),
		glm::vec3(character.getPX() + cos(glm::radians(character.getAng_x())) * cos(glm::radians(character.getAng_y())),
		character.getPY() + sin(glm::radians(character.getAng_y())),
		character.getPZ() + sin(glm::radians(character.getAng_x())) * cos(glm::radians(character.getAng_y()))), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


	//obj.px + 50 * cos(obj.angulo_trans_Z * GL_PI / 180), obj.py + 50 * sin(obj.angulo_trans_Z * GL_PI / 180), 0;

	glm::mat4 projection;
	projection = glm::mat4();
	//projection = glm::ortho(-40.0f * SCR_WIDTH/ SCR_HEIGHT, 40.0f * SCR_WIDTH / SCR_HEIGHT, -40.0f, 40.0f, 0.1f, 10000.0f);
	projection = glm::perspective(glm::radians(character.getPOV()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	*/

	glm::mat4 view; //Matriz de vista
	Player* player = Player::getPlayer();

	glViewport(0, 0, width, height);
	
	view = glm::mat4();
	view = glm::lookAt(
		glm::vec3(player->getPositionX(), player->getPositionY(), player->getPositionZ()),
		glm::vec3(
			player->getPositionX() + cos(glm::radians(player->getAngleRotationX())) * cos(glm::radians(player->getAngleRotationY())),
			player->getPositionY() + sin(glm::radians(player->getAngleRotationY())),
			player->getPositionZ() + sin(glm::radians(player->getAngleRotationX())) * cos(glm::radians(player->getAngleRotationY()))
		),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(player->getFOV()), (float)width / float(height), 0.1f, 1000.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

//REESCALADO
void Camera::rescaling_CallBack(GLFWwindow* window, int new_width, int new_height) {

	width = new_width;
	height = new_height;
	glViewport(0, 0, width, height);

	//updateCamera(); //TODO:Se puede eliminar?

}