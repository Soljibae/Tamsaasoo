#pragma once
#include "AEEngine.h"
#include <map>
namespace InGame
{
	enum EAnimationState
	{
		NONE,
		IDLE,
		MOVE,
		DEAD,
		JUMP,
	};
	class Stat
	{
	public:
		virtual void Init();
		virtual void Update();
		virtual void Draw();
		virtual void Destroy();

		s32 HP;
		f32 MovementSpeed;
		f32 FireRate;
		f32 BulletSpeed;
		s32 Damage;
		EAnimationState AnimationState;
		s32 AnimationCount;
		std::map<EAnimationState, s32> MaxAnimationCount;
	};
}
