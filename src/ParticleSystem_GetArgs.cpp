#include "../includes/ParticleSystem.hpp"

void		ParticleSystem_GetArgs(int argc, char **argv, t_ParticleSystemDatas *PSDatas)
{
	if (argc != 2)
	{
		std::cout << KRED "Usage: ./ParticleSystem [nb_particle]" KRESET << std::endl;
		exit(0);
	}
	else
	{
		if (strlen(argv[1]) > 10)
		{
			std::cout << KRED "Error: Invalid number of particles" KRESET << std::endl;
			exit(-1);
		}
		try
		{
			PSDatas->NbParticlesAsked = std::stoi(argv[1], NULL, 10);
		}
		catch (const std::invalid_argument& ia) {
	  		std::cerr << KRED "Error: Invalid argument: " KRESET << ia.what() << std::endl;
	  		std::cout << KRED "Usage: ./ParticleSystem [nb_particle]" KRESET << std::endl;
			exit(-1);
		}
		if (PSDatas->NbParticlesAsked < 1 || PSDatas->NbParticlesAsked > MAX_NB_PARTICLES)
		{
			std::cout << KRED "Error: Invalid number of particles" KRESET << std::endl;
			std::cout << KRED "Usage: ./ParticleSystem [nb_particle]" KRESET << std::endl;
			exit(-1);
		}
	}
}
