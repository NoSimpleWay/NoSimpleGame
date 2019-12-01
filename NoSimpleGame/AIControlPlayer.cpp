#include "AIControlPlayer.h"


void AIControlPlayer::acceleration_control(Entity* _e, float _d)
{
	if ((glfwGetKey(EWindow::main_window, GLFW_KEY_W) == GLFW_PRESS)) { *_e->speed_y += *_e->acceleration * _d; }
	if ((glfwGetKey(EWindow::main_window, GLFW_KEY_S) == GLFW_PRESS)) { *_e->speed_y -= *_e->acceleration * _d; }

	if ((glfwGetKey(EWindow::main_window, GLFW_KEY_A) == GLFW_PRESS)) { *_e->speed_x -= *_e->acceleration * _d; }
	if ((glfwGetKey(EWindow::main_window, GLFW_KEY_D) == GLFW_PRESS)) { *_e->speed_x += *_e->acceleration * _d; }
}

AIControlPlayer::AIControlPlayer()
{
}

AIControlPlayer::~AIControlPlayer()
{
}
