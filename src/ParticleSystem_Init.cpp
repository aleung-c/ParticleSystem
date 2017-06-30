#include "../includes/ParticleSystem.hpp"

void		ParticleSystem_Init(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	(void)PSDatas;
	// Initializing aleung-c engine->
	engine->InitEngine(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
	engine->GetCamera()->Transform.Position.x = 0.0;
	engine->GetCamera()->Transform.Position.y = 0.0;
	engine->GetCamera()->Transform.Position.z = -50.0;
	engine->SetCameraLookAt(glm::vec3(0.0, 0.0, 0.0));

	// Initializing OPENCL COMPONENTS and kernels.
	engine->CLController.InitOpenCL();
	engine->CLController.LoadKernelFile("./kernels/particles_position.cl");
	engine->CLController.BuildProgram();
	engine->CLController.CreateKernel("place_particles_cubic"); // kernel slot 0
	engine->CLController.CreateKernel("place_particles_spheric"); // kernel slot 1
	engine->CLController.CreateKernel("animate_particles"); // kernel slot 2

	// Init Events
	glfwSetWindowUserPointer(engine->Window, (void *)PSDatas);
	glfwSetKeyCallback(engine->Window, &KeyCallback);
}

/*
**	This function will initiate the scene and put a random positionning
**	on my particles.
**	As there can be several kernels, i made a slot system for them.
**	As of now, 0 is "place_particles", 1 is "animate_particles".
*/

void		ParticleSystem_SceneInit(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	// Setting the main particle object.
	PSDatas->Particle = new ParticleObject(100000, 30.0);
	PSDatas->Particle->Speed = 0.05;
	PSDatas->Particle->Transform.Position.x = 0.0;
	PSDatas->Particle->Transform.Position.y = 0.0;
	PSDatas->Particle->Transform.Position.z = 0.0;
	PositionParticlesRandomly(engine, PSDatas);
}

/*
**	This function is the core of the random positionning.
**	I created a new openGL buffer with random numbers to position
**	my particles, because OpenCL kernel language does NOT have easy access to
**	rand functions.
*/

void		PositionParticlesRandomly(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	// Creating suite of random numbers WITHOUT allocating on the host. \o/
	double					val;
	int						ret;

	srand(getpid());
	// creating openGL buffer for the random suite of doubles.
	glGenVertexArrays(1, &PSDatas->Randsuite_vao);
	glGenBuffers(1, &PSDatas->Randsuite_vbo);	
	glBindBuffer(GL_ARRAY_BUFFER, PSDatas->Randsuite_vbo);
	glBufferData(GL_ARRAY_BUFFER, PSDatas->Particle->ParticleNumber * sizeof(double) * 3, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 1, GL_DOUBLE, GL_FALSE, 0, NULL);
	for (int i = 0; i < PSDatas->Particle->ParticleNumber * 3; i++)
	{
		val = GetRandomDouble(-(double)PSDatas->Particle->Radius, (double)PSDatas->Particle->Radius);
		glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(double), sizeof(double), &val);
	}
	glEnableVertexAttribArray(0);
	// OpenGL buffer <-> OpenCL buffer linkage.
	PSDatas->Randsuite_ObjMem = clCreateFromGLBuffer(engine->CLController.Context, CL_MEM_WRITE_ONLY,
		PSDatas->Randsuite_vbo, &ret);
	if (ret != CL_SUCCESS)
	{
		printf("ParticleSystem_SceneInit - clCreateFromGLBuffer: %s\n",
			engine->CLController.GetCLErrorString(ret));
	}

	/* Set OpenCL Kernel Parameters */
	engine->CLController.SetKernelArg(0, 0, sizeof(cl_mem), (void *)&PSDatas->Particle->ObjMem);
	engine->CLController.SetKernelArg(0, 1, sizeof(float *), (void *)&PSDatas->Particle->Transform.Position);
	engine->CLController.SetKernelArg(0, 2, sizeof(float), (void *)&PSDatas->Particle->Radius); // !! beware of this syntax.
	engine->CLController.SetKernelArg(0, 3, sizeof(cl_mem), (void *)&PSDatas->Randsuite_ObjMem);

	/* Set OpenCL Kernel Parameters for sphere as well */
	engine->CLController.SetKernelArg(1, 0, sizeof(cl_mem), (void *)&PSDatas->Particle->ObjMem);
	engine->CLController.SetKernelArg(1, 1, sizeof(float *), (void *)&PSDatas->Particle->Transform.Position);
	engine->CLController.SetKernelArg(1, 2, sizeof(float), (void *)&PSDatas->Particle->Radius); // !! beware of this syntax.
	engine->CLController.SetKernelArg(1, 3, sizeof(cl_mem), (void *)&PSDatas->Randsuite_ObjMem);

	/* Execute the kernel, as of now in NDRange -> data parallelism. */
	engine->CLController.ExecuteParticleKernel(0, PSDatas->Particle);
}


