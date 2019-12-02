#pragma once
//#include 
#include "EGraphicCore.h"
#include <iostream>

class ETextureAtlas
{
public:
	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;

	bool free_space[1024][1024];

	bool can_place_here(int _x, int _y, int _w, int _h);

	static EGabarite* put_texture_to_atlas(std::string _name, ETextureAtlas* _ta);

	ETextureAtlas();
	~ETextureAtlas();
};