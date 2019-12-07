#pragma once
#include "EGabarite.h"

class ESprite
{
public:
	EGabarite* gabarite;

	float* sprite_x = new float(-0.0f);
	float* sprite_size_x = new float(40.0f);

	float* sprite_y = new float(0.0f);
	float* sprite_size_y = new float(40.0f);


	ESprite(EGabarite* _g, float _left, float _right, float _up, float _down);
	~ESprite();

};