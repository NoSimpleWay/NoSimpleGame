#include "EFont.h"
#include <string>

#include <iostream>
#include "Batcher.h"
#include <fstream>
#include <math.h>

using namespace std;

vector<EFont*> EFont::font_list;

EFont* EFont::active_font = NULL;

bool comma_is_open = true;
int font_array_id = 31;



bool show_console_info = false;

//EGabarite* EFont::gabarite;

EFont::EFont()
{
}

EFont::EFont(string _name, EGabarite* _g, bool _not_cyrrilic)
{
	gabarite = _g;
	load_font_littera(_name);

	name = _name;

	is_not_cyrrilic = _not_cyrrilic;
}

EFont::~EFont()
{
}

/*
int get_size()
{

}
*/
float EFont::get_float_from_sub_data(string _s)
{
	string get_data = "";

	comma_is_open = false;

	//cout << "parse sub_data:" << _s << endl;

	for (int j = 0; j < _s.length(); j++)
	{

		if (_s.at(j) == '"')
		{
			if (!comma_is_open)
			{
				comma_is_open = true;
			}
			else
			{
				//cout << "try return:" << get_data << endl;
				return std::stof(get_data);
			}
		}
		else
		{
			if (comma_is_open) { get_data += _s.at(j); }
		}
	}
}

int EFont::get_int_from_sub_data(string _s)
{
	string get_data = "";

	comma_is_open = false;

	//cout << "parse sub_data:" << _s << endl;

	for (int j = 0; j < _s.length(); j++)
	{

		if (_s.at(j) == '"')
		{
			if (!comma_is_open)
			{
				comma_is_open = true;
			}
			else
			{
				//cout << "try return:" << get_data << endl;
				return std::stoi(get_data);
			}
		}
		else
		{
			if (comma_is_open) { get_data += _s.at(j); }
		}
	}
}

bool compare_with_key(string _data, string _key)
{
	if (_data.substr(0, _key.length()) == _key)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void EFont::final_draw(Batcher* _batcher, string _s, float _x, float _y)
{

	if (align_x == Enums::PositionMode::MID) { _x -= get_width(this, _s) / 2.0f; }
	if (align_x == Enums::PositionMode::RIGHT) { _x -= get_width(this, _s); }

	_x = round(_x);

	for (int sym = 0; sym < _s.length(); sym++)
	{

		float a = 0.5f;
		int target_symbol = (int)_s.at(sym);
		if (target_symbol < 0) { target_symbol += 256; }

		if ((target_symbol == '\\') && (sym + 1 < _s.length()) && (_s.at(sym + 1) == 'n'))
		{
			//_s += " " + std::to_string(x_adding);

			x_adding = 0.0f;
			y_adding += 17.0f;

			sym += 1;


		}
		else
		{
			_batcher->draw_with_custom_uv
			(
				_x + x_adding + offset_x[target_symbol] * scale,
				_y - (real_size_y[target_symbol] - 15.0f + offset_y[target_symbol]) * scale - y_adding,

				real_size_x[target_symbol] * scale,
				real_size_y[target_symbol] * scale,

				x[target_symbol],
				y[target_symbol] - size_y[target_symbol],

				x[target_symbol] + size_x[target_symbol],
				y[target_symbol]
			);

			x_adding += (advance[target_symbol]) * scale;
			//y_adding += 17.0 * scale;
		}


	}

	if (align_only_once)
	{
		align_only_once = false;
		align_x = align_prev;
	}
}
void EFont::draw(Batcher* _batcher, string _s, float _x, float _y)
{
	x_adding = 0.0f;
	y_adding = 0.0f;
	final_draw(_batcher, _s, _x, _y);
}

void EFont::add_draw(Batcher* _batcher, string _s, float _x, float _y)
{
	if (align_x == Enums::PositionMode::MID) { x_adding /= 2.0f; }
	final_draw(_batcher, _s, _x, _y);
}

void EFont::load_font(string _path)
{
	string line;
	string sub_data;
	int data_order;

	ifstream myfilez("data/font_arial.txt");

	for (int i = 0; i < 300; i++)
	{
		x[i] = 0;
		y[i] = 0;
		size_x[i] = 0;
		size_y[i] = 0;
		offset_x[i] = 0;
		offset_y[i] = 0;
	}

	if (myfilez.is_open())
	{
		font_array_id = 31;

		while ((getline(myfilez, line)) && (font_array_id < 300))
		{
			comma_is_open = true;
			data_order = 0;
			//cout << line << '\n';

			//cout << "line #" << font_array_id;

			if (font_array_id > 31)
				for (int i = 0; i < line.length(); i++)
				{


					if ((line.at(i) == ' ') || (i + 1 >= line.length()))
					{

						//if (show_console_info)cout << "subdata | " << sub_data << " |" << endl;
						if (compare_with_key(sub_data, "text="))
						{
							if (show_console_info) cout << "name | " << sub_data << " | id:" << font_array_id << endl;
							//name[font_array_id] =sub_data;
						}
						if (compare_with_key(sub_data, "x="))
						{
							if (show_console_info) cout << "x pos | " << get_float_from_sub_data(sub_data) << " |" << endl;
							x[font_array_id] = get_float_from_sub_data(sub_data);
						}

						if (compare_with_key(sub_data, "y="))
						{
							if (show_console_info) cout << "y pos | " << get_float_from_sub_data(sub_data) << " |" << endl;
							y[font_array_id] = get_float_from_sub_data(sub_data);
						}

						if (compare_with_key(sub_data, "width="))
						{
							if (show_console_info) cout << "x size | " << get_float_from_sub_data(sub_data) << " |" << endl;
							real_size_x[font_array_id] = size_x[font_array_id] = get_float_from_sub_data(sub_data);
						}

						if (compare_with_key(sub_data, "height="))
						{
							real_size_y[font_array_id] = size_y[font_array_id] = get_float_from_sub_data(sub_data);
							if (show_console_info) cout << "y size | " << size_y[font_array_id] << " |" << endl;
						}

						if (compare_with_key(sub_data, "origin-x="))
						{
							if (show_console_info) cout << "x offset | " << get_float_from_sub_data(sub_data) << " |" << endl;
							offset_x[font_array_id] = get_float_from_sub_data(sub_data);
						}

						if (compare_with_key(sub_data, "origin-y="))
						{

							offset_y[font_array_id] = size_y[font_array_id] - get_float_from_sub_data(sub_data);
							if (show_console_info) cout << "y offset | size_y=" << size_y[font_array_id] << " offset=" << get_float_from_sub_data(sub_data) << " result=" << offset_y[font_array_id] << " |" << endl;


						}

						if (compare_with_key(sub_data, "advance="))
						{
							if (show_console_info) cout << "advance | " << get_float_from_sub_data(sub_data) << " |" << endl;
							advance[font_array_id] = get_float_from_sub_data(sub_data);


							size_x[font_array_id] = size_x[font_array_id] / 4096.0f;
							size_y[font_array_id] = size_y[font_array_id] / 4096.0f;


							x[font_array_id] = gabarite->x + x[font_array_id] / 4096.0f;
							y[font_array_id] = gabarite->y + gabarite->uv_size_y - y[font_array_id] / 4096.0f;

						}




						/*
						if (data_order == 1) { cout << "y pos | " << sub_data << " |" << endl; y[font_array_id] = std::stof(sub_data)/128.0;}

						if (data_order == 2) { cout << "x size | " << sub_data << " |" << endl; size_x[font_array_id] = std::stof(sub_data)/512.0;}
						if (data_order == 3) { cout << "y size | " << sub_data << " |" << endl; size_y[font_array_id] = std::stof(sub_data)/128.0;}

						if (data_order == 4) { cout << "x offset | " << sub_data << " |" << endl; offset_x[font_array_id] = std::stof(sub_data);}
						if (data_order == 5) { cout << "y offset | " << sub_data << " |" << endl; offset_y[font_array_id] = std::stof(sub_data); }*/
						//

						//if (data_order == 6) { cout << "advance | " << sub_data << " |" << endl; }
						sub_data = "";






						data_order++;
					}
					else
					{
						sub_data += line.at(i);
					}
				}
			if (show_console_info) { cout << endl; }

			font_array_id++;
		}

		myfilez.close();
	}
	else
	{
		cout << "Unable to open file";

	}
}

//http://kvazars.com/littera/
void EFont::load_font_littera(string _path)
{
	string line;
	string sub_data;
	int data_order;

	ifstream myfilez("data/font/" + _path + ".fnt");

	for (int i = 0; i < 300; i++)
	{
		x[i] = 0;
		y[i] = 0;
		size_x[i] = 0;
		size_y[i] = 0;
		offset_x[i] = 0;
		offset_y[i] = 0;
	}

	if (myfilez.is_open())
	{
		/*
		for (int i = 0; i < 200; i++)
		{
			cout << "id:" << i << "   sym:" << (char)i << endl;
		}*/

		//cout << "symbol a=" << (int)'a' << endl;

		while ((getline(myfilez, line)))
		{
			comma_is_open = true;
			data_order = 0;
			//cout << line << '\n';

			//cout << "line #" << font_array_id;
			for (int i = 0; i < line.length(); i++)
			{

				if ((line.at(i) == ' ') || (i + 1 >= line.length()))
				{

					//if (show_console_info)cout << "subdata | " << sub_data << " |" << endl;
					if (compare_with_key(sub_data, "id="))
					{


						font_array_id = get_int_from_sub_data(sub_data);

						if (font_array_id == 1025) { font_array_id = 168; }
						else
							if (font_array_id == 1105) { font_array_id = 184; }
							else
								if (font_array_id > 255) { font_array_id -= 848; }

						if (show_console_info) cout << "subdata:" << sub_data << " id:" << font_array_id << " sym:" << (char)(font_array_id) << endl;
					}

					if (compare_with_key(sub_data, "x="))
					{
						if (show_console_info) cout << "x pos | " << get_float_from_sub_data(sub_data) << " |" << endl;
						x[font_array_id] = get_float_from_sub_data(sub_data);
					}

					if (compare_with_key(sub_data, "y="))
					{
						if (show_console_info) cout << "y pos | " << get_float_from_sub_data(sub_data) << " |" << endl;
						y[font_array_id] = get_float_from_sub_data(sub_data);
					}

					if (compare_with_key(sub_data, "width="))
					{
						if (show_console_info) cout << "x size | " << get_float_from_sub_data(sub_data) << " |" << endl;
						real_size_x[font_array_id] = size_x[font_array_id] = get_float_from_sub_data(sub_data);
					}

					if (compare_with_key(sub_data, "height="))
					{
						real_size_y[font_array_id] = size_y[font_array_id] = get_float_from_sub_data(sub_data);
						if (show_console_info) cout << "y size | " << size_y[font_array_id] << " |" << endl;
					}

					if (compare_with_key(sub_data, "xoffset="))
					{
						if (show_console_info) cout << "x offset | " << get_float_from_sub_data(sub_data) << " |" << endl;
						offset_x[font_array_id] = get_float_from_sub_data(sub_data);
					}

					if (compare_with_key(sub_data, "yoffset="))
					{

						offset_y[font_array_id] = get_float_from_sub_data(sub_data);
						if (show_console_info) cout << "y offset | size_y=" << size_y[font_array_id] << " offset=" << get_float_from_sub_data(sub_data) << " result=" << offset_y[font_array_id] << " |" << endl;


					}

					if (compare_with_key(sub_data, "xadvance="))
					{
						if (show_console_info) cout << "advance | " << get_float_from_sub_data(sub_data) << " |" << endl;

						if (show_console_info) { cout << "font_array_id=" << font_array_id << endl; }
						advance[font_array_id] = get_float_from_sub_data(sub_data);

						size_x[font_array_id] = size_x[font_array_id] / 4096.0f;
						size_y[font_array_id] = size_y[font_array_id] / 4096.0f;

						x[font_array_id] = gabarite->x + x[font_array_id] / 4096.0f;
						y[font_array_id] = gabarite->y + gabarite->uv_size_y - y[font_array_id] / 4096.0f;
					}




					/*
					if (data_order == 1) { cout << "y pos | " << sub_data << " |" << endl; y[font_array_id] = std::stof(sub_data)/128.0;}

					if (data_order == 2) { cout << "x size | " << sub_data << " |" << endl; size_x[font_array_id] = std::stof(sub_data)/512.0;}
					if (data_order == 3) { cout << "y size | " << sub_data << " |" << endl; size_y[font_array_id] = std::stof(sub_data)/128.0;}

					if (data_order == 4) { cout << "x offset | " << sub_data << " |" << endl; offset_x[font_array_id] = std::stof(sub_data);}
					if (data_order == 5) { cout << "y offset | " << sub_data << " |" << endl; offset_y[font_array_id] = std::stof(sub_data); }*/
					//

					//if (data_order == 6) { cout << "advance | " << sub_data << " |" << endl; }
					sub_data = "";






					data_order++;
				}
				else
				{
					sub_data += line.at(i);
				}
			}
			if (show_console_info) { cout << endl; }

			font_array_id++;
		}

		myfilez.close();
	}
	else
	{
		cout << "Unable to open file";

	}
}

void EFont::draw(string _s, Batcher _batch)
{

}

float EFont::get_width(EFont* _font, string _text)
{
	float temp_w = 0;
	float max_w = 0;

	std::string zzz = "";

	//std::cout << "-------" << std::endl;

	for (int sym = 0; sym < _text.length(); sym++)
	{
		int target_symbol = (int)_text.at(sym);
		if (target_symbol < 0) { target_symbol += 256; }

		zzz += _text.at(sym);

		/*
		if (_text.at(sym) == ' ')
		{
			temp_w += _font->advance[target_symbol];
		}
		else
		{
			if (sym + 1 < _text.length())
			{temp_w += (_font->advance[target_symbol]) * _font->scale;}
			else
			{temp_w += (_font->real_size_x[target_symbol]) * _font->scale;}
		}
		*/
		temp_w += (_font->advance[target_symbol]) * _font->scale;

		//std::cout << _text.substr(sym, 2) << std::endl;

		if ((sym + 1 < _text.length()) && (_text.substr(sym, 2) == "\\n"))
		{
			if (temp_w > max_w)
			{
				//std::cout << zzz << " " << temp_w << std::endl;

				zzz = "";

				max_w = temp_w;

			}

			temp_w = 0;
		}
	}

	if (temp_w > max_w)
	{
		//std::cout << zzz << " final " << temp_w << std::endl;

		max_w = temp_w;
	}

	return max_w;
}

float EFont::get_height(EFont* _font, string _text)
{
	float temp_h = 17.0f;
	for (int sym = 0; sym < _text.length(); sym++)
	{
		int target_symbol = (int)_text.at(sym);
		if (target_symbol < 0) { target_symbol += 256; }


		if ((target_symbol == '\\') && (sym + 2 < _text.length()) && (_text.at(sym + 1) == 'n'))
		{
			temp_h += 17.0f;
		}
	}

	return temp_h;
}

void EFont::set_align_once(Enums::PositionMode _al)
{
	align_prev = align_x;
	align_x = _al;
	align_only_once = true;
}
