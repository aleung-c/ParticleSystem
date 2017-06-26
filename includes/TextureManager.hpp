#ifndef TEXTUREMANAGER_HPP
# define TEXTUREMANAGER_HPP

#include "ParticleSystem.hpp"

/*
**	This class will stock called textures and reassign them to
**	objects (using their pointer) to avoid multiple texture copy
**	overhead.
*/

class TextureManager
{
	public:
		std::vector<t_bmp_texture *>	TextureList;

		void				AddTexture(t_bmp_texture *texture);
		t_bmp_texture		*GetTexture(std::string texPath);
};

#endif
