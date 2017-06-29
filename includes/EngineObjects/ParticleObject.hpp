# include "../../includes/ParticleSystem.hpp"

class ParticleObject : public EngineObject
{
	public:
		ParticleObject(int particleNumber, float rad);
		~ParticleObject();
	
		int				ParticleNumber;
		float			Radius;

		cl_mem			ObjMem;

	private:
		void initParticleObject();
};
