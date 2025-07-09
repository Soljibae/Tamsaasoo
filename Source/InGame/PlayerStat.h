#pragma once
#include "AEEngine.h"
#include "Stat.h"

namespace InGame
{
	class PlayerStat : public Stat
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;

		u32 Level = 0;
		u32 ExpCount = 0;
		f32 ExpGained = 0.f;
		s32 HitCount = 0;
		u32 TargetExp = 0;
		f64 Money = 0.f;
		s16 Potion = 0;
		s32 ReviveCount = 0;
		f32 BurnDamage = 0.f;
		f32 BurnRate = 0.f;

		f32 effectiveDamage = 0.f;
		f32 effectiveFireRate = 0.f;
		f32 effectiveExpGained = 0.f;
		s32 effectiveHitCount = 0;
		f32 effectiveMovementSpeed = 0.f;
	};
}
