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
	Transform.Rotation = glm::vec3(0.0, 0.0, 0.0);
	Transform.Scale = glm::vec3(1.0, 1.0, 1.0);

	// ----- set particle positions buffers
	float	zero;
	int		ret;

	zero = 0.0;
	// set VAO
	_vao = 0;
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	_vbo = 0;
	glGenBuffers(1, &(_vbo));
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, ParticleNumber * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glClearBufferfv(GL_ARRAY_BUFFER, _vbo, &zero);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// Set interoperability OpenGL - OpenCL
	ObjMem = clCreateFromGLBuffer(EngineController::Instance().CLController.Context,
		CL_MEM_WRITE_ONLY, _vbo, &ret);
	if (ret != CL_SUCCESS)
	{
		printf("clCreateFromGLBuffer particles: %s\n",
			EngineController::Instance().CLController.GetCLErrorString(ret));
	}

	// ----- set particle distance to mouse buffers
	// Will be used to set the color of the particles.
	Distance_Vao = 0;
	glGenVertexArrays(1, &Distance_Vao);
	glBindVertexArray(Distance_Vao);
	Distance_Vbo = 0;
	glGenBuffers(1, &(Distance_Vbo));
	glBindBuffer(GL_ARRAY_BUFFER, Distance_Vbo);
	glBufferData(GL_ARRAY_BUFFER, ParticleNumber * sizeof(float), NULL, GL_STATIC_DRAW);
	glClearBufferfv(GL_ARRAY_BUFFER, Distance_Vbo, &zero);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	// Set interoperability OpenGL - OpenCL
	Distance_ObjMem = clCreateFromGLBuffer(EngineController::Instance().CLController.Context,
		CL_MEM_WRITE_ONLY, Distance_Vbo, &ret);
	if (ret != CL_SUCCESS)
	{
		printf("clCreateFromGLBuffer particles distance: %s\n",
			EngineController::Instance().CLController.GetCLErrorString(ret));
	}
}
