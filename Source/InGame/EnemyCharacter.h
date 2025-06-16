#pragma once
#include "Character.h"
namespace InGame
{
	enum EnemyType
	{
		Minion,
	};

	class EnemyCharacter : public Character
	{
	public:
		virtual void Init() override;
		void Spawn(AEVec2 Pos);
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
	s32 Damage = 1;
	u32 Exp = 1;
	protected:
	private:
	};
}