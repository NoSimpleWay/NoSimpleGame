#include "Entity.h"
#include "AIControlMovableEnemy.h"
//#include "" 23:24:14 25.11.2019
#include <iostream>
#include "EWindowGame.h"

AIControlMovableEnemy* Entity::static_control_movable_enemy = new AIControlMovableEnemy();

void Entity::default_update(float _d)
{
	

	


	

	
}

void Entity::update(float _d)
{
}

void Entity::move(float _d)
{
	prev_cluster_position_x = (int)(*position_x / CLUSTER_SIZE);
	prev_cluster_position_y = (int)(*position_y / CLUSTER_SIZE);

	*position_x += *speed_x;
	*position_y += *speed_y;


	new_cluster_position_x = (int)(*position_x / CLUSTER_SIZE);
	new_cluster_position_y = (int)(*position_y / CLUSTER_SIZE);

	//if (*speed_x * *speed_x < 0.01f) { *speed_x = 0; }
	//if (*speed_y * *speed_y < 0.01f) { *speed_y = 0; }




}

void Entity::move_to(float _x, float _y)
{
	prev_cluster_position_x = (int)(*position_x / CLUSTER_SIZE);
	prev_cluster_position_y = (int)(*position_y / CLUSTER_SIZE);

	*position_x = _x;
	*position_y = _y;

	new_cluster_position_x = (int)(*position_x / CLUSTER_SIZE);
	new_cluster_position_y = (int)(*position_y / CLUSTER_SIZE);
}

void Entity::move_relative(float _x, float _y)
{
	prev_cluster_position_x = (int)(*position_x / CLUSTER_SIZE);
	prev_cluster_position_y = (int)(*position_y / CLUSTER_SIZE);

	*position_x += _x;
	*position_y += _y;

	new_cluster_position_x = (int)(*position_x / CLUSTER_SIZE);
	new_cluster_position_y = (int)(*position_y / CLUSTER_SIZE);
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

	
	//current_control = control;
	//control = AIControlMovableEnemy;
	//*position_x = _x;
}

Entity::~Entity()
{
}
