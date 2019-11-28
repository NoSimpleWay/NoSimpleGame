#pragma once
#include "EWindow.h"
#include "Enums.h"
#include <vector>

#include "AIControl.h"
#include "AIControlMovableEnemy.h"
#include "AIControlPlayer.h"

#include "EPathMatrix.h"
#include "ECluster.h"


const int CLUSTER_SIZE = 500;

class EWindowGame :	public EWindow
{
public:
	EWindowGame();
	~EWindowGame();

	//x = 1;

	//const int PMAZ = 100;

	EPathMatrix* path_to_player_matrix;

	float character_position_x = 500.0f;
	float character_position_y = 500.0f;

	int cluster_calculator_left_border = 0;
	int cluster_calculator_right_border = 0;

	int cluster_calculator_up_border = 0;
	int cluster_calculator_down_border = 0;



	float camera_x = 0;
	float camera_y = 0;

	float shoot_cooldown = 0.5f;

	ECluster* cluster[20][20];

	//#include "Entity.h"
	//std::vector<Entity*> entity_list;

	Entity* link_to_player;

	void default_update(float _d);
	virtual void update(float _d);

	void default_draw(float _d);
	virtual void draw(float _d);

	void put_entity_to_cluster(Entity* _e);

	int check_collision(Entity* _a, Entity* _b);
};