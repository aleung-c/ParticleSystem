#include "../includes/ParticleSystem.hpp"

/*
**	Used BEFORE the main loop, to set the main variables that will be used
**	in the kernel.
*/

void		PrepareParticlesAnimation(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	(void)engine;
	(void)PSDatas;

	// Set OpenCL Kernel Parameters - remember: index 2 == "animate_particles"
	engine->CLController.SetKernelArg(2, 0, sizeof(cl_mem), (void *)&PSDatas->Particle->ObjMem);
	engine->CLController.SetKernelArg(2, 1, sizeof(cl_mem), (void *)&PSDatas->Particle->Distance_ObjMem);
	engine->CLController.SetKernelArg(2, 2, sizeof(cl_mem), (void *)&PSDatas->Randsuite_ObjMem);
	engine->CLController.SetKernelArg(2, 3, sizeof(float *), (void *)&PSDatas->Particle->Transform.Position);
	engine->CLController.SetKernelArg(2, 4, sizeof(float), (void *)&PSDatas->Particle->Radius);
	engine->CLController.SetKernelArg(2, 5, sizeof(float), (void *)&PSDatas->Particle->Speed);
	// engine->CLController.SetKernelArg(2, 8, sizeof(float), (void *)&PSDatas->Particle->Angle_val);
	// engine->CLController.SetKernelArg(1, 3, sizeof(double *), (void *)&randsuite_ObjMem);	
}

/*
**	Used at each main loop turn.
**	Will update the input values for the kernel, which will in turn calculate the
**	new position of each particle.
*/

// TODO : FIX mouse precision.
void		UpdateParticlesAnimation(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	float		fCursor_x;
	float		fCursor_y;

	// send mouse position flat for now.
	fCursor_x = (float)PSDatas->Cursor_x;
	fCursor_y = (float)PSDatas->Cursor_y;

	// screen to world pos projected
	glm::mat4 	mouse_projection_mat;

	mouse_projection_mat = -(engine->MatPerspectiveProjection * engine->MatView);

	glm::vec4	in;
	float		winZ = 50.0;
	glm::vec4	screen_ray;
	glm::vec3	worldPos;

	in.x = (2.0f * ((float)(fCursor_x - 0) / (WINDOW_WIDTH - 0))) - 1.0f,
	in.y = 1.0f - (2.0f * ((float)(fCursor_y - 0) / (WINDOW_HEIGHT - 0)));
	in.z = 2.0 * winZ - 1.0;
	in.w = 1.0;

	glm::vec4	screenPos = mouse_projection_mat * in;

	screenPos.w = 1.0 / screenPos.w;
	screenPos.x *= screenPos.w;
	screenPos.y *= screenPos.w;
	screenPos.z *= screenPos.w;

	// printf("in screenpos = %fx %fy %fz\n", screenPos.x, screenPos.y, screenPos.z);

	screen_ray = screenPos - glm::vec4(0.0);

	worldPos = engine->GetCamera()->Transform.Position +
				-(glm::vec3)screenPos * engine->GetCamera()->Transform.Position.z;
	// printf("in worldpos = %fx %fy %fz\n", worldPos.x, worldPos.y, worldPos.z);

	fCursor_x = (float)worldPos.x * 50; // .... =_='
	fCursor_y = (float)worldPos.y * 50;

	// printf("Mouse world pos at %fx %fy\n", cursorWorldSpace_x, cursorWorldSpace_y);

	engine->CLController.SetKernelArg(2, 6, sizeof(float), (void *)&fCursor_x);
	engine->CLController.SetKernelArg(2, 7, sizeof(float), (void *)&fCursor_y);
	// engine->CLController.SetKernelArg(2, 7, sizeof(float), (void *)&fCursor_y);

	// Execute kernels.
	engine->CLController.ExecuteParticleKernel(2, PSDatas->Particle);
}
