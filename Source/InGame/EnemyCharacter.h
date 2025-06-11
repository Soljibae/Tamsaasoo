#pragma once
#include "Character.h"
namespace InGame
{
	class EnemyCharacter : public Character
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
	s32 Damage = 1;
	protected:
	private:
	};
}