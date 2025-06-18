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
}