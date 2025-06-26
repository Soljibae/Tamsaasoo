#pragma once
#include "AEEngine.h"
#include <map>

namespace InGame
{
	enum EStatusEffect
	{
		BURN,
		STUN,
		SLOW,
		FEAR,
		VULNERABLE,
	};
	class Stat
	{
	public:
		virtual void Init();
		virtual void Update();
		virtual void Draw();
		virtual void Destroy();

		std::map<EStatusEffect, f32> StatusEffectTimer;
		f32 MaxHP;
		f32 HP;
		f32 MovementSpeed;
		f32 FireRate;
		AEVec2 ProjectileSize;
		f32 ProjectileSpeed;
		f32 ProjectileCollisionSize;
		f32 Damage;
	};
}
