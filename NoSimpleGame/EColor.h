#pragma once
#include "EColorCollection.h"

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

	static EColorCollection* COLOR_RED;
	static EColorCollection* COLOR_GREEN;
	static EColorCollection* COLOR_BLUE;

	static EColorCollection* COLOR_WHITE;
	static EColorCollection* COLOR_BLACK;

	static EColorCollection* COLOR_LIGHT_GRAY;
	static EColorCollection* COLOR_GRAY;
	static EColorCollection* COLOR_DARK_GRAY;


};