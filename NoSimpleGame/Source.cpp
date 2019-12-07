#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION

#include <ctime>

#include "EWindow.h"
#include "EGraphicCore.h"
//#include "ETextureAtlas.h"


#include "Enums.h"
#include "EWindowGame.h"


















float delta_time;
float saved_time_for_delta;



void recalculate_correction();
void processInput(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void recalculate_correction();
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double _x, double _y);
void char_input_callback(GLFWwindow* window, unsigned int _char);
//void load_texture(char const* _path, int _id);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	srand(time(NULL));

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

// glfw window creation
// --------------------
	EWindow::main_window = glfwCreateWindow(EGraphicCore::SCR_WIDTH, EGraphicCore::SCR_HEIGHT, "Window name", NULL, NULL);

	if (EWindow::main_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(EWindow::main_window);

	glfwSetFramebufferSizeCallback(EWindow::main_window, framebuffer_size_callback);
	glfwSetScrollCallback(EWindow::main_window, scroll_callback);
	glfwSetMouseButtonCallback(EWindow::main_window, mouse_button_callback);
	glfwSetCursorPosCallback(EWindow::main_window, mouse_position_callback);
	glfwSetCharCallback(EWindow::main_window, char_input_callback);
	//glfwSetMousePos(0, 0);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	EGraphicCore::ourShader = new Shader("data/5.1.transform.vs", "data/5.1.transform.fs");

	EGraphicCore::ourShader->use();
	glfwSwapInterval(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, EGraphicCore::SCR_WIDTH, EGraphicCore::SCR_HEIGHT);
	recalculate_correction();

	//
	//batcher initiation
	EGraphicCore::batch = new Batcher();

	for (int i = 0; i < 5000; i++)
	{
		EGraphicCore::batch->fill_indices();
	}

	EGraphicCore::batch->init();
	//
	//

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glBlendEquation(GL_FUNC_ADD);
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------

	//cout << (int)01.35f << endl;
	glViewport(0, 0, EGraphicCore::SCR_WIDTH, EGraphicCore::SCR_HEIGHT);



	EGraphicCore::load_texture("data/white_pixel.png", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, EGraphicCore::texture[0]);
	EGraphicCore::ourShader->setInt("texture1", EGraphicCore::texture[0]);

	EWindowGame* wg = new EWindowGame();
	EWindow::window_game = wg;
	EWindow::window_list.push_back (wg);
	wg->init();
	

	while (!glfwWindowShouldClose(EWindow::main_window))
	{
		clock_t time = clock();
		delta_time = (time - saved_time_for_delta) / 1000.0;
		saved_time_for_delta = time;

		delta_time /= 1.0f;//
		for (EWindow* w : EWindow::window_list)
		{
			w->default_update(delta_time);
			w->update(delta_time);
		}

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.4f, 0.5f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		EGraphicCore::batch->reset();

		
		
		for (EWindow* w : EWindow::window_list)
		{
			w->default_draw(delta_time);
			w->draw(delta_time);
		}
		

		EGraphicCore::batch->reinit();
		EGraphicCore::batch->draw_call();

		glfwSwapBuffers(EWindow::main_window);
		glfwPollEvents();

		processInput(EWindow::main_window);
	}

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{//
	glViewport(0, 0, width, height);
	glfwGetWindowSize(window, &EGraphicCore::SCR_WIDTH, &EGraphicCore::SCR_HEIGHT);

	std::cout << "Resize event width:" << width << " height: " << height << std::endl;

	recalculate_correction();
}

void recalculate_correction()
{
	if ((EGraphicCore::SCR_WIDTH > 100) && (EGraphicCore::SCR_HEIGHT > 100))
	{

		EGraphicCore::correction_x = 1.0 / EGraphicCore::SCR_WIDTH * 2.0;
		EGraphicCore::correction_y = 1.0 / EGraphicCore::SCR_HEIGHT * 2.0;

		//std::cout << "helper correction_x: " << correction_x << " correction_y: " << correction_y << std::endl;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//if () 00:13:53 27.11.2019
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		EWindow::LMB = true;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		EWindow::LMB = false;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		EWindow::RMB = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		EWindow::RMB = false;
	}
}

void mouse_position_callback(GLFWwindow* window, double _x, double _y)
{
	EWindow::mouse_x = _x;
	EWindow::mouse_y = EGraphicCore::SCR_HEIGHT - _y;
}

void char_input_callback(GLFWwindow* window, unsigned int _char)
{

}

void processInput(GLFWwindow* window)
{
	
	

	/*
	if ((glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS))
	{
		for (Entity* e : EWindow::window_game->entity_list)
		{
			*e->position_x = (rand() % 1000) + 55.0f;
			*e->position_y = (rand() % 1000) + 55.0f;
		}
	}*/

	/*
	if ((glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS))
	{
		for (int i = 0; i < PATH_MATRIX_ARRAY_SIZE; i++)
		for (int j = 0; j < PATH_MATRIX_ARRAY_SIZE; j++)
			{

				if (rand() % 5 == 0)
				{
					EWindow::window_game->unwalk_matrix[j][i][0] = true;
					EWindow::window_game->unwalk_matrix[j][i][1] = true;
				}
				else
				{
					EWindow::window_game->unwalk_matrix[j][i][0] = false;
					EWindow::window_game->unwalk_matrix[j][i][1] = false;
				}

				EWindow::window_game->path_matrix[j][i][0] = 99;
				EWindow::window_game->path_matrix[j][i][1] = 99;

				EWindow::window_game->blocked_by_entity[j][i][0] = false;
				EWindow::window_game->blocked_by_entity[j][i][1] = false;
			}
	}*/
}



