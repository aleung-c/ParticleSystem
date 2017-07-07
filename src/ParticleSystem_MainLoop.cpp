#include "../includes/ParticleSystem.hpp"

void		ParticleSystem_MainLoop(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	engine->CheckForOpenGLErrors();
	PrepareParticlesAnimation(engine, PSDatas);

	// ----- FPS COUNT
	int			nbFrames = 0;
	double		lastTime = glfwGetTime();
	double		currentTime;

	GameTextObject	*fpsCount = new GameTextObject("fpsCount", "0.0 fps");
	fpsCount->Transform.Position.x += 15.0;
	fpsCount->Transform.Position.y += 15.0;
	while (!glfwWindowShouldClose(engine->Window))
	{
		// ------ FPS counter //
		currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{
			fpsCount->Text = std::to_string(nbFrames) + "fps";
			nbFrames = 0;
			lastTime += 1.0;
		}
		// ------ FPS counter - END //
		glfwPollEvents();
		glfwGetCursorPos(engine->Window, &PSDatas->Cursor_x, &PSDatas->Cursor_y);
		GetMouseWorldPosition(PSDatas);
		UpdateCamera(PSDatas);
		UpdateParticlesAnimation(engine, PSDatas);
		engine->Draw();
	}
	engine->CLController.CleanMemory();
}
