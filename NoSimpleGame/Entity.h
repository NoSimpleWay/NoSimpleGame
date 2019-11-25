#pragma once
//#include "AIControlMovableEnemy.h"

class AIControl;
class AIControlMovableEnemy;
class EPathMatrix;
class Entity
{
public:
	float *position_x;
	float *position_y;

	float *speed_x;
	float *speed_y;

	//AIControl control;
	//AIControlMovableEnemy enemy_control;

	//AIControl current_control;

	static AIControlMovableEnemy* static_control_movable_enemy;
	AIControl* AI_control;

	EPathMatrix* link_to_path_matrix;

	Entity();
	Entity(float _x, float _y);
	~Entity();
};