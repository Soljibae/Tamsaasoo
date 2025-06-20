#pragma once
#include "Character.h"
#include "Gun.h"
#include "Item.h"
#include "PlayerStat.h"
#include <vector>
#include <memory>
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
		void UpdateMovement();
		void UpdateDash();
		PlayerStat Stats;
		void UpdateKill(u32 Exp);
		// first: item / second: count
		std::vector<std::pair<std::unique_ptr<Item>, s16>> inventory;
		void AddItemToInventory(std::unique_ptr<Item> item);
		
	protected:
		AEVec2 MouseDirection;
		Gun* HoldingGun = nullptr;

		bool bIsInvincible = false;
		f32 InvincibleTimer = 0.f;


		bool bIsDashing = false;
		float DashSpeed = 800.0f;
		float DashTime = 0.2f;
		float DashTimer = 0.0f;
		float DashCooldown = 1.0f;
		float DashCooldownTimer = 0.0f;
		AEVec2 DashDirection;

	private:
	};
}