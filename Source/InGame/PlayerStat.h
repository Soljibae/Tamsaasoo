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

		u32 Level;
		u32 ExpCount;
		f32 ExpGained;
		s32 HitCount;
		u32 TargetExp;
		u32 Money;
		s16 Potion;
		s32 ReviveCount;
		f32 rerollCostRatio;
		f32 BurnDamage;
		f32 BurnRate;

		f32 effectiveDamage;
		f32 effectiveFireRate;
		f32 effectiveExpGained;
		s32 effectiveHitCount;
		f32 effectiveMovementSpeed;
	};
}
