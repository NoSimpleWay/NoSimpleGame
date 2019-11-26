#pragma once
#include "EWindowGame.h"
#include <iostream>
#include "EPathMatrix.h"



	EWindowGame::EWindowGame() : EWindow()
	{
		path_to_player_matrix = new EPathMatrix();

		//generate clusters
		for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
		{
			cluster[j][i] = new ECluster();
		}
		
		for (int i = 0; i < 10; i++)
		{
			Entity* en = new Entity(rand() % 1000 + 100.0f, rand() % 1000 + 100.0f);
			en->link_to_path_matrix = path_to_player_matrix;

			put_entity_to_cluster(en);
			//entity_list.push_back(en);
			//entity_list.push_back(new Entity(300.0f, 150.0f));
			//entity_list.push_back(new Entity(400.0f, 150.0f));
		}
		std::cout << "game window created" << std::endl;
		

	}

	void EWindowGame::update(float _d)
	{
		cluster_calculator_left_border =	(int)(character_position_x / CLUSTER_SIZE) - 1; if (cluster_calculator_left_border < 0) {cluster_calculator_left_border = 0;}
		cluster_calculator_right_border =	(int)(character_position_x / CLUSTER_SIZE) + 1; if (cluster_calculator_right_border > 19) { cluster_calculator_right_border = 19;}
		
		cluster_calculator_down_border =	(int)(character_position_y / CLUSTER_SIZE) - 1; if (cluster_calculator_down_border < 0) { cluster_calculator_down_border = 0;}
		cluster_calculator_up_border =		(int)(character_position_y / CLUSTER_SIZE) + 1; if (cluster_calculator_up_border > 19) { cluster_calculator_up_border = 19;}

		path_to_player_matrix->path_calculation_cooldown -= _d;

		if (path_to_player_matrix->path_calculation_cooldown <= 0)
		{
			//unwalkable element created by entity
			for (int i = cluster_calculator_down_border; i <= cluster_calculator_up_border; i++)
			for (int j = cluster_calculator_left_border; j <= cluster_calculator_right_border; j++)
			for (Entity* e : cluster[j][i]->entity_list)
			{
				path_to_player_matrix->blocked_by_entity[(int)(*e->position_x / PATH_CELL_SIZE)][(int)(*e->position_y / PATH_CELL_SIZE)][path_to_player_matrix->back_path_buffer] = true;
				path_to_player_matrix->blocked_by_entity[(int)(*e->position_x / PATH_CELL_SIZE)][(int)(*e->position_y / PATH_CELL_SIZE)][path_to_player_matrix->current_path_buffer] = true;
			}

			//calculate heatmap matrix
			path_to_player_matrix->default_update(_d);

			//reset cooldown
			path_to_player_matrix->path_calculation_cooldown += 0.025f;
		}

		//change cluster master
		for (int i = cluster_calculator_down_border; i <= cluster_calculator_up_border; i++)
		for (int j = cluster_calculator_left_border; j <= cluster_calculator_right_border; j++)
		for (int k = 0; k < cluster[j][i]->entity_list.size(); k++)
		if (!*cluster[j][i]->entity_list.at(k)->already_updated)
		{
			Entity* e = cluster[j][i]->entity_list.at(k);
			*e->already_updated = true;

			path_to_player_matrix->blocked_by_entity[(int)(*e->position_x / PATH_CELL_SIZE)][(int)(*e->position_y / PATH_CELL_SIZE)][path_to_player_matrix->back_path_buffer] = false;
			path_to_player_matrix->blocked_by_entity[(int)(*e->position_x / PATH_CELL_SIZE)][(int)(*e->position_y / PATH_CELL_SIZE)][path_to_player_matrix->current_path_buffer] = false;

			//entity actions AI
			e->AI_control->default_update(e, _d);
			e->AI_control->update(e, _d);

			e->default_update(_d);

			if ((e->prev_cluster_position_x != e->new_cluster_position_x) || (e->prev_cluster_position_y != e->new_cluster_position_y))
			{
				put_entity_to_cluster(e);

				cluster[e->prev_cluster_position_x][e->prev_cluster_position_y]->entity_list.erase(cluster[e->prev_cluster_position_x][e->prev_cluster_position_y]->entity_list.begin() + k);

				k++;
			}
		}

		for (int i = cluster_calculator_down_border; i <= cluster_calculator_up_border; i++)
		for (int j = cluster_calculator_left_border; j <= cluster_calculator_right_border; j++)
		for (Entity* e : cluster[j][i]->entity_list)
		{
			*e->already_updated = false;
		}

		//target goal of entity
		path_to_player_matrix->path_position_x = (int)(character_position_x / PATH_CELL_SIZE);
		path_to_player_matrix->path_position_y = (int)(character_position_y / PATH_CELL_SIZE);






		
		//entity move to path finding
		//for (Entity* e : entity_list)
		//{
//
		//	e->control->update(e, _d);
		//	
		//}

	}

	void EWindowGame::draw(float _d)
	{
		camera_x = (SCR_WIDTH / 2.0f - character_position_x) * correction_x;
		camera_y = (SCR_HEIGHT / 2.0f - character_position_y) * correction_y;
		EWindow::matrix_transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

		EWindow::matrix_transform = glm::translate(EWindow::matrix_transform, glm::vec3(-1 + camera_x, -1 + camera_y, 0.0f));
		EWindow::matrix_transform = glm::scale(EWindow::matrix_transform, glm::vec3(correction_x, correction_y, 1));

		transformLoc = glGetUniformLocation(EWindow::ourShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(EWindow::matrix_transform));

		
		if (true)
		for (int i = path_to_player_matrix->path_matrix_point_start_y; i < path_to_player_matrix->path_matrix_point_end_y; i++)
		for (int j = path_to_player_matrix->path_matrix_point_start_x; j < path_to_player_matrix->path_matrix_point_end_x; j++)
		{

				//float col = 1.0f - path_matrix[j][i] / 10.0f;
				//EWindow::batch->setcolor(col, col, col, 1.0f);
				/**/

			float col = 1.0f - (path_to_player_matrix->path_matrix[j][i][path_to_player_matrix->current_path_buffer] - 0.0f) / 40.0f;
			batch->setcolor(col, col, col, 1.0f);


			EWindow::batch->draw_rect_with_uv(j * PATH_CELL_SIZE + 1.0f, i * PATH_CELL_SIZE + 1.0f, PATH_CELL_SIZE - 2.0f, PATH_CELL_SIZE - 2.0f, 0, 0, 1, 1);
		}

		EWindow::batch->setcolor(0.0f, 0.0f, 0.0f, 1.0f);
		EWindow::batch->draw_rect_with_uv(character_position_x - 12.5f, character_position_y - 12.5f, 25.0f, 25.0f, 0, 0, 1, 1);

		for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
		for (Entity* e : cluster[j][i]->entity_list)
		{
			EWindow::batch->setcolor(0.0f, 1.0f, 1.0f, 1.0f);

			//int temp_x = *e->position_x;
			EWindow::batch->draw_rect_with_uv(*e->position_x - 12.5f, *e->position_y - 12.5f, 25.0f, 25.0f, 0, 0, 1, 1);
		}

		
		for (int i = path_to_player_matrix->path_matrix_point_start_y; i < path_to_player_matrix->path_matrix_point_end_y; i++)
		for (int j = path_to_player_matrix->path_matrix_point_start_x; j < path_to_player_matrix->path_matrix_point_end_x; j++)
		if (path_to_player_matrix->unwalk_matrix[j][i][path_to_player_matrix->current_path_buffer])
		{
			//if (unwalk_matrix[j][i][current_path_buffer]) { if (unwalk_matrix[j][i][current_path_buffer]) }
			//else { EWindow::batch->setcolor(EColor::COLOR_LIGHT_GRAY); }
			EWindow::batch->setcolor_lum(EColor::COLOR_RED, 0.2f);
			//if (unwalk_matrix[j][i][current_path_buffer])

			EWindow::batch->draw_rect(j * PATH_CELL_SIZE, i * PATH_CELL_SIZE, PATH_CELL_SIZE - 2.0f, PATH_CELL_SIZE - 2.0f);
		}

		/*EWindow::batch->setcolor_alpha(EColor::COLOR_GREEN, 0.2f);
		for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
		{
			if (cluster[j][i]->entity_list.empty())
			{
				EWindow::batch->draw_rect(j * CLUSTER_SIZE, i * CLUSTER_SIZE, CLUSTER_SIZE, CLUSTER_SIZE);
			}

		}*/

		EWindow::batch->setcolor_alpha(EColor::COLOR_BLUE, 0.2f);

		int cursor_path_x = (int)((EWindow::mouse_x - (SCR_WIDTH / 2.0f - character_position_x)) / PATH_CELL_SIZE);
		int cursor_path_y = (int)((EWindow::mouse_y - (SCR_HEIGHT / 2.0f - character_position_y)) / PATH_CELL_SIZE);

		EWindow::batch->draw_rect
		(
			cursor_path_x * PATH_CELL_SIZE,
			cursor_path_y * PATH_CELL_SIZE,
			PATH_CELL_SIZE,
			PATH_CELL_SIZE
		);


		int min_path = path_to_player_matrix->path_matrix[cursor_path_x][cursor_path_y][path_to_player_matrix->current_path_buffer];
		
		

		int choosed_path = -1;

		for (int i = 0; i < 20; i++)
		{
			int cursor_direction_x = 0;
			int cursor_direction_y = 0;

			if (path_to_player_matrix->path_matrix[cursor_path_x][cursor_path_y + 1][path_to_player_matrix->current_path_buffer] < min_path)
			{
				min_path = path_to_player_matrix->path_matrix[cursor_path_x][cursor_path_y + 1][path_to_player_matrix->current_path_buffer];
				cursor_direction_x = 0;
				cursor_direction_y = 1;
			}

			if (path_to_player_matrix->path_matrix[cursor_path_x + 1][cursor_path_y][path_to_player_matrix->current_path_buffer] < min_path)
			{
				min_path = path_to_player_matrix->path_matrix[cursor_path_x + 1][cursor_path_y][path_to_player_matrix->current_path_buffer];
				cursor_direction_x = 1;
				cursor_direction_y = 0;
			}

			if (path_to_player_matrix->path_matrix[cursor_path_x][cursor_path_y - 1][path_to_player_matrix->current_path_buffer] < min_path)
			{
				min_path = path_to_player_matrix->path_matrix[cursor_path_x][cursor_path_y - 1][path_to_player_matrix->current_path_buffer];
				cursor_direction_x = 0;
				cursor_direction_y = -1;
			}

			if (path_to_player_matrix->path_matrix[cursor_path_x - 1][cursor_path_y][path_to_player_matrix->current_path_buffer] < min_path)
			{
				min_path = path_to_player_matrix->path_matrix[cursor_path_x - 1][cursor_path_y][path_to_player_matrix->current_path_buffer];
				cursor_direction_x = -1;
				cursor_direction_y = 0;
			}

			cursor_path_x += cursor_direction_x;
			cursor_path_y += cursor_direction_y;

			EWindow::batch->draw_rect
			(
				cursor_path_x * PATH_CELL_SIZE,
				cursor_path_y * PATH_CELL_SIZE,
				PATH_CELL_SIZE,
				PATH_CELL_SIZE
			);



		}

		EWindow::batch->setcolor_alpha(EColor::COLOR_WHITE, 0.35f);
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
			{
				EWindow::batch->draw_rama(j * CLUSTER_SIZE, i * CLUSTER_SIZE, CLUSTER_SIZE, CLUSTER_SIZE, 2.0f);
			}

	}

	void EWindowGame::put_entity_to_cluster(Entity* _e)
	{
		std::cout << "try place entity to cluster " << (int)(*_e->position_x / CLUSTER_SIZE) << " | " << (int)(*_e->position_y / CLUSTER_SIZE) << std::endl;
		cluster[(int)(*_e->position_x / CLUSTER_SIZE)][(int)(*_e->position_y / CLUSTER_SIZE)]->entity_list.push_back(_e);
	}

	EWindowGame::~EWindowGame()
	{

	}
