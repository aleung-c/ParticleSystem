#ifndef MORPHTARGETANIMATION_HPP
# define MORPHTARGETANIMATION_HPP

#include "../ParticleSystem.hpp"

/*
**	Class nested in GameObject handling the morph targeting.
**	Implemented as a side component of the GameObject.
*/

class MorphTargetAnimation
{
	public:
		MorphTargetAnimation();
		~MorphTargetAnimation();

		std::vector<GameObject *>		KeyFrames;

		int								Frame_i;
		GameObject						*StartFrame;
		GameObject						*CurFrame;
		GameObject						*NextFrame;

		void							AddKeyFrame(GameObject *obj);
		void							ClearFrames();

		// ----- Animation control methods
		void							SetSpeed(float newSpeed);
		void							Start();
		void							Update();
		void							Stop();

		// ----- Accessors / animation settings
		float							GetCurTime();
		void							SetCurTime(float newTime);

		void							SetRepeat(bool val);
		void							SetStayOnEnd(bool val);

		bool							IsRepeating();
		bool							HasStarted();
		bool							IsAnimated();
		bool							StayOnEnd();

	private:
		bool							_isMorphAnimated;
		float							_animCurTime;
		float							_animSpeed;
		bool							_started;
		bool							_repeat;
		bool							_stayOnEnd;
};

#endif
