#pragma once
#include "EnemyCharacter.h"
#include <string>
#include <vector>
namespace InGame
{
	class EnemyBoss : public EnemyCharacter
	{
	public:
		virtual ~EnemyBoss() {};
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void OnPlayerHit();
		std::string name, epithet;
	protected:
		
	private:
	};

	class Stage1Boss : public EnemyBoss
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;

		f32 WaveAttackSpawnTimer = 0.f;
		f32 WaveAttackChamberTimer = 12.f;
		
		bool bIsWaving = false;
	protected:

	private:
	};

	class Stage2Boss : public EnemyBoss
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void OnPlayerHit() override;
	protected:
		AEVec2 dashDirection;
		AEVec2 dashStartPos;
		float dashCooldown = 1.5f;
		float dashTimer = 0.f;
		float dashRange = 500.f;
		float dashSpeed = 1200.f;
		bool bIsCharging = false;
		
		float cooldownTime = 0.f;

		float radialAttackTimer = 0.f;
		float radialAttackCooldown = 2.f;
		bool bDoRadialAttack = false;

		int dashCount = 0;
		bool bIsJumping = false;
		float jumpTimer = 0.f;
		float jumpDuration = 2.0f;
		AEVec2 jumpTargetPos;
		float DrawAlpha = 1.f;
		float jumpFadeOutDuration = 1.0f;  
		float jumpFadeInDuration = 0.5f;   
		float jumpMoveDuration = 2.0f;     

		AEVec2 jumpStartPos;
	private:
	};
	class Stage3Boss : public EnemyBoss
	{
	public:
		virtual ~Stage3Boss() {};
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void OnPlayerHit() override;
		void SpawnBomb(AEVec2& pos);
		void SpawnBlackHole(AEVec2& pos);
	protected:
		//Bomb 
		bool bBombWarned = false;
		f32 BombTimer = 0.f;
		f32 BombSpawnInterval = 2.f;
		f32 BombRadius = 200.f;
		f32 BombDelay = 3.f;
		//BlackHole
		bool bBlackHoleWarned = false;
		float BlackholeTimer = 0.f;
		float BlackholeInterval = 5.f;
		float BlackholeDuration = 10.f;
		AEVec2 BlackholePosition;
		float BlackholeRadius = 300.f;
		//Laser
		f32 LaserTimer = 0.f;
		f32 LaserSpawnInterval = 7.f;
		f32 LaserSpawnMaxTime = 12.f;
		bool bLaserWarned = false;
		bool bUseEightDirections = true;
		bool bLaserRotRight = true;
		bool bUseRotate = true;

		//Projectile
		f32 ProjectileSpawnTimer = 0.f;
		f32 ProjectileChamberTimer = 1.5f;
		f32 WaveAttackSpawnTimer = 0.f;
		bool angleOffsetToggle = false;
		f32 WaveAttackChamberTimer = 30.f;
		bool bIsWaving = false;

		//Movement
		AEVec2 MoveStartPos;
		AEVec2 MoveTargetPos;
		bool bIsMoving = false;
		float MoveTimer = 0.f;
		float MoveCooldown = 5.f;
		std::vector<AEVec2> MovePositions;
	private:
	};
}