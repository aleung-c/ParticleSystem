#include "../../../includes/ParticleSystem.hpp"


TextureComponent::TextureComponent()
{
	_objTexture = NULL;
	_hasTexture = false;
}

/*
**	Returns the object's texture buffer id.
*/

GLuint		TextureComponent::GetTextureID()
{
	return (_objTextureID);
}

/*
**	Returns the object's bmp texture object.
*/

t_bmp_texture		*TextureComponent::GetTexture()
{
	return (_objTexture);
}

bool				TextureComponent::HasTexture()
{
	return (_hasTexture);
}

void				TextureComponent::SetTextureFlag(bool flag)
{
	_hasTexture = flag;
}

/*
**	Public method -
**	If an object is already textured, you can load a new texture with this method.
**	However, this is not the recommended method if you want to change texture many
**	times during runtime, as it will open and parse the texture file each time
**	it is called. This one is to be used casually.
**
**	For runtime multi texture buffering, use the SetTexture() public method, and load
**	bmp textures objects in your game code.
*/

void		TextureComponent::Load(std::string path)
{
	// Checking if the texture is already loaded in the texture manager.
	_objTexture = EngineController::Instance().TextureManager.GetTexture(path);
	if (!_objTexture)
	{
		// if not, load it now
		_objTexture = (t_bmp_texture *)malloc(sizeof(t_bmp_texture));
		if ((EngineController::LoadTextureFile(_objTexture, path)) == -1)
		{
			_hasTexture = false;
			free(_objTexture);
			_objTexture = NULL;
			return ;
		}
	}
	// the loaded texture is now in this->_objTexture
	// _objTexture->texture_path = path;
	EngineController::Instance().TextureManager.AddTexture(_objTexture);
	_hasTexture = true;
	_objTextureID = _objTexture->texture_id;
	// glGenTextures(1, &_objTextureID);

	// glBindTexture(GL_TEXTURE_2D, _objTextureID);
	// glTexStorage2D(GL_TEXTURE_2D, EngineController::Instance().Settings.TextureMipMapValue, GL_RGBA,
	// _objTexture->width, _objTexture->height);

	// // Give the image to OpenGL
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	// 	_objTexture->width,
	// 	_objTexture->height,
	// 	0, GL_BGRA, GL_UNSIGNED_BYTE, _objTexture->data);

	// glGenerateMipmap(GL_TEXTURE_2D);
	// // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// _objTexture->texture_id = _objTextureID;
}

/*
**	Set the object's bmp texture object. Use this to quickly switch between
**	loaded textures during runtime.
**	Make sure the new texture is the same size and is identically mapped
**	as the previous texture.
**	Otherwise the behavior is undefined.
**
**	Note that this does NOT free the old texture pointer.
**	In some cases, you will have leaks using this.
**	You may want to use ClearTexture() before using this,
**	or only use this on untextured objects.
**	you should also look at LoadNewTexture(std::string path)
**	or ReplaceTexture(t_bmp_texture *newTexture).
*/

int			TextureComponent::Swap(t_bmp_texture *newTexture)
{
	// if (_hasTexture == true)
	// {
	// 	glDeleteTextures(1, &_objTextureID);
	// }
	if (newTexture)
	{
		this->_objTexture = newTexture;
		// glGenTextures(1, &_objTextureID);
		_objTextureID = newTexture->texture_id;
		_hasTexture = true;
	}
	else
	{
		_hasTexture = false;
		return (-1);
	}
	return (0);
}

/*
**	Changes the t_bmp_texture by the new one, and delete and free the previous texture.
*/

int			TextureComponent::Replace(t_bmp_texture *newTexture)
{
	if (_hasTexture == true)
	{
		glDeleteTextures(1, &_objTextureID);
		free(_objTexture->data);
		free(_objTexture);
	}
	if (newTexture)
	{
		this->_objTexture = newTexture;
		// glGenTextures(1, &_objTextureID);
		this->_objTextureID = newTexture->texture_id;
		_hasTexture = true;
	}
	else
	{
		_hasTexture = false;
		this->_objTexture = NULL;
		return (-1);
	}
	return (0);
}

/*
**	Delete the current texture on the object, freeing the pointer as it should be.
*/

void		TextureComponent::Clear()
{
	if (_hasTexture == true)
	{
		glDeleteTextures(1, &_objTextureID);
		free(_objTexture->data);
		free(_objTexture);
		_hasTexture = false;
	}
}
