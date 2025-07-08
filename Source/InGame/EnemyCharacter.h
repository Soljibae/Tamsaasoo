#pragma once
#include "Character.h"
#include "Stat.h"
#include "EnemyData.h"
namespace InGame
{
	class EnemyCharacter : public Character
	{
	public:
		virtual ~EnemyCharacter() {};
		virtual void Init() override;
		void Spawn(AEVec2 Pos, EnemyData* InData);
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void adjustHealth(f32 Amount) override;
		void UpdateEffectTime();

		EnemyType Type;
		u32 Exp = 1;
		f32 ProjectileSpawnTimer = 0.f;
		f32 ProjectileChamberTimer = 3.f;
		f32 BurnTimer = 0.f;
		void SpawnProjectile(AEVec2 Dir, AEVec2 Pos);

		/*--------DASHER--------*/
		bool bHasDashed = false;
		bool bIsDashing = false;
		AEVec2 dashDirection = { 0.f, 0.f };
		AEVec2 dashStartPos = { 0.f, 0.f };
		float recoverTimer = 0.f;
		float dashDuration = 0.f;
		/*--------DASHER--------*/

		/*--------BOMBER--------*/
		bool isDetonating = false;
		float detonationTimer = 0.f;
		float detonationDelay = 2.0f;
		float explosionRadius = 150.f;
		int explosionDamage = 4;
		/*--------BOMBER--------*/
	protected:
	private:
	};
}