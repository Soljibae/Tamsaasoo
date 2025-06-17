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
		void UpdateKill(u32 Exp);
	protected:
		AEVec2 MouseDirection;
		Gun* HoldingGun = nullptr;

		bool bIsInvincible = false;
		f32 InvincibleTimer = 0.f;
	private:
	};
}