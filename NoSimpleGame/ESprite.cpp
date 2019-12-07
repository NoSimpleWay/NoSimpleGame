#include "ESprite.h"

ESprite::ESprite(EGabarite* _g, float _x, float _y, float _size_x, float _size_y)
{
	gabarite = _g;

	*sprite_x = _x;
	*sprite_y = _y;

	*sprite_size_x = _size_x;
	*sprite_size_y = _size_y;
	
}

ESprite::~ESprite()
{
}
