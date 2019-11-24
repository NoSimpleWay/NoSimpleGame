#include "EColor.h"

EColorCollection* EColor::COLOR_RED					= new EColorCollection(1.0f,	0.0f,	0.0f,	1.0f);
EColorCollection* EColor::COLOR_GREEN				= new EColorCollection(0.0f,	1.0f,	0.0f,	1.0f);
EColorCollection* EColor::COLOR_BLUE				= new EColorCollection(0.0f,	0.0f,	1.0f,	1.0f);

EColorCollection* EColor::COLOR_WHITE				= new EColorCollection(1.0f,	1.0f,	1.0f,	1.0f);
EColorCollection* EColor::COLOR_BLACK				= new EColorCollection(0.0f,	0.0f,	0.0f,	1.0f);

EColorCollection* EColor::COLOR_LIGHT_GRAY			= new EColorCollection(0.75f,	0.75f,	0.75f,	1.0f);
EColorCollection* EColor::COLOR_GRAY				= new EColorCollection(0.5f,	0.5f,	0.5f,	1.0f);
EColorCollection* EColor::COLOR_DARK_GRAY			= new EColorCollection(0.25f,	0.25f,	0.25f,	1.0f);

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
