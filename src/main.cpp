#include "../includes/ParticleSystem.hpp"

int		main(void)
{
	EngineController *Engine = &EngineController::Instance();

	std::cout << "Hello particle system!" << std::endl;
	Engine->InitEngine(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

	Engine->CheckForOpenGLErrors();
	while (!glfwWindowShouldClose(Engine->Window))
	{
		glfwPollEvents();
		Engine->Draw();
	}
	return 0;
}
