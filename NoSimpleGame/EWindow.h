#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//glm::mat4 matrix_transform;

#include "Batcher.h"
#include <vector>
#include "EFont.h"



#include <iostream>
#include <chrono>

class EWindowGame;
class EWindow
{
public:
	void default_update(float _d);
	virtual void update(float _d);

	void default_draw(float _d);
	virtual void draw(float _d);

	virtual void draw_interface(float _d);

	void add_time_process(std::string _name);

	static GLFWwindow* main_window;

	static unsigned int transformLoc;

	static int mouse_x;
	static int mouse_y;

	static bool LMB;
	static bool RMB;

	static bool system_button_release;

	//static std::pair<float, std::string> time_process;

	static std::vector<float> time_process_value;
	static std::vector < std::string > time_process_name;

	static std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;

	EWindow();
	~EWindow();

	static Batcher* batch;
	static EWindowGame* window_game;
	static std::vector<EWindow*> window_list;
};