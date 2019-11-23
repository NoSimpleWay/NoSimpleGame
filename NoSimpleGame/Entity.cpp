#include "Entity.h"

Entity::Entity()
{
}

Entity::Entity(float _x, float _y)
{
	position_x = new float (_x);
	position_y = new float (_y);

	//*position_x = _x;
}

Entity::~Entity()
{
}
