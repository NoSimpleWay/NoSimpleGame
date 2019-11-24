#pragma once
class EColorCollection
{
public:
	float color_red = 1.0f;
	float color_green = 1.0f;
	float color_blue = 1.0f;
	float color_alpha = 1.0f;

	EColorCollection();
	EColorCollection(float _r, float _g, float _b, float _a);
	~EColorCollection();

};