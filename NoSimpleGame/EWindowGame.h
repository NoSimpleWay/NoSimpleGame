#pragma once
#include "EWindow.h"
#include "Enums.h"
#include "Entity.h"
#include <vector>

class EWindowGame :	public EWindow
{
public:
	EWindowGame();
	~EWindowGame();

	short path_matrix[50][50][2];
	bool unwalk_matrix[50][50][2];
	bool blocked_by_entity[50][50][2];

	int heatmap_phase = Enums::HEATMAP_PHASE::UP;

	int path_position_x = 0;
	int path_position_y = 1;

	float character_position_x = 500.0f;
	float character_position_y = 500.0f;

	float path_calcalation_cooldown = 0.1f;

	int current_path_buffer = 0;
	int back_path_buffer = 0;

	//#include "Entity.h"
	std::vector<Entity*> entity_list;

	void default_update(float _d);
	virtual void update(float _d);

	void default_draw(float _d);
	virtual void draw(float _d);
};