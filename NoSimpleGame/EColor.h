#pragma once

class EColor
{
public:

	float red;
	float green;
	float blue;
	float alpha;

	EColor(float _r, float _g, float _b, float _a);
	~EColor();

	void set_color(float _r, float _g, float _b, float _a);
};