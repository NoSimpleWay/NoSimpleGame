#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Batcher.h"

class EWindow
{
public:

	void default_update(float _d);
	void update(float _d);

	void default_draw(float _d);
	void draw(float _d);

	static GLFWwindow* main_window;

	static int SCR_WIDTH;
	static int SCR_HEIGHT;

	EWindow();

	static Batcher* batch;
};