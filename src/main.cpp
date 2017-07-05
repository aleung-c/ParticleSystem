#include "../includes/ParticleSystem.hpp"

int		main(int argc, char **argv)
{
	EngineController			*Engine = &EngineController::Instance();
	t_ParticleSystemDatas		PSDatas;

	PSDatas.engine = Engine;

	ParticleSystem_GetArgs(argc, argv, &PSDatas);

	ParticleSystem_Init(Engine, &PSDatas);

	ParticleSystem_SceneInit(Engine, &PSDatas);
	
	ParticleSystem_MainLoop(Engine, &PSDatas);

	ParticleSystem_MemoryClean(Engine, &PSDatas);

	return 0;
}
