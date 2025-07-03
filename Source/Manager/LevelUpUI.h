#pragma once
#include "GameManager.h"
#include "../InGame/PlayerCharacter.h"
#include "Playing.h"
#include "../Utils/Utils.h"
#include "Button.h"
#include <vector>
#include <array>
namespace Manager
{
	class LevelUpUI : public InGame::Actor
	{
	public:
		void Init(InGame::PlayerCharacter* InPC);
		void Update() override;
		void Draw() override;
		void Destroy() override;
		bool IsActive() const;
		void Show();
		void Reroll(s8 thisbutton);
		std::vector<std::shared_ptr<InGame::Item>> GenerateRandomRewards();
	private:
		std::vector<std::shared_ptr<InGame::Item>> currentOptions;

		bool isActive = false;

		InGame::PlayerCharacter* PC;

		std::array<Button, 3> ItemWindow;
		static AEGfxVertexList* windowMesh;
		static AEGfxTexture* windowTexture;

		InGame::Actor pauseDimmer;

		std::array<Button, 3> rerollButton;
		static AEGfxVertexList* rerollMesh;
		static AEGfxTexture* rerollTexture;

		s32 rerollCost;
		std::array<Button, 3> ItemSlot;
	};
	extern LevelUpUI pickPanel;
}