#include "../Headers/Controls.h"
#include "../Headers/Player.h"

//ENTRADA DE TECLADO
void Controls::processInput(GLFWwindow* window)
{
	Player* player = Player::getPlayer();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		player->teclaA();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		player->teclaD();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		player->teclaS();
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		player->teclaW();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		player->teclaLeftShiftRun();
	}
	else {
		player->setNormalRunVelocity();
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		player->teclaSpace();
	}
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		//character.setPOV(1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		//character.setPOV(-1.0f);
	}
	
}
// Función de callback para el movimiento del ratón
void Controls::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	//mouse_xpos = static_cast<float>(xpos);
	//mouse_ypos = static_cast<float>(ypos);
	//std::cout << "Mouse Position: (" << mouse_xpos << ", " << mouse_ypos << ")\n";
	Player* player = Player::getPlayer();
	float yaw = 0, pitch = 0;

	static bool firstMouse = true;
	static float lastX = 300, lastY = 400;  // Inicialmente en el centro de la pantalla

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;  // Revertido porque y-coordinates van de abajo hacia arriba
	lastX = xpos;
	lastY = ypos;

	float velocity = 0.4f;
	xoffset *= velocity;
	yoffset *= velocity;

	yaw += xoffset;
	//character.setAng_x(xoffset);
	player->sumAngleRotationX(xoffset);
	//character.setAng_y(yoffset);
	player->sumAngleRotationY(yoffset);
	pitch += yoffset;

	// Asegúrate de que cuando el pitch esté fuera de los límites, la pantalla no se voltee
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Obtén el tamaño de la ventana
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// Envuelve el cursor si se sale de los límites de la ventana
	if (xpos <= 0) {
		glfwSetCursorPos(window, width - 1, ypos);
		lastX = width - 1;
	}
	else if (xpos >= width) {
		glfwSetCursorPos(window, 1, ypos);
		lastX = 1;
	}
	if (ypos <= 0) {
		glfwSetCursorPos(window, xpos, height - 1);
		lastY = height - 1;
	}
	else if (ypos >= height) {
		glfwSetCursorPos(window, xpos, 1);
		lastY = 1;
	}
}