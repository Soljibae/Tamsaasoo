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