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
		float			Angle_val;

		cl_mem			ObjMem; // using base class's vao/vbo

		GLuint			Distance_Vao;
		GLuint			Distance_Vbo;
		cl_mem			Distance_ObjMem;

	private:
		void initParticleObject();
};
