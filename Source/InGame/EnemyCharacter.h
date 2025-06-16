#pragma once
#include "Character.h"
#include "Stat.h"
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
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void adjustHealth(s32 Amount) override;

		Stat Stats;
	protected:
	private:
	};
}