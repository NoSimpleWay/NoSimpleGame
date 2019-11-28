#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
	
#include "EWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/filesystem.h>

#include <iostream>

#include <stb_image.h>
#include "ETexture.h"

#include "Enums.h"

#include <ctime>
#include <vector>

#include "EWindowGame.h"
















unsigned char* data1;
int width, height, nrChannels;

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
void load_texture(char const* _path, int _id);

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
	EWindow::main_window = glfwCreateWindow(EWindow::SCR_WIDTH, EWindow::SCR_HEIGHT, "Window name", NULL, NULL);

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	EWindow::ourShader = new Shader("data/5.1.transform.vs", "data/5.1.transform.fs");

	EWindow::ourShader->use();
	glfwSwapInterval(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, EWindow::SCR_WIDTH, EWindow::SCR_HEIGHT);
	recalculate_correction();

	//
	//batcher initiation
	EWindow::batch = new Batcher();

	for (int i = 0; i < 15000; i++)
	{
		EWindow::batch->fill_indices();
	}

	EWindow::batch->init();
	//
	//

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glBlendEquation(GL_FUNC_ADD);
	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------

	//cout << (int)01.35f << endl;
	glViewport(0, 0, EWindow::SCR_WIDTH, EWindow::SCR_HEIGHT);



	load_texture("data/white_pixel.png", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ETexture::texture[0]);
	EWindow::ourShader->setInt("texture1", ETexture::texture[0]);

	EWindowGame* wg = new EWindowGame();
	EWindow::window_game = wg;
	EWindow::window_list.push_back (wg);
	

	while (!glfwWindowShouldClose(EWindow::main_window))
	{
		clock_t time = clock();
		delta_time = (time - saved_time_for_delta) / 1000.0;
		saved_time_for_delta = time;

		delta_time /= 1.0f;
		for (EWindow* w : EWindow::window_list)
		{
			w->default_update(delta_time);
			w->update(delta_time);
		}

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.4f, 0.5f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		EWindow::batch->reset();

		
		
		for (EWindow* w : EWindow::window_list)
		{
			w->default_draw(delta_time);
			w->draw(delta_time);
		}
		

		EWindow::batch->reinit();
		EWindow::batch->draw_call();

		glfwSwapBuffers(EWindow::main_window);
		glfwPollEvents();

		processInput(EWindow::main_window);
	}

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{//
	glViewport(0, 0, width, height);
	glfwGetWindowSize(window, &EWindow::SCR_WIDTH, &EWindow::SCR_HEIGHT);

	std::cout << "Resize event width:" << width << " height: " << height << std::endl;

	recalculate_correction();
}

void recalculate_correction()
{
	if ((EWindow::SCR_WIDTH > 100) && (EWindow::SCR_HEIGHT > 100))
	{

		EWindow::correction_x = 1.0 / EWindow::SCR_WIDTH * 2.0;
		EWindow::correction_y = 1.0 / EWindow::SCR_HEIGHT * 2.0;

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
	EWindow::mouse_y = EWindow::SCR_HEIGHT - _y;
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

void load_texture(char const* _path, int _id)
{
	glGenTextures(_id, &ETexture::texture[_id]);
	glBindTexture(GL_TEXTURE_2D, ETexture::texture[_id]);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	data1 = stbi_load(_path, &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);

		//cout << "loaded texture W:" << width << " H:" << height << endl;

		//last_texture_h = height;
		//last_texture_w = width;

		//texture_error = false;
		/*texture_w[_id] = width;
		texture_h[_id] = height;*/
	}
	else
	{
		//cout << red << "Failed to load texture " << yellow << "(" << _path << ")" << green << endl;

		//last_texture_h = 21;
		//last_texture_w = 21;

		//texture_error = true;

		//just_created_gabarite = DefaultGabarite::gabarite_error;
	}

	stbi_image_free(data1);
}


