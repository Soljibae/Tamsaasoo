#pragma once
#include "Character.h"
#include "Stat.h"
#include "EnemyData.h"
namespace InGame
{
	class EnemyCharacter : public Character
	{
	public:
		virtual void Init() override;
		void Spawn(AEVec2 Pos, EnemyData* InData);
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void adjustHealth(s32 Amount) override;
		void UpdateEffectTime();

		Stat Stats;
		EnemyType Type;
		u32 Exp = 1;
		f32 ProjectileSpawnTimer = 0.f;
		f32 ProjectileChamberTimer = 5.f;
		void SpawnProjectile(AEVec2 Dir, AEVec2 Pos);

		bool bHasDashed = false;
		bool bIsDashing = false;
		AEVec2 dashDirection = { 0.f, 0.f };
		AEVec2 dashStartPos = { 0.f, 0.f };
		float recoverTimer = 0.f;
		float dashDuration = 0.f;
	protected:
	private:
	};
}