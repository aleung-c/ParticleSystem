#include "../includes/ParticleSystem.hpp"

void		PrepareParticlesAnimation(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	(void)engine;
	(void)PSDatas;
	// Set OpenCL Kernel Parameters - remember: 1 == "animate_particles"
	engine->CLController.SetKernelArg(1, 0, sizeof(cl_mem), (void *)&PSDatas->Particle->ObjMem);
	engine->CLController.SetKernelArg(1, 1, sizeof(float *), (void *)&PSDatas->Particle->Transform.Position);
	engine->CLController.SetKernelArg(1, 2, sizeof(float), (void *)&PSDatas->Particle->Radius);
	// engine->CLController.SetKernelArg(1, 3, sizeof(double *), (void *)&randsuite_ObjMem);
}