#include "Entity.h"
#include "AIControlMovableEnemy.h"

AIControlMovableEnemy* Entity::static_control_movable_enemy = new AIControlMovableEnemy();

Entity::Entity()
{
}

Entity::Entity(float _x, float _y)
{
	position_x = new float (_x);
	position_y = new float (_y);

	AI_control = static_control_movable_enemy;
	//current_control = control;
	//control = AIControlMovableEnemy;
	//*position_x = _x;
}

Entity::~Entity()
{
}
