#include "../../../includes/ParticleSystem.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Texture Manager														//
//																		//
//	This class will stock and returns the textures that have been		//
//	loaded during runtime, as loading ten times the same texture		//
//	cause too many performance problems.								//
//																		//
// --------------------------------------------------------------------	//

void				TextureManager::AddTexture(t_bmp_texture *texture)
{
	TextureList.push_back(texture);
}

t_bmp_texture		*TextureManager::GetTexture(std::string texPath)
{
	for (std::vector<t_bmp_texture *>::iterator it = TextureList.begin();
			it != TextureList.end(); it++)
	{
		if ((*it)->texture_path == texPath)
		{
			return (*it);
		}
	}
	return (NULL);
}
