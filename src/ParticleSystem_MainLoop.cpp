#include "../includes/ParticleSystem.hpp"

void		ParticleSystem_MainLoop(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	(void)PSDatas;
	engine->CheckForOpenGLErrors();
	PrepareParticlesAnimation(engine, PSDatas);

	// ----- FPS COUNT
	int nbFrames = 0;
	double lastTime = glfwGetTime();
	GameTextObject	*fpsCount = new GameTextObject("fpsCount", "0.0 fps");
	fpsCount->Transform.Position.x += 15.0;
	fpsCount->Transform.Position.y += 15.0;
	while (!glfwWindowShouldClose(engine->Window))
	{
		// ------ FPS counter;
		double currentTime = glfwGetTime();

		nbFrames++;
		if ( currentTime - lastTime >= 1.0 )
		{
			fpsCount->Text = std::to_string(1000.0 / double(nbFrames)) + "ms";
			nbFrames = 0;
			lastTime += 1.0;
		}

		glfwPollEvents();
		glfwGetCursorPos(engine->Window, &PSDatas->Cursor_x, &PSDatas->Cursor_y);

		UpdateParticlesAnimation(engine, PSDatas);
		// printf("Mouse at %fx %fy\n", PSDatas->Cursor_x, PSDatas->Cursor_y); // -> functional, but screenspace.

		engine->Draw();
	}
}