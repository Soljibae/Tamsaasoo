#pragma once
#include "GameManager.h"
#include "../InGame/PlayerCharacter.h"
#include "Playing.h"
#include "../Utils/Utils.h"
#include "Button.h"
#include <vector>
#include <array>
#include <map>
#include "../Manager/ItemData.h"

namespace Manager
{
	class LevelUpUI : public InGame::Actor
	{
	public:
		void ResetGotEpic();
		void Init(InGame::PlayerCharacter* InPC);
		void Update() override;
		void Draw() override;
		void Destroy() override;
		bool IsActive() const;
		void Show();
		void Reroll(s8 thisbutton);
		std::vector<std::shared_ptr<InGame::Item>> GenerateRandomRewards();
	private:
		f32 pickDelay{ 1.f };
		f32 delayTime{ 0.f };
		std::vector<std::shared_ptr<InGame::Item>> currentOptions;

		bool isActive = false;
		
		bool gotEpic = false;
		InGame::Actor epicWhite;
		const f32 resetAlpha{ 0.3f };
		f32 epicAlpha{ resetAlpha };
		InGame::PlayerCharacter* PC;

		std::array<Button, 3> ItemWindow;
		std::array<AEGfxVertexList*, 9> windowMesh;
		static AEGfxTexture* windowTexture;

		std::array<InGame::Actor, 3> ItemSlot;
		std::array<AEGfxVertexList*, 9> ItemSlotMesh;
		static AEGfxTexture* ItemSlotTexture;

		InGame::Actor pauseDimmer;

		std::array<Button, 3> rerollButton;
		static AEGfxVertexList* rerollMesh;
		static AEGfxTexture* rerollTexture;

		std::array<InGame::Actor, 3> rerollCostIcon;
		AEGfxVertexList* rerollCostMesh;
		AEGfxTexture* rerollCostTexture;
		std::array<s32, 3> rerollCost;
		s8 pFont;
		std::map<ItemGrade, std::vector<s32>> containerForGrade;
	};
	extern LevelUpUI pickPanel;
}