#include "EGraphicCore.h"



unsigned int EGraphicCore::texture[32];
unsigned char* EGraphicCore::data1;

int EGraphicCore::width;
int EGraphicCore::height;
int EGraphicCore::nrChannels;

int EGraphicCore::last_texture_w;
int EGraphicCore::last_texture_h;

Shader* EGraphicCore::ourShader;
Batcher* EGraphicCore::batch;

glm::mat4 EGraphicCore::matrix_transform;

int EGraphicCore::SCR_WIDTH = 1000;
int EGraphicCore::SCR_HEIGHT = 800;

float EGraphicCore::correction_x = 0.1f;
float EGraphicCore::correction_y = 0.1f;

std::vector<EGabarite*> EGraphicCore::gabarite_list;

//std::vector<EGabarite*> EGraphicCore::gabarite_collection;

EGabarite* EGraphicCore::gabarite_white_pixel;
EGabarite* EGraphicCore::gabarite_tileset;

void EGraphicCore::load_texture(char const* _path, int _id)
{
	glGenTextures(_id, &texture[_id]);
	glBindTexture(GL_TEXTURE_2D, texture[_id]);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	data1 = stbi_load(_path, &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);

		//cout << "loaded texture W:" << width << " H:" << height << endl;

		last_texture_h = height;
		last_texture_w = width;

		//texture_error = false;
		/*texture_w[_id] = width;
		texture_h[_id] = height;*/
	}
	else
	{
		//cout << red << "Failed to load texture " << yellow << "(" << _path << ")" << green << endl;

		//last_texture_h = 21;
		//last_texture_w = 21;

		//texture_error = true;

		//just_created_gabarite = DefaultGabarite::gabarite_error;
	}

	stbi_image_free(data1);
}

EGraphicCore::EGraphicCore()
{
}

EGraphicCore::~EGraphicCore()
{
}
