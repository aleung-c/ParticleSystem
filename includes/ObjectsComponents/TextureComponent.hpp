#ifndef TEXTURECOMPONENT_HPP
# define TEXTURECOMPONENT_HPP

#include "../ParticleSystem.hpp"

class TextureComponent
{
	public:
		TextureComponent();

		std::string						TexturePath;

		GLuint							GetTextureID();
		t_bmp_texture					*GetTexture();

		// ----- Texture handling methods.
		
		bool							HasTexture();
		void							SetTextureFlag(bool flag);
		int								Swap(t_bmp_texture *newTexture);
		int								Replace(t_bmp_texture *newTexture);
		void							Clear();
		void							Load(std::string path);

	private:
		bool							_hasTexture;
		GLuint							_objTextureID;
		t_bmp_texture					*_objTexture;
};

#endif
