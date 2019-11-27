#include "EPathMatrix.h"
#include <iostream>

EPathMatrix::EPathMatrix()
{
	for (int i = 0; i < PATH_MATRIX_ARRAY_SIZE; i++)
		for (int j = 0; j < PATH_MATRIX_ARRAY_SIZE; j++)
		{

			if (rand() % 10 == 0)
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
}

EPathMatrix::~EPathMatrix()
{
}

void EPathMatrix::default_update(float _d)
{
	//set target position
	if (heatmap_phase == Enums::HEATMAP_PHASE::UP)
	{
		path_matrix_point_start_x = path_position_x - PATH_MATRIX_CALCULATION_DISTANCE; if (path_matrix_point_start_x < 1) { path_matrix_point_start_x = 1; }
		path_matrix_point_end_x = path_position_x + PATH_MATRIX_CALCULATION_DISTANCE; if (path_matrix_point_end_x > PATH_MATRIX_ARRAY_SIZE - 2) { path_matrix_point_end_x = PATH_MATRIX_ARRAY_SIZE - 2; }

		path_matrix_point_start_y = path_position_y - PATH_MATRIX_CALCULATION_DISTANCE; if (path_matrix_point_start_y < 1) { path_matrix_point_start_y = 1; }
		path_matrix_point_end_y = path_position_y + PATH_MATRIX_CALCULATION_DISTANCE; if (path_matrix_point_end_y > PATH_MATRIX_ARRAY_SIZE - 2) { path_matrix_point_end_y = PATH_MATRIX_ARRAY_SIZE - 2; }

		path_matrix[path_position_x][path_position_y][back_path_buffer] = 5;
		path_matrix[path_position_x][path_position_y][current_path_buffer] = 5;
	}



	//
	if (heatmap_phase == Enums::HEATMAP_PHASE::UP)
		for (int i = path_matrix_point_start_y; i < path_matrix_point_end_y; i++)
			for (int j = path_matrix_point_start_x; j < path_matrix_point_end_x; j++)
				if ((!unwalk_matrix[j][i + 1][back_path_buffer]) && (!blocked_by_entity[j][i + 1][back_path_buffer]))
				{

					{
						if (path_matrix[j][i + 1][back_path_buffer] > path_matrix[j][i][back_path_buffer]) { path_matrix[j][i + 1][back_path_buffer] = path_matrix[j][i][back_path_buffer] + 1; }
					}
				}
	//
	if (heatmap_phase == Enums::HEATMAP_PHASE::DOWN)
		for (int i = path_matrix_point_end_y; i > path_matrix_point_start_y; i--)
		for (int j = path_matrix_point_start_x; j < path_matrix_point_end_x; j++)
			if ((!unwalk_matrix[j][i - 1][back_path_buffer]) && (!blocked_by_entity[j][i - 1][back_path_buffer]))
			{
					if (path_matrix[j][i - 1][back_path_buffer] > path_matrix[j][i][back_path_buffer]) { path_matrix[j][i - 1][back_path_buffer] = path_matrix[j][i][back_path_buffer] + 1; }
			}
	//
	if (heatmap_phase == Enums::HEATMAP_PHASE::RIGHT)
		for (int j = path_matrix_point_start_x; j < path_matrix_point_end_x; j++)
			for (int i = path_matrix_point_start_y; i < path_matrix_point_end_y; i++)
				if ((!unwalk_matrix[j + 1][i][back_path_buffer]) && (!blocked_by_entity[j + 1][i][back_path_buffer]))
				{

					{
						if (path_matrix[j + 1][i][back_path_buffer] > path_matrix[j][i][back_path_buffer]) { path_matrix[j + 1][i][back_path_buffer] = path_matrix[j][i][back_path_buffer] + 1; }
					}
				}



	//
	if (heatmap_phase == Enums::HEATMAP_PHASE::LEFT)
		for (int i = path_matrix_point_start_y; i < path_matrix_point_end_y; i++)
			for (int j = path_matrix_point_end_x; j > path_matrix_point_start_x; j--)
				if ((!unwalk_matrix[j - 1][i][back_path_buffer]) && (!blocked_by_entity[j - 1][i][back_path_buffer]))
				{

					{
						if (path_matrix[j - 1][i][back_path_buffer] > path_matrix[j][i][back_path_buffer]) { path_matrix[j - 1][i][back_path_buffer] = path_matrix[j][i][back_path_buffer] + 1; }
					}
				}

	//
	if (heatmap_phase == Enums::HEATMAP_PHASE::HEATING)
		for (int i = path_matrix_point_start_y; i < path_matrix_point_end_y; i++)
			for (int j = path_matrix_point_start_x; j < path_matrix_point_end_x; j++)
			{
				path_matrix[j][i][back_path_buffer] += 4;
				if (path_matrix[j][i][back_path_buffer] > 255) { path_matrix[j][i][back_path_buffer] = 255; }
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
