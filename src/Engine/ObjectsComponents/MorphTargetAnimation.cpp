#include "../../../includes/ParticleSystem.hpp"

/*
** The file for the GameObject's morph targeting animation methods.
**	Class nested in GameObject handling the morph targeting.
**	Implemented as a side component of the GameObject.
**
**	The whole usage is still a bit janky, and would get a lot 
**	to be refactored, especially concerning animation state changing.
*/

MorphTargetAnimation::MorphTargetAnimation()
:
	StartFrame(NULL),
	CurFrame(NULL),
	NextFrame(NULL),
	_isMorphAnimated(false),
	_animCurTime(0.0),
	_animSpeed(0.01),
	_started(false),
	_repeat(false),
	_stayOnEnd(false)
{
	if (_started)
	{}
}

MorphTargetAnimation::~MorphTargetAnimation()
{

}

/*
**	Main method to create morph animations: for an GameObject, add a keyframe by
**	giving it another GameObject's address, with its model loaded.
*/

void				MorphTargetAnimation::AddKeyFrame(GameObject *obj)
{
	KeyFrames.push_back(obj);
	// First Call initialization.
	if (_isMorphAnimated == false)
	{
		Frame_i = 0;
		_isMorphAnimated = true; // flag for the engine's drawing.
		CurFrame = StartFrame;
		NextFrame = obj;
	}
}

void				MorphTargetAnimation::ClearFrames()
{
	KeyFrames.clear();
	_isMorphAnimated = false;
	_started = false;
	_repeat = false;
}

// --------------------------------------------------------------------	//
//																		//
//	Morph target animation control										//
//																		//
// --------------------------------------------------------------------	//

/*
**	Set the speed of the animation interpolation.
**	Knowing that an animation frame start at 0.0 and end at 1.0,
**	values should not be too high as it will go super fast.
*/

void				MorphTargetAnimation::SetSpeed(float newSpeed)
{
	if (newSpeed >= 0.0)
	{
		_animSpeed = newSpeed;
	}
	else
	{
		_animSpeed = 0.0;
	}
}

/*
**	This method will make the current key frame animation at the value given;
**	Useful for reseting the current interpolation.
**	Must be between 0.0 and 1.0.
*/

void				MorphTargetAnimation::SetCurTime(float newTime)
{
	if (newTime <= 1.0 && newTime >= 0.0)
	{
		_animCurTime = newTime;
	}
}

void				MorphTargetAnimation::Start()
{
	_started = true;
}

/*
**	Method to update the playing animations. Launched by default
**	during the engine's drawing.
**	This will transition from the StartFrame to the last element
**	in the KeyFrames vector, then go back to the first frame and
**	stop or replay.
*/


void				MorphTargetAnimation::Update()
{
	if (_started == true)
	{
		_animCurTime += _animSpeed;
	}
	// When current NextFrame is reached
	if (_animCurTime >= 1.0)
	{
		CurFrame = KeyFrames[Frame_i];
		Frame_i += 1;
		// Set the animation to go to the next key frame
		if ((int)KeyFrames.size() - 1 >= Frame_i)
		{
			NextFrame = KeyFrames[Frame_i];
			_animCurTime = 0.0;
		}
		else
		{
			// last frame reached
			CurFrame = StartFrame;
			if (_repeat == true)
			{
				_animCurTime = 0.0;
				Frame_i = 0;
				NextFrame = KeyFrames[Frame_i];
			}
			else
			{
				Stop();
				if (_stayOnEnd == true)
				{
					CurFrame = KeyFrames.back();
				}
			}
		}
	}
}

void				MorphTargetAnimation::Stop()
{
	_started = false;
	_animCurTime = 0.0;
}

/*
**	Accessors / animation settings
*/

float				MorphTargetAnimation::GetCurTime()
{
	return (_animCurTime);
}

/*
**	If this is at TRUE, the animation will repeat itself at its end indefinitly.
*/

void				MorphTargetAnimation::SetRepeat(bool val)
{
	_repeat = val;
}

/*
**	If this is at TRUE and _repeat is FALSE, the animation will block on its last
**	key frame when the end is reached.
*/

void				MorphTargetAnimation::SetStayOnEnd(bool val)
{
	_stayOnEnd = val;
}

bool				MorphTargetAnimation::IsRepeating()
{
	return (_repeat);
}

bool				MorphTargetAnimation::HasStarted()
{
	return (_started);
}

bool				MorphTargetAnimation::IsAnimated()
{
	return (_isMorphAnimated);
}

bool				MorphTargetAnimation::StayOnEnd()
{
	return (_stayOnEnd);
}
