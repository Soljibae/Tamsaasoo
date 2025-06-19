#pragma once
#include "AEEngine.h"
namespace InGame
{
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
	};
}
