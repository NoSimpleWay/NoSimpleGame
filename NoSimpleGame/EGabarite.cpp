#include "EGabarite.h"

EGabarite::EGabarite(std::string _name, float _x, float _y, float _size_x, float _size_y)
{
	x = _x;
	y = _y;

	uv_size_x = _size_x;
	uv_size_y = _size_y;

	x2 = _x + _size_x;
	y2 = _y + _size_y;

	size_x = uv_size_x * 4096.0f;
	size_y = uv_size_y * 4096.0f;
}

EGabarite::~EGabarite()
{
}
