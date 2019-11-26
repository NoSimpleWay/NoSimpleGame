#include "Entity.h"
#include "AIControlMovableEnemy.h"
//#include "" 23:24:14 25.11.2019
#include <iostream>
#include "EWindowGame.h"

AIControlMovableEnemy* Entity::static_control_movable_enemy = new AIControlMovableEnemy();

void Entity::default_update(float _d)
{
	prev_cluster_position_x = (int)(*position_x / CLUSTER_SIZE);
	prev_cluster_position_y = (int)(*position_y / CLUSTER_SIZE);

	*position_x += *speed_x;
	*position_y += *speed_y;

	float friction_multiplier = pow(0.05f, _d);

	*speed_x *= friction_multiplier;
	*speed_y *= friction_multiplier;

	new_cluster_position_x = (int)(*position_x / CLUSTER_SIZE);
	new_cluster_position_y = (int)(*position_y / CLUSTER_SIZE);

	

	
}

void Entity::update(float _d)
{
}

Entity::Entity()
{
}

Entity::Entity(float _x, float _y)
{
	position_x = new float (_x);
	position_y = new float (_y);

	speed_x = new float(0);
	speed_y = new float(0);

	AI_control = static_control_movable_enemy;
	//current_control = control;
	//control = AIControlMovableEnemy;
	//*position_x = _x;
}

Entity::~Entity()
{
}
