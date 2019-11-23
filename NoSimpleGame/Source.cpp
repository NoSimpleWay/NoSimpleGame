#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
	
#include "EWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_s.h>

#include <iostream>


#include <stb_image.h>
#include "ETexture.h"

#include "Enums.h"

#include <ctime>
#include "Entity.h"
#include <vector>



float correction_x = 0.1f;
float correction_y = 0.1f;

short path_matrix [50][50];
bool unwalk_matrix[50][50];
bool blocked_by_entity[50][50];

int heatmap_phase = Enums::HEATMAP_PHASE::UP;

float character_position_x = 500.0f;
float character_position_y = 500.0f;

int path_position_x = 0;
int path_position_y = 0;


Shader* ourShader;

glm::mat4 matrix_transform;

unsigned int transformLoc;

unsigned char* data1;
int width, height, nrChannels;

float delta_time;
float saved_time_for_delta;

std::vector<Entity*> entity_list;

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

	ourShader = new Shader("data/5.1.transform.vs", "data/5.1.transform.fs");

	ourShader->use();
	glfwSwapInterval(0);

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
	ourShader->setInt("texture1", ETexture::texture[0]);

	for (int i = 0; i < 50; i++)
	for (int j = 0; j < 50; j++)
	{
	
		if (rand() % 5 == 0)
		{
			unwalk_matrix[j][i] = true;
		}
		else
		{
			unwalk_matrix[j][i] = false;
		}

		path_matrix[j][i] = 99;
		blocked_by_entity[j][i] = false;
	}

	entity_list.push_back(new Entity(200.0f, 150.0f));
	entity_list.push_back(new Entity(300.0f, 150.0f));
	entity_list.push_back(new Entity(400.0f, 150.0f));
	

	while (true)
	{
		clock_t time = clock();
		delta_time = (time - saved_time_for_delta) / 1000.0;
		saved_time_for_delta = time;

		ourShader->use();

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.4f, 0.5f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		matrix_transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

		matrix_transform = glm::translate(matrix_transform, glm::vec3(-1, -1, 0.0f));
		matrix_transform = glm::scale(matrix_transform, glm::vec3(correction_x, correction_y, 1));

		transformLoc = glGetUniformLocation(ourShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix_transform));


		EWindow::batch->reset();

		if (heatmap_phase == Enums::HEATMAP_PHASE::UP)
		{
			path_position_x = (int)(character_position_x / 50.0f);
			path_position_y = (int)(character_position_y / 50.0f);

			path_matrix[path_position_x][path_position_y] = 5;
		}

		for (Entity* e : entity_list)
		{
			blocked_by_entity[(int)(*e->position_x / 50.0f)][(int)(*e->position_y / 50.0f)] = true;
		}

		if (heatmap_phase == Enums::HEATMAP_PHASE::UP)
		for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
		if ((!unwalk_matrix[j][i]) && (!blocked_by_entity[j][i]))
		{
			if (i < 19)
			{
				if (path_matrix[j][i + 1] > path_matrix[j][i]) { path_matrix[j][i + 1] = path_matrix[j][i] + 1; }
			}
		}

		if (heatmap_phase == Enums::HEATMAP_PHASE::RIGHT)
		for (int j = 0; j < 20; j++)
		for (int i = 0; i < 20; i++)
		if ((!unwalk_matrix[j][i]) && (!blocked_by_entity[j][i]))
		{
			if (j < 19)
			{
				if (path_matrix[j + 1][i] > path_matrix[j][i]) { path_matrix[j + 1][i] = path_matrix[j][i] + 1; }
			}
		}

		if (heatmap_phase == Enums::HEATMAP_PHASE::DOWN)
		for (int i = 19; i > 0; i--)
		for (int j = 0; j < 20; j++)
		if ((!unwalk_matrix[j][i]) && (!blocked_by_entity[j][i]))
		{
			if (i > 0)
			{
				if (path_matrix[j][i - 1] > path_matrix[j][i]) { path_matrix[j][i - 1] = path_matrix[j][i] + 1; }
			}
		}

		if (heatmap_phase == Enums::HEATMAP_PHASE::LEFT)
		for (int j = 19; j > 0; j--)
		for (int i = 0; i < 20; i++)
		if ((!unwalk_matrix[j][i]) && (!blocked_by_entity[j][i]))
		{
			if (j > 0)
			{
				if (path_matrix[j - 1][i] > path_matrix[j][i]) { path_matrix[j - 1][i] = path_matrix[j][i] + 1; }
			}
		}

		if (heatmap_phase == Enums::HEATMAP_PHASE::HEATING)
		for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
		{
			path_matrix[j][i] += 1;
			if (path_matrix[j][i] > 100) { path_matrix[j][i] = 100; }
		}

		for (int i=0; i<20; i++)
		for (int j=0; j<20; j++)
		{
			if (unwalk_matrix[j][i])
			{
				EWindow::batch->setcolor(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				//float col = 1.0f - path_matrix[j][i] / 10.0f;
				//EWindow::batch->setcolor(col, col, col, 1.0f);

				int color_correction = 0;
				if ((heatmap_phase == Enums::HEATMAP_PHASE::HEATING)) { color_correction = -1; }

				if (path_matrix[j][i] + color_correction > 15) { EWindow::batch->setcolor(0.0f, 0.0f, 0.0f, 0.75f); }
				else
				if (path_matrix[j][i] + color_correction > 13) { EWindow::batch->setcolor(1.0f, 0.0f, 0.0f, 0.5f); }
				else
				if (path_matrix[j][i] + color_correction > 10) { EWindow::batch->setcolor(1.0f, 1.0f, 0.0f, 0.5f); }
				else
				if (path_matrix[j][i] + color_correction > 8) { EWindow::batch->setcolor(0.0f, 1.0f, 0.0f, 0.5f); }
				else
				if (path_matrix[j][i] + color_correction > 5) { EWindow::batch->setcolor(0.0f, 1.0f, 1.0f, 0.5f); }
				else
				if (path_matrix[j][i] + color_correction == 5) { EWindow::batch->setcolor(1.0f, 1.0f, 1.0f, 0.5f); }
				else
				{ EWindow::batch->setcolor(0.0f, 0.0f, 0.0f, 1.0f); }

			}

			
			EWindow::batch->draw_rect_with_uv(j * 50.0f, i * 50.0f, 45.0f, 45.0f, 0, 0, 1, 1);
		}

		heatmap_phase++;
		if (heatmap_phase > 4) { heatmap_phase = 0; }

		EWindow::batch->setcolor(0.0f, 0.0f, 0.0f, 1.0f);
		EWindow::batch->draw_rect_with_uv(character_position_x - 12.5f, character_position_y - 12.5f, 25.0f, 25.0f, 0, 0, 1, 1);

		
		for (Entity* e : entity_list)
		{
			blocked_by_entity[(int)(*e->position_x / 50.0f)][(int)(*e->position_y / 50.0f)] = false;
		}

		for (Entity* e : entity_list)
		{

			int selected_direction_x = 0;
			int selected_direction_y = 0;

			int min_direction = 99;

			int rounded_pos_x = (int)(*e->position_x / 50.0f);
			int rounded_pos_y = (int)(*e->position_y / 50.0f);

			float dst_x = *e->position_x - character_position_x;
			float dst_y = *e->position_y - character_position_y;
			float dst_total = dst_x * dst_x + dst_y * dst_y;

			if (dst_total >= 3200)
			{
				if ((path_matrix[rounded_pos_x][rounded_pos_y + 1] < min_direction) && (!unwalk_matrix[rounded_pos_x][rounded_pos_y + 1]))
				{
					min_direction = path_matrix[rounded_pos_x][rounded_pos_y + 1];
					selected_direction_x = 0;
					selected_direction_y = 1;

					if (*e->position_x < rounded_pos_x * 50.0f + 20.0f)
					{
						selected_direction_x = 1;
						selected_direction_y = 0;
					}

					if (*e->position_x > rounded_pos_x * 50.0f + 30.0f)
					{
						selected_direction_x = -1;
						selected_direction_y = 0;
					}
				}

				if ((path_matrix[rounded_pos_x + 1][rounded_pos_y] < min_direction) && (!unwalk_matrix[rounded_pos_x + 1][rounded_pos_y]))
				{
					min_direction = path_matrix[rounded_pos_x + 1][rounded_pos_y];
					selected_direction_x = 1;
					selected_direction_y = 0;

					if (*e->position_y < rounded_pos_y * 50.0f + 20.0f)
					{
						selected_direction_x = 0;
						selected_direction_y = 1;
					}

					if (*e->position_y > rounded_pos_y * 50.0f + 30.0f)
					{
						selected_direction_x = 0;
						selected_direction_y = -1;
					}
				}

				if ((path_matrix[rounded_pos_x][rounded_pos_y - 1] < min_direction) && (!unwalk_matrix[rounded_pos_x][rounded_pos_y - 1]))
				{
					min_direction = path_matrix[rounded_pos_x][rounded_pos_y - 1];
					selected_direction_x = 0;
					selected_direction_y = -1;

					if (*e->position_x < rounded_pos_x * 50.0f + 20.0f)
					{
						selected_direction_x = 1;
						selected_direction_y = 0;
					}

					if (*e->position_x > rounded_pos_x * 50.0f + 30.0f)
					{
						selected_direction_x = -1;
						selected_direction_y = 0;
					}
				}

				if ((path_matrix[rounded_pos_x - 1][rounded_pos_y] < min_direction) && (!unwalk_matrix[rounded_pos_x - 1][rounded_pos_y]))
				{
					min_direction = path_matrix[rounded_pos_x - 1][rounded_pos_y];
					selected_direction_x = -1;
					selected_direction_y = 0;

					if (*e->position_y < rounded_pos_y * 50.0f + 20.0f)
					{
						selected_direction_x = 0;
						selected_direction_y = 1;
					}

					if (*e->position_y > rounded_pos_y * 50.0f + 30.0f)
					{
						selected_direction_x = 0;
						selected_direction_y = -1;
					}
				}
			}

			
				*e->position_x += 100.0f * delta_time * selected_direction_x;
				*e->position_y += 100.0f * delta_time * selected_direction_y;



			EWindow::batch->setcolor(0.0f, 1.0f, 1.0f, 1.0f);

			//int temp_x = *e->position_x;
			EWindow::batch->draw_rect_with_uv(*e->position_x - 12.5f, *e->position_y - 12.5f, 25.0f, 25.0f, 0, 0, 1, 1);
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

		correction_x = 1.0 / EWindow::SCR_WIDTH * 2.0;
		correction_y = 1.0 / EWindow::SCR_HEIGHT * 2.0;

		//std::cout << "helper correction_x: " << correction_x << " correction_y: " << correction_y << std::endl;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

void mouse_position_callback(GLFWwindow* window, double _x, double _y)
{

}

void char_input_callback(GLFWwindow* window, unsigned int _char)
{

}

void processInput(GLFWwindow* window)
{
	if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)) {character_position_y += 100.0f * delta_time;}
	if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)) {character_position_y -= 100.0f * delta_time;}

	if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)) {character_position_x -= 100.0f * delta_time;}
	if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {character_position_x += 100.0f * delta_time;}

	if ((glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS))
	{
		for (Entity* e : entity_list)
		{
			*e->position_x = (rand() % 1000);
			*e->position_y = (rand() % 1000);
		}
	}

	if ((glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS))
	{
		for (int i = 0; i < 50; i++)
			for (int j = 0; j < 50; j++)
			{

				if (rand() % 5 == 0)
				{
					unwalk_matrix[j][i] = true;
				}
				else
				{
					unwalk_matrix[j][i] = false;
				}

				path_matrix[j][i] = 99;
				blocked_by_entity[j][i] = false;
			}
	}
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


