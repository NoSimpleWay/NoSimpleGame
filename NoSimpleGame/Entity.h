#pragma once
class Entity
{
public:
	float *position_x;
	float *position_y;

	float *speed_x;
	float *speed_y;

	Entity();
	Entity(float _x, float _y);
	~Entity();
};