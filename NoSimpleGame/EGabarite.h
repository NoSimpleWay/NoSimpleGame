#pragma once
#include <string>
class EGabarite
{
public:
	float x = 0;
	float y = 0;

	float x2 = 0;
	float y2 = 0;

	float size_x = 0;
	float size_y = 0;

	float uv_size_x = 0;
	float uv_size_y = 0;



	std::string name;

	EGabarite(std::string _name, float _x, float _y, float _size_x, float _size_y);
	~EGabarite();
};