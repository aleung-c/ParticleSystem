#include "../../../includes/ParticleSystem.hpp"

ParticleObject::ParticleObject(int particleNumber)
{
	ParticleNumber = particleNumber;
	Visible = true;
	initParticleObject();
	EngineController::Instance().ParticleObjectList.push_back(this);
}

ParticleObject::~ParticleObject()
{

}

void	ParticleObject::initParticleObject()
{


	Transform.Position = glm::vec3(0.0, 0.0, 0.0);
	// for now, we will imagine euleur rotations.
	Transform.Rotation = glm::vec3(0.0, 0.0, 0.0);
	Transform.Scale = glm::vec3(1.0, 1.0, 1.0);

	// set VAO
	_vao = 0;
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// set VBO
	_vbo = 0;
	glGenBuffers(1, &(_vbo));

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	for (int i = 0; i != ParticleNumber; i++)
	{
		pos.push_back(glm::vec4(x, y, z, 1.0));
		x += 0.1;
		y += 0.1;
		z += 0.1;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, ParticleNumber * sizeof(glm::vec4), &pos[0],
					GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	ObjMem = (cl_mem)gcl_gl_create_ptr_from_buffer(_vbo);

}
