#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//glm::mat4 matrix_transform;

#include "Batcher.h"
#include <vector>



#include <iostream>

class EWindowGame;
class EWindow
{
public:
	void default_update(float _d);
	virtual void update(float _d);

	void default_draw(float _d);
	virtual void draw(float _d);

	static GLFWwindow* main_window;

	static unsigned int transformLoc;

	static int mouse_x;
	static int mouse_y;

	static bool LMB;
	static bool RMB;

	EWindow();
	~EWindow();

	static Batcher* batch;
	static EWindowGame* window_game;
	static std::vector<EWindow*> window_list;
};