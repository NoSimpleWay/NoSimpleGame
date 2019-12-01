#include "AIControlMovableEnemy.h"
#include <iostream>
#include "EWindow.h"
#include "EWindowGame.h"
#include "EPathMatrix.h"
#include "Entity.h"

void AIControlMovableEnemy::acceleration_control(Entity* _e, float _d)
{
	//std::cout << "@@@" << std::endl;

	int selected_direction_x = 0;
	int selected_direction_y = 0;

	int min_direction = 99;

	int rounded_pos_x = (int)(*_e->position_x / PATH_CELL_SIZE);
	int rounded_pos_y = (int)(*_e->position_y / PATH_CELL_SIZE);

	float dst_x = *_e->position_x - EWindow::window_game->character_position_x;
	float dst_y = *_e->position_y - EWindow::window_game->character_position_y;
	float dst_total = dst_x * dst_x + dst_y * dst_y;

	
	if (dst_total >= 3200)
	{
		if ((_e->link_to_path_matrix->path_matrix[rounded_pos_x][rounded_pos_y + 1][_e->link_to_path_matrix->current_path_buffer] < min_direction) && (!_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x][rounded_pos_y + 1][_e->link_to_path_matrix->current_path_buffer]))
		{
			min_direction = _e->link_to_path_matrix->path_matrix[rounded_pos_x][rounded_pos_y + 1][_e->link_to_path_matrix->current_path_buffer];
			selected_direction_x = 0;
			selected_direction_y = 1;

			//move up, correction right
			if ((*_e->position_x < rounded_pos_x * PATH_CELL_SIZE + 20.0f) && (_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x - 1][rounded_pos_y + 1][_e->link_to_path_matrix->current_path_buffer]))
			{
				selected_direction_x = 1;
				selected_direction_y = 0;
			}

			if ((*_e->position_x > rounded_pos_x* PATH_CELL_SIZE + 30.0f) && (_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x + 1][rounded_pos_y + 1][_e->link_to_path_matrix->current_path_buffer]))
			{
				selected_direction_x = -1;
				selected_direction_y = 0;
			}
		}

		if ((_e->link_to_path_matrix->path_matrix[rounded_pos_x + 1][rounded_pos_y][_e->link_to_path_matrix->current_path_buffer] < min_direction) && (!_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x + 1][rounded_pos_y][_e->link_to_path_matrix->current_path_buffer]))
		{
			min_direction = _e->link_to_path_matrix->path_matrix[rounded_pos_x + 1][rounded_pos_y][_e->link_to_path_matrix->current_path_buffer];
			selected_direction_x = 1;
			selected_direction_y = 0;

			if ((*_e->position_y < rounded_pos_y * PATH_CELL_SIZE + 20.0f) && (_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x + 1][rounded_pos_y - 1][_e->link_to_path_matrix->current_path_buffer]))
			{
				selected_direction_x = 0;
				selected_direction_y = 1;
			}

			if ((*_e->position_y > rounded_pos_y* PATH_CELL_SIZE + 30.0f) && (_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x + 1][rounded_pos_y + 1][_e->link_to_path_matrix->current_path_buffer]))
			{
				selected_direction_x = 0;
				selected_direction_y = -1;
			}
		}

		if ((_e->link_to_path_matrix->path_matrix[rounded_pos_x][rounded_pos_y - 1][_e->link_to_path_matrix->current_path_buffer] < min_direction) && (!_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x][rounded_pos_y - 1][_e->link_to_path_matrix->current_path_buffer]))
		{
			min_direction = _e->link_to_path_matrix->path_matrix[rounded_pos_x][rounded_pos_y - 1][_e->link_to_path_matrix->current_path_buffer];
			selected_direction_x = 0;
			selected_direction_y = -1;

			if ((*_e->position_x < rounded_pos_x * PATH_CELL_SIZE + 20.0f) && (_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x - 1][rounded_pos_y - 1][_e->link_to_path_matrix->current_path_buffer]))
			{
				selected_direction_x = 1;
				selected_direction_y = 0;
			}

			if ((*_e->position_x > rounded_pos_x* PATH_CELL_SIZE + 30.0f) && (_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x + 1][rounded_pos_y - 1][_e->link_to_path_matrix->current_path_buffer]))
			{
				selected_direction_x = -1;
				selected_direction_y = 0;
			}
		}

		if ((_e->link_to_path_matrix->path_matrix[rounded_pos_x - 1][rounded_pos_y][_e->link_to_path_matrix->current_path_buffer] < min_direction) && (!_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x - 1][rounded_pos_y][_e->link_to_path_matrix->current_path_buffer]))
		{
			min_direction = _e->link_to_path_matrix->path_matrix[rounded_pos_x - 1][rounded_pos_y][_e->link_to_path_matrix->current_path_buffer];
			selected_direction_x = -1;
			selected_direction_y = 0;

			if ((*_e->position_y < rounded_pos_y * PATH_CELL_SIZE + 20.0f) && (_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x - 1][rounded_pos_y - 1][_e->link_to_path_matrix->current_path_buffer]))
			{
				selected_direction_x = 0;
				selected_direction_y = 1;
			}

			if ((*_e->position_y > rounded_pos_y* PATH_CELL_SIZE + 30.0f) && (_e->link_to_path_matrix->unwalk_matrix[rounded_pos_x - 1][rounded_pos_y + 1][_e->link_to_path_matrix->current_path_buffer]))
			{
				selected_direction_x = 0;
				selected_direction_y = -1;
			}
		}
	}


	*_e->speed_x += *_e->acceleration * selected_direction_x * _d;
	*_e->speed_y += *_e->acceleration * selected_direction_y * _d;

	

}

AIControlMovableEnemy::AIControlMovableEnemy() : AIControl()
{
}

AIControlMovableEnemy::~AIControlMovableEnemy()
{
}
