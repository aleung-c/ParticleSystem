#include "../includes/ParticleSystem.hpp"

void	KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	t_ParticleSystemDatas			*PSDatas;

	(void)key;
	(void)scancode;
	(void)action;
	(void)mods;
	PSDatas = (t_ParticleSystemDatas *)glfwGetWindowUserPointer(window);
	// TODO: left click change grav point;
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
		if (key == GLFW_KEY_SPACE)
		{
			PSDatas->FollowKeyPressed = true;
		}
	}
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_SPACE)
		{
			PSDatas->FollowKeyPressed = false;
		}
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		std::cout << "Exiting ParticleSystem ..." << std::endl;
		exit(0);
	}
}

/*
**	Not very precise right now, but does the work...
*/

void	GetMouseWorldPosition(t_ParticleSystemDatas *PSDatas)
{
	glm::mat4 			mouse_unprojection_mat;
	glm::vec4			in;
	static float		winZ = 50.0;
	glm::vec4			screen_ray;
	glm::vec3			worldPos;
	glm::vec4			screenPos;

	// Mouse position flat.
	PSDatas->World_fCursor_x = (float)PSDatas->Cursor_x;
	PSDatas->World_fCursor_y = (float)PSDatas->Cursor_y;

	// screen to world pos unprojected
	mouse_unprojection_mat = -(PSDatas->engine->MatPerspectiveProjection * PSDatas->engine->MatView);

	in.x = (2.0f * ((float)(PSDatas->World_fCursor_x - 0) / (WINDOW_WIDTH - 0))) - 1.0f,
	in.y = 1.0f - (2.0f * ((float)(PSDatas->World_fCursor_y - 0) / (WINDOW_HEIGHT - 0)));
	in.z = 2.0 * winZ - 1.0;
	in.w = 1.0;

	screenPos = mouse_unprojection_mat * in;
	screenPos.w = 1.0 / screenPos.w;
	screenPos.x *= screenPos.w;
	screenPos.y *= screenPos.w;
	screenPos.z *= screenPos.w;

	// printf("in screenpos = %fx %fy %fz\n", screenPos.x, screenPos.y, screenPos.z);

	screen_ray = screenPos - glm::vec4(0.0);

	worldPos = PSDatas->engine->GetCamera()->Transform.Position +
				-(glm::vec3)screenPos * PSDatas->engine->GetCamera()->Transform.Position.z;

	// printf("in worldpos = %fx %fy %fz\n", worldPos.x, worldPos.y, worldPos.z);

	PSDatas->World_fCursor_x = (float)worldPos.x * 50; // .... =_='
	PSDatas->World_fCursor_y = (float)worldPos.y * 50;

	// printf("Mouse world pos at %fx %fy\n", cursorWorldSpace_x, cursorWorldSpace_y);
}