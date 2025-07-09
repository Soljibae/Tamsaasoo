#pragma once
#include "Character.h"
#include "Stat.h"
#include "EnemyData.h"
#include "ArealAttack.h"
namespace InGame
{
	enum class SniperState {
		APPROACHING,
		RETREATING,
		SHOOTING
	};
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

		/*--------ZIGZAG--------*/
		bool bIsZigzagMoving = false;
		AEVec2 MoveTargetDir;
		f32 ZigZagMaxTime = 0.f;
		f32 ZigZagTimer = 0.f;
		bool bZigZagLeft = true;
		/*--------ZIGZAG--------*/

		/*--------ORBITER--------*/
		bool bRotRight = true;
		float OrbitAngle = 0.f;
		float OrbitSpeed = 1.0f;
		float OrbitRadius = 300.f;
		const float OrbitIdealDistance = 250.f; // 완충 거리
		const float AngleAdjustDeg = 10.f; // 각도 보정 (inward angle)
		float OrbitShootTimer = 0.f;
		float OrbitShootInterval = 1.5f;
		/*--------ORBITER--------*/
		/*--------SNIPER--------*/
		SniperState sniperState = SniperState::APPROACHING;
		float SniperApproachDistance = 250.f;
		float SniperRetreatDistance = 600.f;

		float SniperShootTimer = 0.f;
		float SniperShootInterval = 2.5f;

		AEVec2 SniperRetreatDir;
		AEVec2 SniperRetreatStartPos;
		bool bRetreatDirInitialized = false;
		bool bIsWarned = false;
		/*--------SNIPER--------*/
		/*--------BURNER--------*/
		float FlameZoneTimer = 0.f;
		float FlameZoneInterval = 4.0f;
		float FlameZoneDuration = 3.0f;
		AEVec2 FlameZonePos;
		float FlameZoneRadius = 200.f;
		/*--------BURNER--------*/
		/*--------HOLER--------*/
		float BlackHoleSpawnTimer = 0.f;
		float BlackHoleSpawnInterval = 10.0f;
		float BlackHoleDuration = 13.0f;
		AEVec2 BlackHolePos;
		float BlackHoleRadius = 400.f;
		/*--------HOLER--------*/
	protected:
	private:
	};
}