#pragma once
#include "Character.h"
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
		EnemyType Type;
		s32 Damage = 1;
		u32 Exp = 1;
	protected:
	private:
	};
}