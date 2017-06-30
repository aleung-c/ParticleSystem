#include "../includes/ParticleSystem.hpp"

int		main(void)
{
	EngineController			*Engine = &EngineController::Instance();
	t_ParticleSystemDatas		PSDatas;

	std::cout << "Hello particle system!" << std::endl;

	PSDatas.engine = Engine;
	ParticleSystem_Init(Engine, &PSDatas);

	ParticleSystem_SceneInit(Engine, &PSDatas);
	
	ParticleSystem_MainLoop(Engine, &PSDatas);

	ParticleSystem_MemoryClean(Engine, &PSDatas);

	return 0;
}
