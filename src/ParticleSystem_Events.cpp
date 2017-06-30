#include "../includes/ParticleSystem.hpp"

void	KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	t_ParticleSystemDatas			*PSDatas;

	(void)key;
	(void)scancode;
	(void)action;
	(void)mods;
	PSDatas = (t_ParticleSystemDatas *)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_E)
		{
			PSDatas->engine->CLController.ExecuteParticleKernel(0, PSDatas->Particle);
		}
		if (key == GLFW_KEY_R)
		{
			PSDatas->engine->CLController.ExecuteParticleKernel(1, PSDatas->Particle);
		}
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		std::cout << "Exiting ParticleSystem ..." << std::endl;
		exit(0);
	}
}
