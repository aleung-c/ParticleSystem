#include "../includes/ParticleSystem.hpp"

/*
**	The main function for event handling. Catch the keys and prepare
**	to apply the required transformations.
*/

void	KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	t_ParticleSystemDatas			*PSDatas;

	(void)scancode;
	(void)mods;
	PSDatas = (t_ParticleSystemDatas *)glfwGetWindowUserPointer(window);
	// On key press. It seems these events are triggered ONLY ONCE.
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			std::cout << "Exiting ParticleSystem ..." << std::endl;
			PSDatas->engine->CLController.CleanMemory();
			exit(0);
		}
		if (key == GLFW_KEY_E)
		{
			PSDatas->GravityPoint_x = 0.0;
			PSDatas->GravityPoint_y = 0.0;
			// execute CUBIC positionning.
			PSDatas->engine->CLController.ExecuteParticleKernel(0, PSDatas->Particle);
		}
		if (key == GLFW_KEY_R)
		{
			PSDatas->GravityPoint_x = 0.0;
			PSDatas->GravityPoint_y = 0.0;
			// execute SPHERICAL positionning.
			PSDatas->engine->CLController.ExecuteParticleKernel(1, PSDatas->Particle);
		}
		// Particles movement ATTRACTION mode.
		if (key == GLFW_KEY_C)
		{
			PSDatas->ParticleStatus = ATTRACTION;
		}
		// Particles movement REPULSION mode.
		if (key == GLFW_KEY_V)
		{
			PSDatas->ParticleStatus = REPULSION;
		}
		// Toggle between mouse following and set gravity point modes.
		if (key == GLFW_KEY_SPACE)
		{
			if (PSDatas->FollowKeyPressed == false)
				PSDatas->FollowKeyPressed = true;
			else
				PSDatas->FollowKeyPressed = false;
		}
		// Speed modification
		if (key == GLFW_KEY_F)
		{
			if (PSDatas->Particle->Speed < MAX_PARTICLE_SPEED)
			{
				PSDatas->Particle->Speed += PARTICLE_SPEED_MODIFIER;
			}
		}
		else if (key == GLFW_KEY_G)
		{
			if (PSDatas->Particle->Speed > 0.0)
			{
				PSDatas->Particle->Speed -= PARTICLE_SPEED_MODIFIER;
				if (PSDatas->Particle->Speed < 0.0)
				{
					PSDatas->Particle->Speed = 0.0;
				}
			}
		}
		// Camera displacement.
		if (key == GLFW_KEY_Z || key == GLFW_KEY_W)
		{
			PSDatas->PressedDirection = UP;
		}
		else if (key == GLFW_KEY_A || key == GLFW_KEY_Q)
		{
			PSDatas->PressedDirection = LEFT;
		}
		else if (key == GLFW_KEY_S)
		{
			PSDatas->PressedDirection = DOWN;
		}
		else if (key == GLFW_KEY_D)
		{
			PSDatas->PressedDirection = RIGHT;
		}
	}
	// On key release.
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_C || key == GLFW_KEY_V)
		{
			PSDatas->ParticleStatus = ORBIT_DEFAULT;
		}
		// Camera displacement.
		if (key == GLFW_KEY_Z || key == GLFW_KEY_W
			|| key == GLFW_KEY_A || key == GLFW_KEY_Q
			|| key == GLFW_KEY_S
			|| key == GLFW_KEY_D)
		{
			PSDatas->PressedDirection = NONE;
		}
	}
	
}

void	MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	t_ParticleSystemDatas			*PSDatas;
	glm::vec3						mouseDisplacement;

	(void)mods;
	PSDatas = (t_ParticleSystemDatas *)glfwGetWindowUserPointer(window);
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			PSDatas->RightClickPressed = true;
			PSDatas->MousePosCamAnchor_x = PSDatas->World_fCursor_x;
			PSDatas->MousePosCamAnchor_y = PSDatas->World_fCursor_y;
		}
		else if (action == GLFW_RELEASE)
		{
			PSDatas->MousePosCamAnchor_x = 0.0;
			PSDatas->MousePosCamAnchor_y = 0.0;
			PSDatas->CamLookAt->x = 0.0;
			PSDatas->CamLookAt->y = 0.0;
			PSDatas->CamLookAt->z = 0.0;
			PSDatas->RightClickPressed = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			PSDatas->GravityPoint_x = PSDatas->World_fCursor_x;
			PSDatas->GravityPoint_y = PSDatas->World_fCursor_y;
		}
	}
}

void	UpdateCamera(t_ParticleSystemDatas *PSDatas)
{
	if (PSDatas->RightClickPressed == true)
	{
		PSDatas->CamLookAt->x = PSDatas->MousePosCamAnchor_x;
		PSDatas->CamLookAt->y = PSDatas->MousePosCamAnchor_y;
	}
	if (PSDatas->PressedDirection == UP)
	{
		if (PSDatas->Camera->Transform.Position.z < 0.0)
			PSDatas->Camera->Transform.Position.z += CAM_MOVE_SPEED;
	}
	if (PSDatas->PressedDirection == LEFT)
	{
		PSDatas->Camera->Transform.Position.x += CAM_MOVE_SPEED;
	}
	if (PSDatas->PressedDirection == RIGHT)
	{
		PSDatas->Camera->Transform.Position.x -= CAM_MOVE_SPEED;
	}
	if (PSDatas->PressedDirection == DOWN)
	{
		PSDatas->Camera->Transform.Position.z -= CAM_MOVE_SPEED;
	}
}

/*
**	From anton's opengl. works fine without camera movement.
*/

void	GetMouseWorldPosition(t_ParticleSystemDatas *PSDatas)
{
	glm::mat4 			mouse_unprojection_mat;
	glm::vec3			worldPos;
	glm::vec3			ray_nds;
	glm::vec4			ray_clip;
	glm::vec4			ray_eye;
	glm::vec3			ray_wor;
	static glm::vec3	zero_pos = glm::vec3(0.0,0.0,0.0);

	float x = (2.0f * (float)PSDatas->Cursor_x) / WINDOW_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * (float)PSDatas->Cursor_y) / WINDOW_HEIGHT;
	float z = 1.0f;
	ray_nds = glm::vec3(x, y, z);
	ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	ray_eye = glm::inverse(PSDatas->engine->MatPerspectiveProjection) * ray_clip;
	ray_wor = (glm::inverse(PSDatas->engine->MatView) * ray_eye);
	// don't forget to normalise the vector at some point
	// ray_wor = glm::normalize(ray_wor); // better precision without.
	worldPos = PSDatas->Camera->Transform.Position
				+ ray_wor * glm::distance(PSDatas->Camera->Transform.Position, zero_pos);
	PSDatas->World_fCursor_x = (float)worldPos.x;
	PSDatas->World_fCursor_y = (float)worldPos.y;
	// printf("Mouse world pos at %fx %fy\n", cursorWorldSpace_x, cursorWorldSpace_y);
}
