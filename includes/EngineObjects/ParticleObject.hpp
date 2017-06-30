# include "../../includes/ParticleSystem.hpp"

class ParticleObject : public EngineObject
{
	public:
		ParticleObject(int particleNumber, float rad);
		~ParticleObject();
	
		int				ParticleNumber;
		float			Radius;
		float			Speed;
		float			Time;

		cl_mem			ObjMem;

	private:
		void initParticleObject();
};
