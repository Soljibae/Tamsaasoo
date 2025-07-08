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
		f32 MaxHP = 0.f;
		f32 HP = 0.f;
		f32 MovementSpeed = 0.f;
		f32 FireRate = 0.f;
		AEVec2 ProjectileSize = {0.f, 0.f};
		f32 ProjectileSpeed = 0.f;
		f32 ProjectileCollisionSize = 0.f;
		f32 Damage = 0.f;
	};
}
