# include "../../includes/ParticleSystem.hpp"

class ParticleObject : public EngineObject
{
	public:
		ParticleObject(int particleNumber);
		~ParticleObject();
	
		int				ParticleNumber;

		cl_mem			ObjMem;
		std::vector<glm::vec4> pos; // 

	private:
		void initParticleObject();
};