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

	float* acceleration = new float(200.0f);
	float* friction = new float(0.1f);

	float* step_cooldown = new float(0.0f);

	/////////////////
	float* life = new float(100.0f);

	/////////////////
	float* bullet_damage = new float(10.0f);


	float* sprite_size_x = new float(25.0f);
	float* sprite_size_y = new float(25.0f);

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

	void default_collision_action(Entity* _a, Entity* _b, int _side);

	bool *is_bullet = new bool(false);
	bool *is_dead = new bool(false);

	int prev_cluster_position_x;
	int prev_cluster_position_y;

	int new_cluster_position_x;
	int new_cluster_position_y;

	bool* already_updated = new (bool){ false };

	float* mass = new float (100.0f);

	float* next_x = new float(0.0f);
	float* next_y = new float(0.0f);

	float *collision_size_up =			new float(13.0f);
	float *collision_size_right =		new float(13.0f);
	float *collision_size_down =		new float(13.0f);
	float *collision_size_left =		new float (13.0f);

	float bri;



	Entity();
	Entity(float _x, float _y);
	~Entity();
};