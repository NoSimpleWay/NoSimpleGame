#include "ETextureAtlas.h"

bool ETextureAtlas::can_place_here(int _x, int _y, int _w, int _h)
{
	for (int j = _x; j <= _x + _w; j++)
	for (int i = _y; i <= _y + _h; i++)
	{
		if ((j >= 1024) || (i >= 1024))
		{
			return false;
		}

		if (!free_space[j][i]) { return false; }
	}

	return true;
}

EGabarite* ETextureAtlas::put_texture_to_atlas(std::string _name, ETextureAtlas* _ta)
{



	glViewport(0, 0, 4096, 4096);
	glBindFramebuffer(GL_FRAMEBUFFER, _ta->framebuffer);

	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_MAX);

	//set correct zoom
	EGraphicCore::matrix_transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	EGraphicCore::matrix_transform = glm::translate(EGraphicCore::matrix_transform, glm::vec3(-1, -1, 0.0f));
	EGraphicCore::matrix_transform = glm::scale(EGraphicCore::matrix_transform, glm::vec3(1 / 4096.0f * 2.0f, 1 / 4096.0f * 2.0f, 1));

	//use shader
	EGraphicCore::ourShader->use();

	unsigned int transformLoc = glGetUniformLocation(EGraphicCore::ourShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(EGraphicCore::matrix_transform));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, EGraphicCore::texture[0]);
	EGraphicCore::ourShader->setInt("texture1", 0);
	EGraphicCore::batch->setcolor(EColor::COLOR_WHITE);

	EGraphicCore::load_texture(_name.c_str(), 0);

	

	EGabarite* duplicate_gabarite = NULL;

	//search already loaded texture gabarite
	for (EGabarite* g : EGraphicCore::gabarite_list)
	{
		if (g->name == _name)
		{
			duplicate_gabarite = g;
		}
	}

	EGabarite* new_gabarite = NULL;
	if (duplicate_gabarite == NULL)
	{

		std::cout << "no dublicates with <" << _name << ">" << std::endl;

		int place_x = 0;
		int place_y = 0;
		for (int x = 0; x < 1024; x++)
		for (int y = 0; y < 1024; y++)
		{
			if (_ta->can_place_here(x, y, (int)(EGraphicCore::last_texture_w / 4.0f), (int)(EGraphicCore::last_texture_h / 4.0f)))
			{
				place_x = x * 4;
				place_y = y * 4;

				x = 99999;
				y = 99999;
			}
		}

		for (int x = (int)(place_x / 4.0f); x < (int)((place_x + EGraphicCore::last_texture_w) / 4.0f) + 1; x++)
		for (int y = (int)(place_y / 4.0f); y < (int)((place_y + EGraphicCore::last_texture_h) / 4.0f) + 1; y++)
		if ((x < 1024) && (y < 1024))
		{
			_ta->free_space[x][y] = false;
		}

		EGraphicCore::batch->reset();
		EGraphicCore::batch->draw_rect(place_x, place_y, EGraphicCore::last_texture_w, EGraphicCore::last_texture_h);
		EGraphicCore::batch->reinit();
		EGraphicCore::batch->draw_call();

		std::cout << "draw to x=" << place_x << " y=" << place_y << std::endl;

		new_gabarite = new EGabarite(_name, place_x / 4096.0f, place_y / 4096.0f, EGraphicCore::last_texture_w / 4096.0f, EGraphicCore::last_texture_h / 4096.0f);
		
		EGraphicCore::gabarite_list.push_back(new_gabarite);
	}
	else
	{
		std::cout << "DUBLICATE <" << _name << ">" << std::endl;
		new_gabarite = duplicate_gabarite;
	}

	//load_texture(_path, 0);
	

	//set atlas texture is active
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//return normal blend mode
	glDisable(GL_DEPTH_TEST);
	glBlendEquation(GL_FUNC_ADD);
	

	//--------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------

	//cout << (int)01.35f << endl;

	//return normal viewport
	glViewport(0, 0, EGraphicCore::SCR_WIDTH, EGraphicCore::SCR_HEIGHT);

	//active main texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _ta->textureColorbuffer);
	EGraphicCore::ourShader->setInt("texture1", 0);
	EGraphicCore::batch->setcolor(EColor::COLOR_WHITE);

	return new_gabarite;
}

ETextureAtlas::ETextureAtlas()
{
	for (int j = 0; j < 1024; j++)
	for (int i = 0; i < 1024; i++)
	{free_space[j][i] = true;}

	//glGenTextures(1, &ETexture::texture[1]);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4096, 4096, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 4096, 4096); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, EGraphicCore::SCR_WIDTH, EGraphicCore::SCR_HEIGHT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, EGraphicCore::texture[0]);
	EGraphicCore::ourShader->setInt("texture1", 0);
	EGraphicCore::batch->setcolor(EColor::COLOR_WHITE);

}

ETextureAtlas::~ETextureAtlas()
{
}
