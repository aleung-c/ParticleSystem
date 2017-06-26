#ifndef GAMEUIOBJECT_HPP
# define GAMEUIOBJECT_HPP

#include "../ParticleSystem.hpp"

class GameUIObject : public EngineObject
{
	public:
		GameUIObject(std::string name, std::string imgPath);
		GameUIObject(std::string name);
		~GameUIObject();

	private:
		void							initValues();
		void							setBuffers();
};

#endif