#pragma once
#include "Enums.h"

const int PATH_MATRIX_ARRAY_SIZE = 100;
const float PATH_CELL_SIZE = 50;
const float PATH_MATRIX_CALCULATION_DISTANCE = 15;

class EPathMatrix
{
public:
	short path_matrix[PATH_MATRIX_ARRAY_SIZE][PATH_MATRIX_ARRAY_SIZE][2];
	bool unwalk_matrix[PATH_MATRIX_ARRAY_SIZE][PATH_MATRIX_ARRAY_SIZE][2];
	bool blocked_by_entity[PATH_MATRIX_ARRAY_SIZE][PATH_MATRIX_ARRAY_SIZE][2];

	int path_matrix_point_start_x;
	int path_matrix_point_end_x;

	int path_matrix_point_start_y;
	int path_matrix_point_end_y;

	int heatmap_phase = Enums::HEATMAP_PHASE::PHASE_UP;
	float path_calculation_cooldown = 0.1f;

	int path_position_x = 0;
	int path_position_y = 1;
	int current_path_buffer = 0;
	int back_path_buffer = 0;

	EPathMatrix();
	~EPathMatrix();

	void default_update(float _d);
};