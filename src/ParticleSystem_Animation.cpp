#include "../includes/ParticleSystem.hpp"

void		PrepareParticlesAnimation(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	(void)engine;
	(void)PSDatas;

	engine->CLController.CreateKernel("animate_particles"); // kernel slot 1
	// Set OpenCL Kernel Parameters - remember: 1 == "animate_particles"
	engine->CLController.SetKernelArg(1, 0, sizeof(cl_mem), (void *)&PSDatas->Particle->ObjMem);
	engine->CLController.SetKernelArg(1, 1, sizeof(float *), (void *)&PSDatas->Particle->Transform.Position);
	engine->CLController.SetKernelArg(1, 2, sizeof(float), (void *)&PSDatas->Particle->Radius);
	// engine->CLController.SetKernelArg(1, 3, sizeof(double *), (void *)&randsuite_ObjMem);
}

void		UpdateParticlesAnimation(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	float		fCursor_x;
	float		fCursor_y;

	engine->CLController.SetKernelArg(1, 0, sizeof(cl_mem), (void *)&PSDatas->Particle->ObjMem);
	engine->CLController.SetKernelArg(1, 1, sizeof(float *), (void *)&PSDatas->Particle->Transform.Position);
	engine->CLController.SetKernelArg(1, 2, sizeof(float), (void *)&PSDatas->Particle->Radius);
	// send mouse position flat for now.
	fCursor_x = (float)PSDatas->Cursor_x;
	fCursor_y = (float)PSDatas->Cursor_y;

	engine->CLController.SetKernelArg(1, 3, sizeof(float), (void *)&fCursor_x);
	engine->CLController.SetKernelArg(1, 4, sizeof(float), (void *)&fCursor_y);

	engine->CLController.ExecuteParticleKernel(1, PSDatas->Particle);
}
