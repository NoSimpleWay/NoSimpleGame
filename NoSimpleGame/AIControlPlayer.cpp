#include "AIControlPlayer.h"

void AIControlPlayer::update(Entity* _e, float _d)
{
	if ((glfwGetKey(EWindow::main_window, GLFW_KEY_W) == GLFW_PRESS)) { *_e->speed_y += 10.0 * _d; }
	if ((glfwGetKey(EWindow::main_window, GLFW_KEY_S) == GLFW_PRESS)) { *_e->speed_y -= 10.0 * _d; }

	if ((glfwGetKey(EWindow::main_window, GLFW_KEY_A) == GLFW_PRESS)) { *_e->speed_x -= 10.0 * _d; }
	if ((glfwGetKey(EWindow::main_window, GLFW_KEY_D) == GLFW_PRESS)) { *_e->speed_x += 10.0 * _d; }
}

AIControlPlayer::AIControlPlayer()
{
}

AIControlPlayer::~AIControlPlayer()
{
}
