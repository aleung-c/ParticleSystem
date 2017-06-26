#ifndef ENGINEOBJECT_HPP
# define ENGINEOBJECT_HPP

#include "../ParticleSystem.hpp"

/*
**	The base class for the objects
**	Each object whatever his type has a set of base component.
**	That means the transform, the texture, etc etc.
**	This allow similar behaviors throughout our objects.
*/

class EngineObject
{
	public:
		EngineObject();
		~EngineObject();
		
		std::string						Name;
		bool							Visible;

		// ----- Object's base components

		TransformComponent				Transform;

		TextureComponent				Texture;

		GLuint							GetVao();
		GLuint							GetVbo();		

	protected:
		GLuint							_vao;
		GLuint							_vbo;
};


#endif
