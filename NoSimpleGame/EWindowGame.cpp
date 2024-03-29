#pragma once
#include "EWindowGame.h"
#include <iostream>
#include "EPathMatrix.h"




	EWindowGame::EWindowGame() : EWindow()
	{
		
	}

	void EWindowGame::update(float _d)
	{
		if
		(
			(glfwGetKey(EWindow::main_window, GLFW_KEY_PAGE_UP) == GLFW_RELEASE)
			&&
			(glfwGetKey(EWindow::main_window, GLFW_KEY_PAGE_DOWN) == GLFW_RELEASE)
		)
		{
			system_button_release = true;
		}

		if ((glfwGetKey(EWindow::main_window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) && (system_button_release))
		{
			system_button_release = false;
			camera_zoom *= 0.5f;
		}

		if ((glfwGetKey(EWindow::main_window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) && (system_button_release))
		{
			system_button_release = false;
			camera_zoom *= 2.0f;
		}

		start = std::chrono::high_resolution_clock::now();

		time_process_name.clear();
		time_process_value.clear();

		if (shoot_cooldown > 0) { shoot_cooldown -= _d; }

		if ((EWindow::LMB) && (shoot_cooldown <= 0))
		{
			shoot_cooldown += 0.1f;

			Entity* bullet = new Entity(*link_to_player->position_x, *link_to_player->position_y);

			float dstx = EWindow::mouse_x - (EGraphicCore::SCR_WIDTH / 2.0f - character_position_x) - character_position_x;
			float dsty = EWindow::mouse_y - (EGraphicCore::SCR_HEIGHT / 2.0f - character_position_y) - character_position_y;


			float totaldst = pow(dstx * dstx + dsty * dsty, 0.5f);



			*bullet->speed_x = 5000.0f * dstx / totaldst;
			*bullet->speed_y = 5000.0f * dsty / totaldst;
			*bullet->mass = 0.001f;
			*bullet->friction = 0.9f;
			*bullet->is_bullet = true;
			*bullet->sprite_size_x = 5.0f;
			*bullet->sprite_size_y = 5.0f;
			*bullet->collision_size_up = 2.5f;
			*bullet->collision_size_down = 2.5f;
			*bullet->collision_size_right = 2.5f;
			*bullet->collision_size_left = 2.5f;
			*bullet->step_cooldown = 0.2f;

			bullet->link_to_path_matrix = path_to_player_matrix;

			put_bullet_to_cluster(bullet);
		}

		character_position_x = *link_to_player->position_x;
		character_position_y = *link_to_player->position_y;

		cluster_calculator_left_border =	(int)(character_position_x / CLUSTER_SIZE) - 2; if (cluster_calculator_left_border < 0) {cluster_calculator_left_border = 0;}
		cluster_calculator_right_border =	(int)(character_position_x / CLUSTER_SIZE) + 2; if (cluster_calculator_right_border > 19) { cluster_calculator_right_border = 19;}
		
		cluster_calculator_down_border =	(int)(character_position_y / CLUSTER_SIZE) - 1; if (cluster_calculator_down_border < 0) { cluster_calculator_down_border = 0;}
		cluster_calculator_up_border =		(int)(character_position_y / CLUSTER_SIZE) + 1; if (cluster_calculator_up_border > 19) { cluster_calculator_up_border = 19;}

		path_to_player_matrix->path_calculation_cooldown -= _d;

		if (path_to_player_matrix->path_calculation_cooldown <= 0)
		{
			//unwalkable element created by entity
			for (int i = cluster_calculator_down_border; i <= cluster_calculator_up_border; i++)
			for (int j = cluster_calculator_left_border; j <= cluster_calculator_right_border; j++)
			for (Entity* e : cluster[j][i]->entity_list)
			if (!*e->is_dead)
			{
				path_to_player_matrix->blocked_by_entity[(int)(*e->position_x / PATH_CELL_SIZE)][(int)(*e->position_y / PATH_CELL_SIZE)][path_to_player_matrix->back_path_buffer] = true;
				path_to_player_matrix->blocked_by_entity[(int)(*e->position_x / PATH_CELL_SIZE)][(int)(*e->position_y / PATH_CELL_SIZE)][path_to_player_matrix->current_path_buffer] = true;
			}

			//calculate heatmap matrix
			path_to_player_matrix->default_update(_d);

			//reset cooldown
			path_to_player_matrix->path_calculation_cooldown += 0.025f;
		}
		
		add_time_process("create bullets");
	
		float reverse_delta = 1 - _d;
		float delta_correction = _d * (1.0f + reverse_delta * reverse_delta);

		//change cluster master
		for (int i = cluster_calculator_down_border; i <= cluster_calculator_up_border; i++)
		for (int j = cluster_calculator_left_border; j <= cluster_calculator_right_border; j++)
		for (int wtf = 0; wtf < cluster[j][i]->entity_list.size(); wtf++)
		if ((!*cluster[j][i]->entity_list.at(wtf)->already_updated) && (!*cluster[j][i]->entity_list.at(wtf)->is_dead))
		{
			Entity* e = cluster[j][i]->entity_list.at(wtf);
			*e->already_updated = true;

			path_to_player_matrix->blocked_by_entity[(int)(*e->position_x / PATH_CELL_SIZE)][(int)(*e->position_y / PATH_CELL_SIZE)][path_to_player_matrix->back_path_buffer] = false;
			path_to_player_matrix->blocked_by_entity[(int)(*e->position_x / PATH_CELL_SIZE)][(int)(*e->position_y / PATH_CELL_SIZE)][path_to_player_matrix->current_path_buffer] = false;

			

			//entity actions AI
			if (e->AI_control != NULL)
			{
				e->AI_control->default_update(e, _d);
				e->AI_control->update(e, _d);

				e->AI_control->acceleration_control(e, delta_correction);
			}

			*e->next_x = *e->position_x + *e->speed_x * _d; if (*e->next_x < 10) { *e->next_x = 10; } if (*e->next_x > 20.0 * CLUSTER_SIZE) { *e->next_x = 20.0f * CLUSTER_SIZE - 10.0f; }
			*e->next_y = *e->position_y + *e->speed_y * _d; if (*e->next_y < 10) { *e->next_y = 10; } if (*e->next_y > 20.0 * CLUSTER_SIZE) { *e->next_y = 20.0f * CLUSTER_SIZE - 10.0f; }

			bool collision_detect = false;
			int collision_side = -1;

			for (int a = j - 1; a <= j + 1; a++)
			for (int b = i - 1; b <= i + 1; b++)
			if ((a >= 0) && (a < 20) && (b>=0) && (b<20))
			for (int f = 0; f < cluster[a][b]->entity_list.size(); f++)
			if (!*cluster[a][b]->entity_list.at(f)->is_dead)
			{
				Entity* e2 = cluster[a][b]->entity_list.at(f);



				if ((*e->speed_x * *e->speed_x > 0) || (*e->speed_y * *e->speed_y > 0))
				{
					collision_side = check_collision(e, e2);
				}

				if (collision_side != -1)
				{
					collision_detect = true;

					//*e->speed_x = 0;

					float total_mass = *e->mass + *e2->mass;

					float mass_multiplier_a = *e->mass / total_mass;
					float mass_multiplier_b =  *e2->mass / total_mass;

					float speed_difference = 0.0f;
					if
					(
						(
							(*e->speed_x > 0) && (*e->speed_x > *e2->speed_x)
							||
							(*e->speed_x < 0) && (*e->speed_x < *e2->speed_x)
						)
					)
					{
						speed_difference = *e->speed_x - *e2->speed_x;

						*e->speed_x -= speed_difference * mass_multiplier_b;
						*e2->speed_x += speed_difference * mass_multiplier_a;

						//*e->speed_y *= pow(0.1, _d);
					}

					if
					(
						(
							(*e->speed_y > 0) && (*e->speed_y > *e2->speed_y)
							||
							(*e->speed_y < 0) && (*e->speed_y < *e2->speed_y)
						)

					)
					{
						speed_difference = *e->speed_y - *e2->speed_y;

						*e->speed_y -= speed_difference * mass_multiplier_b;
						*e2->speed_y += speed_difference * mass_multiplier_a;

						//*e->speed_x *= pow(0.1, _d);
					}

					
					/*
					if (collision_side == 0){e->move_relative( 0.0f, (*e2->position_y - *e->position_y + *e2->collision_size_up + *e->collision_size_down) * 0.99f);}
					if (collision_side == 1) { e->move_relative((*e2->position_x - *e->position_x + *e2->collision_size_right + *e->collision_size_left) * 0.99f, 0.0f); }
					
					if (collision_side == 2) { e->move_relative(0.0f, (*e2->position_y - *e->position_y - *e2->collision_size_down - *e->collision_size_up) * 0.99f); }
					if (collision_side == 3) { e->move_relative((*e2->position_x - *e->position_x - *e2->collision_size_left - *e->collision_size_right) * 0.99f, 0.0f); }
					*/
					//if (collision_side == 0){*e->speed_y = (*e2->position_y - *e->position_y + *e2->collision_size_up + *e->collision_size_down) * 0.8f;}
				}
			}


			
			e->default_update(_d);

			if (!collision_detect) { e->move(_d); }

			float friction_multiplier = pow(*e->friction, _d);

			*e->speed_x *= friction_multiplier;
			*e->speed_y *= friction_multiplier;

			if ((e->prev_cluster_position_x != e->new_cluster_position_x) || (e->prev_cluster_position_y != e->new_cluster_position_y))
			{
				put_entity_to_cluster(e);
				cluster[j][i]->entity_list.erase(cluster[j][i]->entity_list.begin() + wtf);
				wtf++;
			}
		}

		add_time_process("entity control and collision");

		//bullet clusters
		for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
		for (int wtf = 0; wtf < cluster[j][i]->bullet_list.size(); wtf++)
		if ((!*cluster[j][i]->bullet_list.at(wtf)->already_updated) && (!*cluster[j][i]->bullet_list.at(wtf)->is_dead))
		{
			Entity* bu = cluster[j][i]->bullet_list.at(wtf);
			*bu->already_updated = true;

			//entity actions AI

			*bu->next_x = *bu->position_x + *bu->speed_x * _d; if (*bu->next_x < 10) { *bu->next_x = 10; *bu->is_dead = true; } if (*bu->next_x > 20.0 * CLUSTER_SIZE) { *bu->next_x = 20.0f * CLUSTER_SIZE - 10.0f; *bu->is_dead = true; }
			*bu->next_y = *bu->position_y + *bu->speed_y * _d; if (*bu->next_y < 10) { *bu->next_y = 10; *bu->is_dead = true; } if (*bu->next_y > 20.0 * CLUSTER_SIZE) { *bu->next_y = 20.0f * CLUSTER_SIZE - 10.0f; *bu->is_dead = true;}

			bool collision_detect = false;
			int collision_side = -1;

			for (int a = j - 1; a <= j + 1; a++)
			for (int b = i - 1; b <= i + 1; b++)
				if ((a >= 0) && (a < 20) && (b >= 0) && (b < 20))
				for (int f = 0; f < cluster[a][b]->entity_list.size(); f++)
							if (!*cluster[a][b]->entity_list.at(f)->is_dead)
							{
								Entity* e2 = cluster[a][b]->entity_list.at(f);



								if ((*bu->speed_x * *bu->speed_x > 0) || (*bu->speed_y * *bu->speed_y > 0))
								{collision_side = check_collision(bu, e2);}

								if (collision_side != -1)
								{
									bu->default_collision_action(bu, e2, collision_side);

									collision_detect = true;

									//*e->speed_x = 0;

									/*
									if (collision_side == 0){e->move_relative( 0.0f, (*e2->position_y - *e->position_y + *e2->collision_size_up + *e->collision_size_down) * 0.9f);}
									if (collision_side == 1) { e->move_relative((*e2->position_x - *e->position_x + *e2->collision_size_right + *e->collision_size_left) * 0.9f, 0.0f); }

									if (collision_side == 2) { e->move_relative(0.0f, (*e2->position_y - *e->position_y - *e2->collision_size_down - *e->collision_size_up) * 0.9f); }
									if (collision_side == 3) { e->move_relative((*e2->position_x - *e->position_x - *e2->collision_size_left - *e->collision_size_right) * 0.9f, 0.0f); }
									*/
									//if (collision_side == 0){*e->speed_y = (*e2->position_y - *e->position_y + *e2->collision_size_up + *e->collision_size_down) * 0.8f;}
								}
							}



			bu->default_update(_d);

			if (!collision_detect) { bu->move(_d); }

			float friction_multiplier = pow(*bu->friction, _d);

			*bu->speed_x *= friction_multiplier;
			*bu->speed_y *= friction_multiplier;

			if ((bu->prev_cluster_position_x != bu->new_cluster_position_x) || (bu->prev_cluster_position_y != bu->new_cluster_position_y))
			{
				put_bullet_to_cluster(bu);
				cluster[j][i]->bullet_list.erase(cluster[j][i]->bullet_list.begin() + wtf);
				wtf++;
			}
		}

		add_time_process("bullet update process");

			for (int i = cluster_calculator_down_border; i <= cluster_calculator_up_border; i++)
			for (int j = cluster_calculator_left_border; j <= cluster_calculator_right_border; j++)
			for (Entity* e : cluster[j][i]->entity_list)
			{
				*e->already_updated = false;
			}

			for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
			for (Entity* bu : cluster[j][i]->bullet_list)
			{
				*bu->already_updated = false;
			}

			//target goal of entity
			path_to_player_matrix->path_position_x = (int)(character_position_x / PATH_CELL_SIZE);
			path_to_player_matrix->path_position_y = (int)(character_position_y / PATH_CELL_SIZE);
	}

	void EWindowGame::draw(float _d)
	{

		camera_x = round(EGraphicCore::SCR_WIDTH / 2.0f - character_position_x * camera_zoom) * EGraphicCore::correction_x;
		camera_y = round(EGraphicCore::SCR_HEIGHT / 2.0f - character_position_y * camera_zoom) * EGraphicCore::correction_y;
		EGraphicCore::matrix_transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

		EGraphicCore::matrix_transform = glm::translate(EGraphicCore::matrix_transform, glm::vec3(-1 + camera_x, -1 + camera_y, 0.0f));
		EGraphicCore::matrix_transform = glm::scale(EGraphicCore::matrix_transform, glm::vec3(EGraphicCore::correction_x * camera_zoom, EGraphicCore::correction_y * camera_zoom, 1));

		transformLoc = glGetUniformLocation(EGraphicCore::ourShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(EGraphicCore::matrix_transform));

		EGraphicCore::batch->setcolor(EColor::COLOR_WHITE);

		//ground tileset
		for (int i = path_to_player_matrix->path_matrix_point_start_y; i < path_to_player_matrix->path_matrix_point_end_y; i++)
		for (int j = path_to_player_matrix->path_matrix_point_start_x; j < path_to_player_matrix->path_matrix_point_end_x; j++)
		{
			EGraphicCore::batch->draw_gabarite_with_offset
			(
				j * 50.0f,
				i * 50.0f,
				50.0f,
				50.0f,
				tilemap_uv_start_x[j][i],
				tilemap_uv_start_y[j][i],
				tilemap_uv_end_x[j][i],
				tilemap_uv_end_y[j][i],
				EGraphicCore::gabarite_tileset
			);
		}

		add_time_process("draw tileset");


		//path visualise
		if (false)
		{
			for (int i = path_to_player_matrix->path_matrix_point_start_y; i < path_to_player_matrix->path_matrix_point_end_y; i++)
			for (int j = path_to_player_matrix->path_matrix_point_start_x; j < path_to_player_matrix->path_matrix_point_end_x; j++)
			{

				//float col = 1.0f - path_matrix[j][i] / 10.0f;
				//EWindow::batch->setcolor(col, col, col, 1.0f);
				/**/

				float col = 1.0f - (path_to_player_matrix->path_matrix[j][i][path_to_player_matrix->current_path_buffer] - 0.0f) / 40.0f;
				EGraphicCore::batch->setcolor(col, col, col, 1.0f);


				EGraphicCore::batch->draw_gabarite(j * PATH_CELL_SIZE + 1.0f, i * PATH_CELL_SIZE + 1.0f, PATH_CELL_SIZE - 2.0f, PATH_CELL_SIZE - 2.0f, EGraphicCore::gabarite_white_pixel);
			}

			add_time_process("path visualise");
		}

		if (false)
		for (int i = path_to_player_matrix->path_matrix_point_start_y; i < path_to_player_matrix->path_matrix_point_end_y; i++)
		for (int j = path_to_player_matrix->path_matrix_point_start_x; j < path_to_player_matrix->path_matrix_point_end_x; j++)
		if (path_to_player_matrix->unwalk_matrix[j][i][path_to_player_matrix->current_path_buffer])
		{
			//if (unwalk_matrix[j][i][current_path_buffer]) { if (unwalk_matrix[j][i][current_path_buffer]) }
			//else { EWindow::batch->setcolor(EColor::COLOR_LIGHT_GRAY); }
			EGraphicCore::batch->setcolor_lum(EColor::COLOR_RED, 0.2f);
			//if (unwalk_matrix[j][i][current_path_buffer])

			EGraphicCore::batch->draw_gabarite(j * PATH_CELL_SIZE, i * PATH_CELL_SIZE, PATH_CELL_SIZE - 2.0f, PATH_CELL_SIZE - 2.0f, EGraphicCore::gabarite_white_pixel);
		}

		add_time_process("draw unwalk matrix");

		

		//entity draw
		for (int i = cluster_calculator_up_border; i >= cluster_calculator_down_border; i--)
		{
			//reset_draw_buffer
			draw_buffer.clear();

			//fill draw_buffer
			for (int j = cluster_calculator_left_border; j <= cluster_calculator_right_border; j++)
			for (Entity* e : cluster[j][i]->entity_list)
			if (!*e->is_dead)
			{
				draw_buffer.push_back(e);
			}

			int swap_count = 0;
			bool already_sort = false;

			//sort draw_buffer
			while ((!already_sort) && (swap_count < 1000) && (draw_buffer.size() > 0))
			{
				already_sort = true;

				for (int k = 0; k < draw_buffer.size() - 1; k++)
				{
					Entity* e = draw_buffer.at(k);
					Entity* e_next = draw_buffer.at(k + 1);

					if (*e->position_y < *e_next->position_y)
					{
						swap_count++;
						already_sort = false;
						//Entity* swap = e;
						draw_buffer.at(k) = e_next;
						draw_buffer.at(k + 1) = e;
						//EGraphicCore::batch->setcolor(0.0f, 1.0f, 1.0f, 1.0f);
					}
				}

				if (swap_count > 10000)
				{
					std::cout << "Too many swaps" << std::endl;
				}
			}

			for (Entity* e : draw_buffer)
			{
				

				//int temp_x = *e->position_x;
				for (ESprite* s : e->sprite_list)
				{
					EGraphicCore::batch->setcolor(e->color);

					//std::cout << *s->sprite_x << std::endl;
					EGraphicCore::batch->draw_gabarite(*e->position_x + *s->sprite_x, *e->position_y + *s->sprite_y, *s->sprite_size_x, *s->sprite_size_y, s->gabarite);

					if (false)
					{
						EGraphicCore::batch->setcolor_alpha(EColor::COLOR_RED, 0.5f);
						EGraphicCore::batch->draw_rama
						(
							*e->position_x - *e->collision_size_left,
							*e->position_y - *e->collision_size_down,
							*e->collision_size_left + *e->collision_size_right,
							*e->collision_size_down + *e->collision_size_up,
							1.0f,
							EGraphicCore::gabarite_white_pixel
						);
					}
				}
			}

			
		}

		add_time_process("draw entity");

		for (int i = cluster_calculator_down_border; i <= cluster_calculator_up_border; i++)
		for (int j = cluster_calculator_left_border; j <= cluster_calculator_right_border; j++)
		for (Entity* bu : cluster[j][i]->bullet_list)
		if (!*bu->is_dead)
		{
			EGraphicCore::batch->setcolor(0.0f, 1.0f, 1.0f, 1.0f);

			//int temp_x = *e->position_x;
			EGraphicCore::batch->draw_gabarite(*bu->position_x - *bu->sprite_size_x / 2.0f, *bu->position_y - *bu->sprite_size_y / 2.0f, *bu->sprite_size_x, *bu->sprite_size_y, EGraphicCore::gabarite_white_pixel);

			EGraphicCore::batch->setcolor_alpha(EColor::COLOR_RED, 0.5f);
			EGraphicCore::batch->draw_rama
			(
				*bu->position_x - *bu->collision_size_left,
				*bu->position_y - *bu->collision_size_down,
				*bu->collision_size_left + *bu->collision_size_right,
				*bu->collision_size_down + *bu->collision_size_up,
				1.0f,
				EGraphicCore::gabarite_white_pixel
			);
		}

		add_time_process("draw bullets");

		



		EGraphicCore::batch->setcolor_alpha(EColor::COLOR_BLUE, 0.2f);

		int cursor_path_x = (int)((EWindow::mouse_x - (EGraphicCore::SCR_WIDTH / 2.0f - character_position_x)) / PATH_CELL_SIZE);
		int cursor_path_y = (int)((EWindow::mouse_y - (EGraphicCore::SCR_HEIGHT / 2.0f - character_position_y)) / PATH_CELL_SIZE);

		
		if (EWindow::RMB)
		{
			path_to_player_matrix->unwalk_matrix[cursor_path_x][cursor_path_y][0] = false;
			path_to_player_matrix->unwalk_matrix[cursor_path_x][cursor_path_y][1] = false;
		}

		int min_path = path_to_player_matrix->path_matrix[cursor_path_x][cursor_path_y][path_to_player_matrix->current_path_buffer];
		
		int choosed_path = -1;

		if (glfwGetKey(EWindow::main_window, GLFW_KEY_END) == GLFW_PRESS) 
		{
			EGraphicCore::batch->setcolor(EColor::COLOR_GRAY);
			EGraphicCore::batch->draw_gabarite(0, 0, 4096.0f, 4069.0f, EGraphicCore::gabarite_white_pixel);

			EGraphicCore::batch->setcolor(EColor::COLOR_WHITE);
			EGraphicCore::batch->draw_rect(0, 0, 4096.0f, 4096.0f);
		}



	}
	void EWindowGame::draw_interface(float _d)
	{	
		EGraphicCore::batch->reinit();
		EGraphicCore::batch->draw_call();
		EGraphicCore::batch->reset();

		EGraphicCore::matrix_transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

		EGraphicCore::matrix_transform = glm::translate(EGraphicCore::matrix_transform, glm::vec3(-1, -1, 0.0f));
		EGraphicCore::matrix_transform = glm::scale(EGraphicCore::matrix_transform, glm::vec3(EGraphicCore::correction_x, EGraphicCore::correction_y, 1));

		transformLoc = glGetUniformLocation(EGraphicCore::ourShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(EGraphicCore::matrix_transform));

		float microseconds = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000000.0f;


		if (glfwGetKey(EWindow::main_window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
		{

			EGraphicCore::batch->setcolor_alpha(EColor::COLOR_BLACK, 0.9f);
			EGraphicCore::batch->draw_gabarite(0.0f, 0.0f, 400.0f, EGraphicCore::SCR_HEIGHT, EGraphicCore::gabarite_white_pixel);

			for (int i = 0; i < time_process_name.size(); i++)
			{
				EGraphicCore::batch->setcolor(EColor::COLOR_WHITE);
				EFont::active_font->draw(EGraphicCore::batch, time_process_name.at(i), 8.0f, EGraphicCore::SCR_HEIGHT - i * 17.0f - 17.0f);

				EGraphicCore::batch->setcolor(EColor::COLOR_GREEN);
				EFont::active_font->draw(EGraphicCore::batch, std::to_string(time_process_value.at(i)), 208.0f, EGraphicCore::SCR_HEIGHT - i * 17.0f - 17.0f);
			}
		}
	}

	void EWindowGame::put_bullet_to_cluster (Entity* _e)
	{
		//std::cout << "try place entity to cluster " << (int)(*_e->position_x / CLUSTER_SIZE) << " | " << (int)(*_e->position_y / CLUSTER_SIZE) << std::endl;
		cluster[(int)(*_e->position_x / CLUSTER_SIZE)][(int)(*_e->position_y / CLUSTER_SIZE)]->bullet_list.push_back(_e);

		_e->prev_cluster_position_x = (int)(*_e->position_x / CLUSTER_SIZE);
		_e->prev_cluster_position_y = (int)(*_e->position_y / CLUSTER_SIZE);
	}

	void EWindowGame::put_entity_to_cluster(Entity* _e)
	{
		//std::cout << "try place entity to cluster " << (int)(*_e->position_x / CLUSTER_SIZE) << " | " << (int)(*_e->position_y / CLUSTER_SIZE) << std::endl;
		cluster[(int)(*_e->position_x / CLUSTER_SIZE)][(int)(*_e->position_y / CLUSTER_SIZE)]->entity_list.push_back(_e);

		_e->prev_cluster_position_x = (int)(*_e->position_x / CLUSTER_SIZE);
		_e->prev_cluster_position_y = (int)(*_e->position_y / CLUSTER_SIZE);
	}

	/*
	void EWindowGame::put_bullet_to_cluster(Entity* _e)
	{

	}*/

	int EWindowGame::check_collision(Entity* _a, Entity* _b)
	{
		//up to down
		float pseudo_ray = *_b->position_y + *_a->collision_size_down + *_b->collision_size_up;
		float projection_point = *_a->position_x - (*_a->position_y - pseudo_ray) * *_a->speed_x / *_a->speed_y;
		if
		(
			(*_a->position_y >= pseudo_ray)
			&&
			(*_a->next_y <= pseudo_ray)
			&&
			( projection_point >= *_b->position_x - *_a->collision_size_right -*_b->collision_size_left )
			&&
			(projection_point <= *_b->position_x + *_a->collision_size_left + *_b->collision_size_right)
			//&&
			//(*_a->collision_size_left + *_b->collision_size_right >= *_a->speed_x * *_a->speed_x / (- *_a->speed_y))
		)
		{ return 0; }


		//down to up
		pseudo_ray = *_b->position_y - *_a->collision_size_up - *_b->collision_size_down;
		if
		(
			(*_a->position_y <= pseudo_ray)// first point position
			&&
			(*_a->next_y >= pseudo_ray)//second point position
			&&//42 + (42 - 57) * -1 / 1 = 42 - 15 * -1 = 42 + 15
			(*_a->position_x - (*_a->position_y - pseudo_ray) * *_a->speed_x / *_a->speed_y >= *_b->position_x - *_a->collision_size_right -*_b->collision_size_left )
			&&
			(*_a->position_x - (*_a->position_y - pseudo_ray) * *_a->speed_x / *_a->speed_y <= *_b->position_x + *_a->collision_size_left + *_b->collision_size_right)
			//&&
			//(*_a->collision_size_left + *_b->collision_size_right >= *_a->speed_x * *_a->speed_x / (- *_a->speed_y))
		)
		{ return 2; }


		//left to right
		pseudo_ray = *_b->position_x - *_a->collision_size_right - *_b->collision_size_left;
		if
		(
			(*_a->position_x <= pseudo_ray)
			&&
			(*_a->next_x >= pseudo_ray)
			&&//			-50
			(*_a->position_y - (*_a->position_x - pseudo_ray) * *_a->speed_y / *_a->speed_x >= *_b->position_y - *_a->collision_size_up - *_b->collision_size_down)
			&&
			(*_a->position_y - (*_a->position_x - pseudo_ray) * *_a->speed_y / *_a->speed_x <= *_b->position_y + *_a->collision_size_down + *_b->collision_size_up)
			//&&
			//(*_a->collision_size_left + *_b->collision_size_right >= *_a->speed_x * *_a->speed_x / (- *_a->speed_y))
		)
		{
			return 3;
		}

		//left to right
		pseudo_ray = *_b->position_x + *_a->collision_size_left + *_b->collision_size_right;
		if
		(
		(*_a->position_x >= pseudo_ray)
		&&
		(*_a->next_x <= pseudo_ray)
		&&//33	-	(61 - 48) * (1 / -1) = 33 - 13 * -1 = 33 + 13 = 46
		(*_a->position_y - (*_a->position_x - pseudo_ray) * *_a->speed_y / *_a->speed_x >= *_b->position_y - *_a->collision_size_up - *_b->collision_size_down)
		&&
		(*_a->position_y - (*_a->position_x - pseudo_ray) * *_a->speed_y / *_a->speed_x <= *_b->position_y + *_a->collision_size_down + *_b->collision_size_up)
		//&&
		//(*_a->collision_size_left + *_b->collision_size_right >= *_a->speed_x * *_a->speed_x / (- *_a->speed_y))
		)
		{
			return 1;
		}

		return -1;
	}

	void EWindowGame::init()
	{
		path_to_player_matrix = new EPathMatrix();

		//generate clusters
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++)
			{
				cluster[j][i] = new ECluster();
			}

		//generate tileset
		for (int i = 0; i < 200; i++)
			for (int j = 0; j < 200; j++)
			{
				int selected_tile_x = rand() % 6;
				int selected_tile_y = rand() % 6 + 4;

				tilemap_uv_start_x[j][i] = 50.0f * selected_tile_x / 4096.0f;
				tilemap_uv_start_y[j][i] = 50.0f * selected_tile_y / 4096.0f;

				tilemap_uv_end_x[j][i] = (50.0f * selected_tile_x + 50.0f) / 4096.0f;
				tilemap_uv_end_y[j][i] = (50.0f * selected_tile_y + 50.0f) / 4096.0f;
			}

		for (int i = 0; i < 400; i++)
		{
			Entity* en = new Entity(rand() % 2400 + 100, rand() % 2400 + 100);

			//*en->collision_size_right -= 10.0f * i;


			en->link_to_path_matrix = path_to_player_matrix;
			en->AI_control = Entity::static_control_movable_enemy;
			en->sprite_list.push_back(new ESprite(ETextureAtlas::put_texture_to_atlas("data/sphere.png", EWindow::window_game->terrain_atlas), 0.0f, 0.0f, 0.0f, 0.0f));

			put_entity_to_cluster(en);
			//entity_list.push_back(en);
			//entity_list.push_back(new Entity(300.0f, 150.0f));
			//entity_list.push_back(new Entity(400.0f, 150.0f));
		}

		for (int j = 0; j < 20; j++)
		for (int i = 0; i < 20; i++)
		if (rand() % 15 == 0)
		{
			Entity* en = new Entity(j * 150.0f + 75.0f, i * 100.0f);

			for (int x = j * 3; x < j * 3 + 3; x++)
			for (int y = i * 2; y < i * 2 + 2; y++)
			{
				path_to_player_matrix->unwalk_matrix[x][y][0] = true;
				path_to_player_matrix->unwalk_matrix[x][y][1] = true;
			}

			//*en->collision_size_right -= 10.0f * i;


			en->link_to_path_matrix = path_to_player_matrix;
			//en->AI_control = Entity::static_control_movable_enemy;
			en->sprite_list.push_back(new ESprite(ETextureAtlas::put_texture_to_atlas("data/pixel_garage.png", EWindow::window_game->terrain_atlas), 0.0f, 0.0f, 0.0f, 0.0f));
			
			*en->collision_size_left = 76.0f;
			*en->collision_size_right = 76.0f;

			*en->collision_size_up = 50.0f;
			*en->collision_size_down = 0.0f;

			*en->mass = 100000.0f;
			*en->life = 10000;

			put_entity_to_cluster(en);
			//entity_list.push_back(en);
			//entity_list.push_back(new Entity(300.0f, 150.0f));
			//entity_list.push_back(new Entity(400.0f, 150.0f));
		}

		for (int j = 0; j < 100; j++)
		for (int i = 0; i < 100; i++)
		if ((rand() % 10 == 0) && (!path_to_player_matrix->unwalk_matrix[j][i][0]))
		{
			Entity* en = new Entity(j * 50.0f + 25.0f, i * 50.0f);

				path_to_player_matrix->unwalk_matrix[j][i][0] = true;
				path_to_player_matrix->unwalk_matrix[j][i][1] = true;

			//*en->collision_size_right -= 10.0f * i;


			en->link_to_path_matrix = path_to_player_matrix;
			//en->AI_control = Entity::static_control_movable_enemy;
			en->sprite_list.push_back(new ESprite(ETextureAtlas::put_texture_to_atlas("data/block.png", EWindow::window_game->terrain_atlas), 0.0f, 0.0f, 0.0f, 0.0f));
			
			*en->collision_size_left = 26.0f;
			*en->collision_size_right = 26.0f;

			*en->collision_size_up = 50.0f;
			*en->collision_size_down = 0.0f;

			*en->mass = 100000.0f;
			*en->life = 10000;

			put_entity_to_cluster(en);
			//entity_list.push_back(en);
			//entity_list.push_back(new Entity(300.0f, 150.0f));
			//entity_list.push_back(new Entity(400.0f, 150.0f));
		}

		link_to_player = new Entity(rand() % 100 + 100.0f, rand() % 100 + 100.0f);
		link_to_player->sprite_list.push_back(new ESprite(ETextureAtlas::put_texture_to_atlas("data/sphere.png", EWindow::window_game->terrain_atlas), 0.0f, 0.0f, 0.0f, 0.0f));
		*link_to_player->mass = 1000.0f;
		link_to_player->link_to_path_matrix = path_to_player_matrix;
		link_to_player->AI_control = new AIControlPlayer();

		put_entity_to_cluster(link_to_player);

		std::cout << "game window created" << std::endl;

		EGraphicCore::gabarite_tileset = ETextureAtlas::put_texture_to_atlas("data/tileset.png", terrain_atlas);
		EGraphicCore::gabarite_white_pixel = ETextureAtlas::put_texture_to_atlas("data/white_pixel.png", terrain_atlas);


		EGraphicCore::gabarite_white_pixel->x += 1 / 4096.0f;
		EGraphicCore::gabarite_white_pixel->y += 1 / 4096.0f;

		EGraphicCore::gabarite_white_pixel->x2 -= 1 / 4096.0f;
		EGraphicCore::gabarite_white_pixel->y2 -= 1 / 4096.0f;
	}

	EWindowGame::~EWindowGame()
	{

	}
