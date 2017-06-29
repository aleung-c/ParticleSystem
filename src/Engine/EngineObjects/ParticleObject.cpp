#include "../../../includes/ParticleSystem.hpp"

ParticleObject::ParticleObject(int particleNumber, float rad)
{
	ParticleNumber = particleNumber;
	Radius = rad;
	Visible = true;
	initParticleObject();
	EngineController::Instance().ParticleObjectList.push_back(this);
}

ParticleObject::~ParticleObject()
{
	clReleaseMemObject(ObjMem);
}

/*
**	Init the buffer with the position of each particle.
**	As asked from the subject, the values are directly set into
**	the buffer(VRAM), that is why I dont have any vector of points in
**	this class.
*/

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

	// float x = 0.0;
	// float y = 0.0;
	// float z = 0.0;

	// for (int i = 0; i != ParticleNumber; i++)
	// {
	// 	pos.push_back(glm::vec4(x, y, z, 1.0));
	// 	x += 0.1;
	// 	y += 0.1;
	// 	z += 0.1;
	// }

	float	zero;

	zero = 0.0;
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, ParticleNumber * sizeof(glm::vec4), NULL,
					GL_STATIC_DRAW);
	glClearBufferfv(GL_ARRAY_BUFFER, _vbo, &zero);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// ObjMem = (cl_mem)gcl_gl_create_ptr_from_buffer(_vbo);
	int ret;
	ObjMem = clCreateFromGLBuffer(EngineController::Instance().CLController.Context, CL_MEM_WRITE_ONLY,
		_vbo, &ret);
	if (ret != CL_SUCCESS)
	{
		printf("clCreateFromGLBuffer: %s\n",
			EngineController::Instance().CLController.GetCLErrorString(ret));
	}
}
