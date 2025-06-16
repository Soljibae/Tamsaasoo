#pragma once
#include "Character.h"
#include "Gun.h"
#include "PlayerStat.h"
namespace InGame
{
	class PlayerCharacter : public Character
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void adjustHealth(s32 Amount) override;
		void GetMouseDir();

		PlayerStat Stats;
	protected:
		AEVec2 MouseDirection;
		Gun* HoldingGun = nullptr;
	private:
	};
}