#pragma once
//#include "AIControlMovableEnemy.h"

class AIControl;
class AIControlMovableEnemy;
class AIControlPlayer;

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

	void default_update(float _d);
	virtual void update(float _d);
	void move(float _d);
	void move_to(float _x, float _y);
	void move_relative(float _x, float _y);

	int prev_cluster_position_x;
	int prev_cluster_position_y;

	int new_cluster_position_x;
	int new_cluster_position_y;

	bool* already_updated = new (bool){ false };

	float* mass = new float (100.0f);

	float *collision_size_up =			new float(13.0f);
	float *collision_size_right =		new float(13.0f);
	float *collision_size_down =		new float(13.0f);
	float *collision_size_left =		new float (13.0f);

	float bri;



	Entity();
	Entity(float _x, float _y);
	~Entity();
};