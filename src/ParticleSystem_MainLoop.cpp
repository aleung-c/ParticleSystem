#include "../includes/ParticleSystem.hpp"

void		ParticleSystem_MainLoop(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	(void)PSDatas;
	engine->CheckForOpenGLErrors();
	PrepareParticlesAnimation(engine, PSDatas);
	while (!glfwWindowShouldClose(engine->Window))
	{
		glfwPollEvents();
		glfwGetCursorPos(engine->Window, &PSDatas->Cursor_x, &PSDatas->Cursor_y);
		// printf("Mouse at %fx %fy\n", PSDatas->Cursor_x, PSDatas->Cursor_y); -> functional, but screenspace.

		// every frame code
		
		// [...]

		engine->Draw();
	}
}