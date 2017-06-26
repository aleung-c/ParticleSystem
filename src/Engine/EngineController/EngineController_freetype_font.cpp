#include "../../../includes/ParticleSystem.hpp"

// --------------------------------------------------------------------	//
//																		//
//	This file contains methods for writing text on the screen 			//
//	Using freetype 2 library for loading truetype fonts					//
//																		//
// --------------------------------------------------------------------	//

// Thanks to Joey de Vries from https://learnopengl.com/#!In-Practice/Text-Rendering
// Most of the code in this file comes from there.

// --------------------------------------------------------------------	//
//																		//
//	Freetype lib font methods											//
//																		//
// --------------------------------------------------------------------	//

int		EngineController::initFreeType()
{
	if (FT_Init_FreeType(&FT_Lib))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return (-1);
	}
	// Get one font
	int error = FT_New_Face(FT_Lib,
				"./fonts/Roboto_Condensed/RobotoCondensed-Regular.ttf", 0, &Face);
	if (error == FT_Err_Unknown_File_Format)
	{
		std::cout << KRED "FreeType init: Font format not supported" KRESET << std::endl;
		return (-1);
	}
	else if (error)
	{
		std::cout << KRED "FreeType init: Cant open or read font file." KRESET << std::endl;
		return (-1);
	}
	FT_Set_Pixel_Sizes(Face, 0, 48);
	loadFreeTypesCharacters();
	return (0);
}

/*
**	In here, we stock ascii's first 128 characters from truetype font into a map
**	that we will use to draw our letters.
*/

void	EngineController::loadFreeTypesCharacters()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(Face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint	texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			Face->glyph->bitmap.width,
			Face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, Face->glyph->bitmap.buffer);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		 // Now store character for later use
		Character character = {
			texture, 
			glm::ivec2(Face->glyph->bitmap.width, Face->glyph->bitmap.rows),
			glm::ivec2(Face->glyph->bitmap_left, Face->glyph->bitmap_top),
			Face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	FT_Done_Face(Face);
	FT_Done_FreeType(FT_Lib);
}
