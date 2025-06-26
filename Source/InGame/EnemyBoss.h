#pragma once
#include "EnemyCharacter.h"

namespace InGame
{
	class EnemyBoss : public EnemyCharacter
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void OnPlayerHit();
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
		float jumpFadeOutDuration = 1.0f;  // 사라지는 시간
		float jumpFadeInDuration = 0.5f;   // 다시 나타나는 시간
		float jumpMoveDuration = 2.0f;     // 점프 전체 시간 (= 점프 거리 이동 시간)

		AEVec2 jumpStartPos;
	private:
	};
	class Stage3Boss : public EnemyBoss
	{
	public:
		//virtual void Init() override;
		//virtual void Update() override;
		//virtual void Draw() override;
		//virtual void Destroy() override;

		//f32 WaveAttackSpawnTimer = 0.f;
		//f32 WaveAttackChamberTimer = 12.f;
		//bool bIsWaving = false;
	protected:

	private:
	};
}