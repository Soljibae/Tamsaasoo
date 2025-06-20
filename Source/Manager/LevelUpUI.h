#pragma once
#include "GameManager.h"
#include "../InGame/PlayerCharacter.h"
#include <vector>
#include "Playing.h"
#include "Button.h"
#include "../Utils/Utils.h"
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
		void Show(const std::vector<std::shared_ptr<InGame::Item>>&);
		std::vector<std::shared_ptr<InGame::Item>> GenerateRandomRewards();
	private:
		std::vector<std::shared_ptr<InGame::Item>> currentOptions;
		bool isActive = false;
		InGame::PlayerCharacter* PC;
		std::array<Button, 3> ItemWindow;
		static AEGfxVertexList* windowMesh;
		static AEGfxTexture* windowTexture;
		std::array<Button, 3> ItemSlot;
		static AEGfxVertexList* slotMesh;
		static AEGfxTexture* slotTexture;
	};
	extern LevelUpUI pickPanel;
}