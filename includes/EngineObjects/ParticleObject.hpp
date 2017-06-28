# include "../../includes/ParticleSystem.hpp"

class ParticleObject : public EngineObject
{
	public:
		ParticleObject(int particleNumber);
		~ParticleObject();
	
		int				ParticleNumber;

		cl_mem			ObjMem;

	private:
		void initParticleObject();
};