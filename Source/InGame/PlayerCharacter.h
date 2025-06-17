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
		
		PlayerStat Stats;
		void UpdateKill(u32 Exp);

		std::vector<std::unique_ptr<Item>> inventory;
		void AddItemToInventory(std::unique_ptr<Item> item);
	protected:
		AEVec2 MouseDirection;
		Gun* HoldingGun = nullptr;
	private:
	};
}