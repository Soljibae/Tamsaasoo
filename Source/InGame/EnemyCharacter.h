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

		Stat Stats;
		EnemyType Type;
		u32 Exp = 1;
	protected:
	private:
	};
}