#include "Batcher.h"

#include <glad/glad.h>
#include <iostream>

#include "EColor.h"
#include "EGabarite.h"

float vertices[80000];
unsigned int indices[100000];

unsigned int VBO, VAO, EBO;

unsigned int id = 0;
unsigned int indices_id = 0;
unsigned int indices_order = 0;

float batch_color_r;
float batch_color_g;
float batch_color_b;
float batch_color_a;



void Batcher::setcolor(float _r, float _g, float _b, float _a)
{
	batch_color_r = _r;
	batch_color_g = _g;
	batch_color_b = _b;

	batch_color_a = _a;
}



Batcher::Batcher()
{
	std::cout << "created" << std::endl;
}

void Batcher::init()
{
	std::cout << "initiate" << std::endl;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Batcher::draw_rect_position(float _x, float _y, float _x2, float _y2)
{
	//std::cout << "filled rect" << std::endl;
	//std::cout << "filled rect" << std::endl;

	//.#
	//..
	vertices[id + 0] = (_x2);
	vertices[id + 1] = (_y2);
	//vertices[id + 2] = 0;

	vertices[id + 2] = batch_color_r;
	vertices[id + 3] = batch_color_g;
	vertices[id + 4] = batch_color_b;
	vertices[id + 5] = batch_color_a;

	vertices[id + 6] = 1.0f;
	vertices[id + 7] = 1.0f;





	//..
	//.#
	vertices[id + 8] = (_x2);
	vertices[id + 9] = _y;
	//vertices[id + 10] = 0;

	vertices[id + 10] = batch_color_r;
	vertices[id + 11] = batch_color_g;
	vertices[id + 12] = batch_color_b;
	vertices[id + 13] = batch_color_a;

	vertices[id + 14] = 1.0f;
	vertices[id + 15] = 0.0f;




	//..
	//#.
	vertices[id + 16] = _x;
	vertices[id + 17] = _y;
	//vertices[id + 18] = 0;

	vertices[id + 18] = batch_color_r;
	vertices[id + 19] = batch_color_g;
	vertices[id + 20] = batch_color_b;
	vertices[id + 21] = batch_color_a;

	vertices[id + 22] = 0.0f;
	vertices[id + 23] = 0.0f;





	//#.
	//..
	vertices[id + 24] = _x;
	vertices[id + 25] = (_y2);
	//vertices[id + 26] = 0;

	vertices[id + 26] = batch_color_r;
	vertices[id + 27] = batch_color_g;
	vertices[id + 28] = batch_color_b;
	vertices[id + 29] = batch_color_a;

	vertices[id + 30] = 0.0f;
	vertices[id + 31] = 1.0f;

	id += 32;
}

void Batcher::draw_rect(float _x, float _y, float _w, float _h)
{
	//std::cout << "filled rect" << std::endl;
	//std::cout << "filled rect" << std::endl;

	//.#
	//..
	vertices[id + 0] = (_x + _w);
	vertices[id + 1] = (_y + _h);
	//vertices[id + 2] = 0;

	vertices[id + 2] = batch_color_r;
	vertices[id + 3] = batch_color_g;
	vertices[id + 4] = batch_color_b;
	vertices[id + 5] = batch_color_a;

	vertices[id + 6] = 1.0f;
	vertices[id + 7] = 1.0f;





	//..
	//.#
	vertices[id + 8] = (_x + _w);
	vertices[id + 9] = _y;
	//vertices[id + 10] = 0;

	vertices[id + 10] = batch_color_r;
	vertices[id + 11] = batch_color_g;
	vertices[id + 12] = batch_color_b;
	vertices[id + 13] = batch_color_a;

	vertices[id + 14] = 1.0f;
	vertices[id + 15] = 0.0f;




	//..
	//#.
	vertices[id + 16] = _x;
	vertices[id + 17] = _y;
	//vertices[id + 18] = 0;

	vertices[id + 18] = batch_color_r;
	vertices[id + 19] = batch_color_g;
	vertices[id + 20] = batch_color_b;
	vertices[id + 21] = batch_color_a;

	vertices[id + 22] = 0.0f;
	vertices[id + 23] = 0.0f;





	//#.
	//..
	vertices[id + 24] = _x;
	vertices[id + 25] = (_y + _h);
	//vertices[id + 26] = 0;

	vertices[id + 26] = batch_color_r;
	vertices[id + 27] = batch_color_g;
	vertices[id + 28] = batch_color_b;
	vertices[id + 29] = batch_color_a;

	vertices[id + 30] = 0.0f;
	vertices[id + 31] = 1.0f;

	id += 32;

	if (id > batch_force_draw_call)
	{
		reinit();
		draw_call();
		reset();
	}
}

void Batcher::draw_rect_without_color(float _x, float _y, float _w, float _h, int _tx, int _ty)
{
	//std::cout << "filled rect" << std::endl;

	vertices[id + 0] = _x + _w;
	vertices[id + 1] = _y + _h;
	vertices[id + 2] = 0;

	vertices[id + 3] = 0.25f + 0.25f * _tx;
	vertices[id + 4] = 0.20f + 0.20f * _ty;






	vertices[id + 5] = _x + _w;
	vertices[id + 6] = _y - _h;
	vertices[id + 7] = 0;

	vertices[id + 8] = 0.25f + 0.25f * _tx;
	vertices[id + 9] = 0.0f + 0.20f * _ty;






	vertices[id + 10] = _x - _w;
	vertices[id + 11] = _y - _h;
	vertices[id + 12] = 0;

	vertices[id + 13] = 0.0f + 0.25f * _tx;
	vertices[id + 14] = 0.0f + 0.20f * _ty;






	vertices[id + 15] = _x - _w;
	vertices[id + 16] = _y + _h;
	vertices[id + 17] = 0;

	vertices[id + 18] = 0.0f + 0.25f * _tx;
	vertices[id + 19] = 0.20f + 0.20f * _ty;

	id += 20;
}

void Batcher::draw_rect_with_uv(float _x, float _y, float _w, float _h, float _start_x, float _start_y, float _end_x, float _end_y)
{
	//std::cout << "filled rect" << std::endl;

	//.#
	//..
	vertices[id + 0] = (_x + _w);
	vertices[id + 1] = (_y + _h);
	//vertices[id + 2] = 0;

	vertices[id + 2] = batch_color_r;
	vertices[id + 3] = batch_color_g;
	vertices[id + 4] = batch_color_b;
	vertices[id + 5] = batch_color_a;

	vertices[id + 6] = _end_x;
	vertices[id + 7] = _end_y;





	//..
	//.#
	vertices[id + 8] = (_x + _w);
	vertices[id + 9] = _y;
	//vertices[id + 10] = 0;

	vertices[id + 10] = batch_color_r;
	vertices[id + 11] = batch_color_g;
	vertices[id + 12] = batch_color_b;
	vertices[id + 13] = batch_color_a;

	vertices[id + 14] = _end_x;
	vertices[id + 15] = _start_y;




	//..
	//#.
	vertices[id + 16] = _x;
	vertices[id + 17] = _y;
	//vertices[id + 18] = 0;

	vertices[id + 18] = batch_color_r;
	vertices[id + 19] = batch_color_g;
	vertices[id + 20] = batch_color_b;
	vertices[id + 21] = batch_color_a;

	vertices[id + 22] = _start_x;
	vertices[id + 23] = _start_y;





	//#.
	//..
	vertices[id + 24] = _x;
	vertices[id + 25] = (_y + _h);
	//vertices[id + 26] = 0;

	vertices[id + 26] = batch_color_r;
	vertices[id + 27] = batch_color_g;
	vertices[id + 28] = batch_color_b;
	vertices[id + 29] = batch_color_a;

	vertices[id + 30] = _start_x;
	vertices[id + 31] = _end_y;

	id += 32;

	if (id > batch_force_draw_call)
	{
		reinit();
		draw_call();
		reset();
	}
}

void Batcher::draw_rect_with_uv(float _x, float _y, float _w, float _h, EGabarite* _g)
{
	//std::cout << "filled rect" << std::endl;

	//.#
	//..
	vertices[id + 0] = (_x + _w);
	vertices[id + 1] = (_y + _h);
	//vertices[id + 2] = 0;

	vertices[id + 2] = batch_color_r;
	vertices[id + 3] = batch_color_g;
	vertices[id + 4] = batch_color_b;
	vertices[id + 5] = batch_color_a;

	vertices[id + 6] = _g->x2;
	vertices[id + 7] = _g->y2;





	//..
	//.#
	vertices[id + 8] = (_x + _w);
	vertices[id + 9] = _y;
	//vertices[id + 10] = 0;

	vertices[id + 10] = batch_color_r;
	vertices[id + 11] = batch_color_g;
	vertices[id + 12] = batch_color_b;
	vertices[id + 13] = batch_color_a;

	vertices[id + 14] = _g->x2;
	vertices[id + 15] = _g->y;




	//..
	//#.
	vertices[id + 16] = _x;
	vertices[id + 17] = _y;
	//vertices[id + 18] = 0;

	vertices[id + 18] = batch_color_r;
	vertices[id + 19] = batch_color_g;
	vertices[id + 20] = batch_color_b;
	vertices[id + 21] = batch_color_a;

	vertices[id + 22] = _g->x;
	vertices[id + 23] = _g->y;





	//#.
	//..
	vertices[id + 24] = _x;
	vertices[id + 25] = (_y + _h);
	//vertices[id + 26] = 0;

	vertices[id + 26] = batch_color_r;
	vertices[id + 27] = batch_color_g;
	vertices[id + 28] = batch_color_b;
	vertices[id + 29] = batch_color_a;

	vertices[id + 30] = _g->x;
	vertices[id + 31] = _g->y2;

	id += 32;

	if (id > batch_force_draw_call)
	{
		reinit();
		draw_call();
		reset();
	}
}

void Batcher::fill_indices()
{
	indices[indices_id + 0] = indices_order + 0;
	indices[indices_id + 1] = indices_order + 1;
	indices[indices_id + 2] = indices_order + 3;

	indices[indices_id + 3] = indices_order + 1;
	indices[indices_id + 4] = indices_order + 2;
	indices[indices_id + 5] = indices_order + 3;


	indices_id += 6;
	indices_order += 4;
}

void Batcher::setcolor_255(int _r, int _g, int _b, int _a)
{
	setcolor(_r / 255.0f, _g / 255.0f, _b / 255.0f, _a / 100.0f);
}




Batcher::~Batcher()
{
}

void Batcher::reinit()
{
	glBindBuffer(GL_ARRAY_BUFFER, VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
}

void Batcher::draw_call()
{
	//reinit();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6 * (id / 32), GL_UNSIGNED_INT, 0);
	//std::cout << blue << "vertices: " << red << id << std::endl<<white;
}

void Batcher::reset()
{
	id = 0;
	indices_id = 0;
	indices_order = 0;

}

void Batcher::setcolor(EColor* _color)
{
	batch_color_r = _color->red;
	batch_color_g = _color->green;
	batch_color_b = _color->blue;
	batch_color_a = _color->alpha;
}

void Batcher::setcolor(EColorCollection* _color)
{
	batch_color_r = _color->color_red;
	batch_color_g = _color->color_green;
	batch_color_b = _color->color_blue;
	batch_color_a = _color->color_alpha;
}

void Batcher::setcolor_lum(EColorCollection* _color, float _lum)
{
	batch_color_r = _color->color_red * _lum;
	batch_color_g = _color->color_green * _lum;
	batch_color_b = _color->color_blue * _lum;
	batch_color_a = _color->color_alpha;
}

void Batcher::setcolor_alpha(EColorCollection* _color, float _a)
{
	batch_color_r = _color->color_red;
	batch_color_g = _color->color_green;
	batch_color_b = _color->color_blue;
	batch_color_a = _a;
}

void Batcher::draw_rama(float _x, float _y, float _sx, float _sy, float _t, EGabarite* _g)
{
	draw_rect_with_uv(_x - _t, _y - _t, _t, _sy + _t * 2.0f, _g);//left
	draw_rect_with_uv(_x + _sx, _y - _t, _t, _sy + _t * 2.0f, _g);//right

	draw_rect_with_uv(_x - _t, _y + _sy, _sx + _t * 2.0f, _t, _g);//up
	draw_rect_with_uv(_x - _t, _y - _t, _sx + _t * 2.0f, _t, _g);//up
}

void Batcher::draw_rect_gabarite_custom_uv(float _x, float _y, float _w, float _h, EGabarite* _g, float _sx, float _sy, float _ex, float _ey)
{
	//.#
	//..
	vertices[id + 0] = (_x + _w);
	vertices[id + 1] = (_y + _h);
	//vertices[id + 2] = 0;

	vertices[id + 2] = batch_color_r;
	vertices[id + 3] = batch_color_g;
	vertices[id + 4] = batch_color_b;
	vertices[id + 5] = batch_color_a;

	vertices[id + 6] = _g->x + _ex / 4096.0f;
	vertices[id + 7] = _g->y + _ey / 4096.0f;





	//..
	//.#
	vertices[id + 8] = (_x + _w);
	vertices[id + 9] = _y;
	//vertices[id + 10] = 0;

	vertices[id + 10] = batch_color_r;
	vertices[id + 11] = batch_color_g;
	vertices[id + 12] = batch_color_b;
	vertices[id + 13] = batch_color_a;

	vertices[id + 14] = _g->x + _ex / 4096.0f;
	vertices[id + 15] = _g->y + _sy / 4096.0f;




	//..
	//#.
	vertices[id + 16] = _x;
	vertices[id + 17] = _y;
	//vertices[id + 18] = 0;

	vertices[id + 18] = batch_color_r;
	vertices[id + 19] = batch_color_g;
	vertices[id + 20] = batch_color_b;
	vertices[id + 21] = batch_color_a;

	vertices[id + 22] = _g->x + _sx / 4096.0f;
	vertices[id + 23] = _g->y + _sy / 4096.0f;





	//#.
	//..
	vertices[id + 24] = _x;
	vertices[id + 25] = (_y + _h);
	//vertices[id + 26] = 0;

	vertices[id + 26] = batch_color_r;
	vertices[id + 27] = batch_color_g;
	vertices[id + 28] = batch_color_b;
	vertices[id + 29] = batch_color_a;

	vertices[id + 30] = _g->x + _sx / 4096.0f;
	vertices[id + 31] = _g->y + _ey / 4096.0f;

	id += 32;

	if (id > batch_force_draw_call)
	{
		reinit();
		draw_call();
		reset();
	}
}

void Batcher::draw_simple_rect(float _x, float _y, float _size_x, float _size_y)
{
	//draw_rect_with_uv(_x, _y, _size_x, _size_y, DefaultGabarite::gabarite_white);
}
