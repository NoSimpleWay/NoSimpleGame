#include "EColor.h"

EColor::EColor(float _r, float _g, float _b, float _a)
{
}

EColor::~EColor()
{
}

void EColor::set_color(float _r, float _g, float _b, float _a)
{
	red = _r;
	green = _g;
	blue = _b;
	alpha = _a;
}
