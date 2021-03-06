#include "../includes/ParticleSystem.hpp"

/*
**	Used BEFORE the main loop, to set the main variables that will be used
**	in the kernel.
*/

void		PrepareParticlesAnimation(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	// Set OpenCL Kernel Parameters - remember: index 2 == "animate_particles"
	engine->CLController.SetKernelArg(2, 0, sizeof(cl_mem), (void *)&PSDatas->Particle->ObjMem);
	engine->CLController.SetKernelArg(2, 1, sizeof(cl_mem), (void *)&PSDatas->Particle->Distance_ObjMem);
	engine->CLController.SetKernelArg(2, 2, sizeof(float *), (void *)&PSDatas->Particle->Transform.Position);
	engine->CLController.SetKernelArg(2, 3, sizeof(float), (void *)&PSDatas->Particle->Speed);
}

/*
**	Used at each main loop turn.
**	Will update the input values for the kernel, which will in turn calculate the
**	new position of each particle.
*/

void		UpdateParticlesAnimation(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	if (PSDatas->FollowKeyPressed == true)
	{
		// grav point = mouse world pos;
		engine->CLController.SetKernelArg(2, 4, sizeof(float), (void *)&PSDatas->World_fCursor_x);
		engine->CLController.SetKernelArg(2, 5, sizeof(float), (void *)&PSDatas->World_fCursor_y);
	}
	else
	{
		// grav point = last set point;
		engine->CLController.SetKernelArg(2, 4, sizeof(float), (void *)&PSDatas->GravityPoint_x);
		engine->CLController.SetKernelArg(2, 5, sizeof(float), (void *)&PSDatas->GravityPoint_y);
	}
	engine->CLController.SetKernelArg(2, 3, sizeof(float), (void *)&PSDatas->Particle->Speed);
	engine->CLController.SetKernelArg(2, 6, sizeof(float), (void *)&PSDatas->World_fCursor_x);
	engine->CLController.SetKernelArg(2, 7, sizeof(float), (void *)&PSDatas->World_fCursor_y);
	engine->CLController.SetKernelArg(2, 8, sizeof(int), (void *)&PSDatas->ParticleStatus);

	// Execute kernels.
	engine->CLController.ExecuteParticleKernel(2, PSDatas->Particle);
}
