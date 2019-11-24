#pragma once
#include "EWindowGame.h"
#include <iostream>



	EWindowGame::EWindowGame() : EWindow()
	{
		for (int i = 0; i < 50; i++)
		for (int j = 0; j < 50; j++)
		{
	
			if (rand() % 5 == 0)
			{
				unwalk_matrix[j][i][0] = true;
				unwalk_matrix[j][i][1] = true;
			}
			else
			{
				unwalk_matrix[j][i][0] = false;
				unwalk_matrix[j][i][1] = false;
			}

			path_matrix[j][i][0] = 99;
			path_matrix[j][i][1] = 99;

			blocked_by_entity[j][i][0] = false;
			blocked_by_entity[j][i][1] = false;
		}

		entity_list.push_back(new Entity(200.0f, 150.0f));
		entity_list.push_back(new Entity(300.0f, 150.0f));
		entity_list.push_back(new Entity(400.0f, 150.0f));

		std::cout << "game window created" << std::endl;
	}

	void EWindowGame::update(float _d)
	{
		path_calcalation_cooldown -= _d;

		if (path_calcalation_cooldown <= 0)
		{
			path_calcalation_cooldown += 0.02f;

			//heatmap phases calculation
			if (heatmap_phase == Enums::HEATMAP_PHASE::UP)
			{
				path_position_x = (int)(character_position_x / 50.0f);
				path_position_y = (int)(character_position_y / 50.0f);

				path_matrix[path_position_x][path_position_y][back_path_buffer] = 5;
				path_matrix[path_position_x][path_position_y][current_path_buffer] = 5;
			}

			for (Entity* e : entity_list)
			{
				blocked_by_entity[(int)(*e->position_x / 50.0f)][(int)(*e->position_y / 50.0f)][back_path_buffer] = true;
				blocked_by_entity[(int)(*e->position_x / 50.0f)][(int)(*e->position_y / 50.0f)][current_path_buffer] = true;
			}

			//
			if (heatmap_phase == Enums::HEATMAP_PHASE::UP)
				for (int i = 0; i < 20; i++)
					for (int j = 0; j < 20; j++)
						if ((!unwalk_matrix[j][i + 1][back_path_buffer]) && (!blocked_by_entity[j][i + 1][back_path_buffer]))
						{
							if (i < 19)
							{
								if (path_matrix[j][i + 1][back_path_buffer] > path_matrix[j][i][back_path_buffer]) { path_matrix[j][i + 1][back_path_buffer] = path_matrix[j][i][back_path_buffer] + 1; }
							}
						}

			//
			if (heatmap_phase == Enums::HEATMAP_PHASE::RIGHT)
				for (int j = 0; j < 20; j++)
					for (int i = 0; i < 20; i++)
						if ((!unwalk_matrix[j + 1][i][back_path_buffer]) && (!blocked_by_entity[j + 1][i][back_path_buffer]))
						{
							if (j < 19)
							{
								if (path_matrix[j + 1][i][back_path_buffer] > path_matrix[j][i][back_path_buffer]) { path_matrix[j + 1][i][back_path_buffer] = path_matrix[j][i][back_path_buffer] + 1; }
							}
						}

			//
			if (heatmap_phase == Enums::HEATMAP_PHASE::DOWN)
				for (int i = 19; i > 0; i--)
					for (int j = 0; j < 20; j++)
						if ((!unwalk_matrix[j][i - 1][back_path_buffer]) && (!blocked_by_entity[j][i - 1][back_path_buffer]))
						{
							if (i > 0)
							{
								if (path_matrix[j][i - 1][back_path_buffer] > path_matrix[j][i][back_path_buffer]) { path_matrix[j][i - 1][back_path_buffer] = path_matrix[j][i][back_path_buffer] + 1; }
							}
						}

			//
			if (heatmap_phase == Enums::HEATMAP_PHASE::LEFT)
				for (int j = 19; j > 0; j--)
					for (int i = 0; i < 20; i++)
						if ((!unwalk_matrix[j - 1][i][back_path_buffer]) && (!blocked_by_entity[j - 1][i][back_path_buffer]))
						{
							if (j > 0)
							{
								if (path_matrix[j - 1][i][back_path_buffer] > path_matrix[j][i][back_path_buffer]) { path_matrix[j - 1][i][back_path_buffer] = path_matrix[j][i][back_path_buffer] + 1; }
							}
						}

			//
			if (heatmap_phase == Enums::HEATMAP_PHASE::HEATING)
				for (int i = 0; i < 20; i++)
					for (int j = 0; j < 20; j++)
					{
						path_matrix[j][i][back_path_buffer] += 8;
						if (path_matrix[j][i][back_path_buffer] > 100) { path_matrix[j][i][back_path_buffer] = 100; }
					}

			heatmap_phase++;
			if (heatmap_phase > 4)
			{
				
				if (current_path_buffer == 0)
				{
					current_path_buffer = 1;
					back_path_buffer = 0;
				}
				else
				{
					current_path_buffer = 0;
					back_path_buffer = 1;
				}

				heatmap_phase = 0;
			}
		}
		

		

		for (Entity* e : entity_list)
		{
			blocked_by_entity[(int)(*e->position_x / 50.0f)][(int)(*e->position_y / 50.0f)][back_path_buffer] = false;
			blocked_by_entity[(int)(*e->position_x / 50.0f)][(int)(*e->position_y / 50.0f)][current_path_buffer] = false;
		}

		//entity move to path finding
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
				if ((path_matrix[rounded_pos_x][rounded_pos_y + 1][current_path_buffer] < min_direction) && (!unwalk_matrix[rounded_pos_x][rounded_pos_y + 1][current_path_buffer]))
				{
					min_direction = path_matrix[rounded_pos_x][rounded_pos_y + 1][current_path_buffer];
					selected_direction_x = 0;
					selected_direction_y = 1;

					//move up, correction right
					if ((*e->position_x < rounded_pos_x * 50.0f + 20.0f) && (unwalk_matrix[rounded_pos_x - 1][rounded_pos_y + 1][current_path_buffer]))
					{
						selected_direction_x = 1;
						selected_direction_y = 0;
					}

					if ((*e->position_x > rounded_pos_x * 50.0f + 30.0f) && (unwalk_matrix[rounded_pos_x + 1][rounded_pos_y + 1][current_path_buffer]))
					{
						selected_direction_x = -1;
						selected_direction_y = 0;
					}
				}

				if ((path_matrix[rounded_pos_x + 1][rounded_pos_y][current_path_buffer] < min_direction) && (!unwalk_matrix[rounded_pos_x + 1][rounded_pos_y][current_path_buffer]))
				{
					min_direction = path_matrix[rounded_pos_x + 1][rounded_pos_y][current_path_buffer];
					selected_direction_x = 1;
					selected_direction_y = 0;

					if ((*e->position_y < rounded_pos_y * 50.0f + 20.0f) && (unwalk_matrix[rounded_pos_x + 1][rounded_pos_y - 1][current_path_buffer]))
					{
						selected_direction_x = 0;
						selected_direction_y = 1;
					}

					if ((*e->position_y > rounded_pos_y * 50.0f + 30.0f) && (unwalk_matrix[rounded_pos_x + 1][rounded_pos_y + 1][current_path_buffer]))
					{
						selected_direction_x = 0;
						selected_direction_y = -1;
					}
				}

				if ((path_matrix[rounded_pos_x][rounded_pos_y - 1][current_path_buffer] < min_direction) && (!unwalk_matrix[rounded_pos_x][rounded_pos_y - 1][current_path_buffer]))
				{
					min_direction = path_matrix[rounded_pos_x][rounded_pos_y - 1][current_path_buffer];
					selected_direction_x = 0;
					selected_direction_y = -1;

					if ((*e->position_x < rounded_pos_x * 50.0f + 20.0f) && (unwalk_matrix[rounded_pos_x - 1][rounded_pos_y - 1][current_path_buffer]))
					{
						selected_direction_x = 1;
						selected_direction_y = 0;
					}

					if ((*e->position_x > rounded_pos_x * 50.0f + 30.0f) && (unwalk_matrix[rounded_pos_x + 1][rounded_pos_y - 1][current_path_buffer]))
					{
						selected_direction_x = -1;
						selected_direction_y = 0;
					}
				}

				if ((path_matrix[rounded_pos_x - 1][rounded_pos_y][current_path_buffer] < min_direction) && (!unwalk_matrix[rounded_pos_x - 1][rounded_pos_y][current_path_buffer]))
				{
					min_direction = path_matrix[rounded_pos_x - 1][rounded_pos_y][current_path_buffer];
					selected_direction_x = -1;
					selected_direction_y = 0;

					if ((*e->position_y < rounded_pos_y * 50.0f + 20.0f) && (unwalk_matrix[rounded_pos_x - 1][rounded_pos_y - 1][current_path_buffer]))
					{
						selected_direction_x = 0;
						selected_direction_y = 1;
					}

					if ((*e->position_y > rounded_pos_y * 50.0f + 30.0f) && (unwalk_matrix[rounded_pos_x - 1][rounded_pos_y + 1][current_path_buffer]))
					{
						selected_direction_x = 0;
						selected_direction_y = -1;
					}
				}
			}


			*e->position_x += 100.0f * _d * selected_direction_x;
			*e->position_y += 100.0f * _d * selected_direction_y;



			
		}

	}

	void EWindowGame::draw(float _d)
	{




		if (true)
		for (int i=0; i<20; i++)
		for (int j=0; j<20; j++)
		{

				//float col = 1.0f - path_matrix[j][i] / 10.0f;
				//EWindow::batch->setcolor(col, col, col, 1.0f);
				/**/

			float col = 1.0f - (path_matrix[j][i][current_path_buffer] - 0.0f) / 40.0f;
			batch->setcolor(col, col, col, 1.0f);


			EWindow::batch->draw_rect_with_uv(j * 50.0f + 1.0f, i * 50.0f + 1.0f, 48.0f, 48.0f, 0, 0, 1, 1);
		}

		EWindow::batch->setcolor(0.0f, 0.0f, 0.0f, 1.0f);
		EWindow::batch->draw_rect_with_uv(character_position_x - 12.5f, character_position_y - 12.5f, 25.0f, 25.0f, 0, 0, 1, 1);

		for (Entity* e : entity_list)
		{
			EWindow::batch->setcolor(0.0f, 1.0f, 1.0f, 1.0f);

			//int temp_x = *e->position_x;
			EWindow::batch->draw_rect_with_uv(*e->position_x - 12.5f, *e->position_y - 12.5f, 25.0f, 25.0f, 0, 0, 1, 1);
		}

		for (int j = 0; j < 20; j++)
		for (int i = 0; i < 20; i++)
		if (unwalk_matrix[j][i][current_path_buffer])
		{
			//if (unwalk_matrix[j][i][current_path_buffer]) { if (unwalk_matrix[j][i][current_path_buffer]) }
			//else { EWindow::batch->setcolor(EColor::COLOR_LIGHT_GRAY); }
			EWindow::batch->setcolor(EColor::COLOR_RED);
			//if (unwalk_matrix[j][i][current_path_buffer])

			EWindow::batch->draw_rect(j * 50.0f, i * 50.0f, 48.0f, 48.0f);
		}

	}

	EWindowGame::~EWindowGame()
	{

	}
