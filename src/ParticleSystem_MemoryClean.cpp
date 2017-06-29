#include "../includes/ParticleSystem.hpp"

void		ParticleSystem_MemoryClean(EngineController *engine, t_ParticleSystemDatas *PSDatas)
{
	(void)PSDatas;
	engine->CLController.CleanMemory();
}
