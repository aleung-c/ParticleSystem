#include "../../../includes/ParticleSystem.hpp"

// --------------------------------------------------------------------	//
//																		//
//	The class for a game object that is text only.						//
//	A new class seems useful because the text are not handled the same	//
//	way as a 3d model.													//
//																		//
// --------------------------------------------------------------------	//

GameUIObject::GameUIObject(std::string name, std::string imgPath)
{
	Name = name;
	initValues();
	setBuffers();

	Texture.Load(imgPath);

	// Generate texture -> This must be done at this point, unlike with 3d objects.
	glBindTexture(GL_TEXTURE_2D, Texture.GetTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		Texture.GetTexture()->width,
		Texture.GetTexture()->height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, Texture.GetTexture()->data);

	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Add this object to the engine's list of objects.
	EngineController::Instance().GameUIObjectList.push_back(this);
}

GameUIObject::GameUIObject(std::string name)
{
	Name = name;
	initValues();
	// Add this object to the engine's list of objects.
	EngineController::Instance().GameUIObjectList.push_back(this);
}

void		GameUIObject::initValues()
{
	Transform.Position = glm::vec3(0.0, 0.0, 0.0);
	Transform.ScaleValue = 1.0;
}

void		GameUIObject::setBuffers()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
}
