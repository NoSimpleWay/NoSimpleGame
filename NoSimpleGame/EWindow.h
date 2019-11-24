#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Batcher.h"
#include <vector>

class EWindowGame;
class EWindow
{
public:
	void default_update(float _d);
	virtual void update(float _d);

	void default_draw(float _d);
	virtual void draw(float _d);

	static GLFWwindow* main_window;

	static int SCR_WIDTH;
	static int SCR_HEIGHT;

	EWindow();
	~EWindow();

	static Batcher* batch;
	static EWindowGame* window_game;
	static std::vector<EWindow*> window_list;
};