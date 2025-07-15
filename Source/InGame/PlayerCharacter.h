#pragma once
#include "Character.h"
#include "EnemyCharacter.h"
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
		virtual void adjustHealth(f32 Amount) override;
		void GetMouseDir();
		void UpdateMovement();
		void UpdateDash();
		PlayerStat* PS = nullptr;
		void UpdateKill(u32 Exp);
		// first: item / second: count
		std::vector<std::pair<std::shared_ptr<Item>, s16>> inventory;
		void AddItemToInventory(std::shared_ptr<Item> item);

		Gun* HoldingGun = nullptr;
		GunStruct* GunData = nullptr;

		void OnProjectileHit(InGame::EnemyCharacter* target, bool isTargetBoss);
		void OnDamaged();
		void UpdateEffectTime();
		void InitAdditionalStats();
		void UpdateStats();
		
		bool IsRevivable();
		bool IsPlayerInvincible();
		void SetPlayerInvincible();

		bool bIsDashing = false;
		bool bisCoinDroped = false;
	protected:
		AEVec2 MouseDirection;

		bool bIsInvincible = false;
		f32 InvincibleTimer = 0.f;


		
		float DashSpeed = 550.0f;
		float DashTime = 0.5f;
		float DashTimer = 0.0f;
		float DashCooldown = 1.0f;
		float DashCooldownTimer = 0.0f;
		AEVec2 DashDirection;

	private:
	};
}