#include "../includes/ParticleSystem.hpp"

int		main(int argc, char **argv)
{
	EngineController			*Engine = &EngineController::Instance();
	t_ParticleSystemDatas		PSDatas;

	PSDatas.engine = Engine;
	try
	{
		ParticleSystem_GetArgs(argc, argv, &PSDatas);
		ParticleSystem_Init(Engine, &PSDatas);
		ParticleSystem_SceneInit(Engine, &PSDatas);
		ParticleSystem_MainLoop(Engine, &PSDatas);
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception!" << std::endl;
		exit(-1);
	}
	return 0;
}
