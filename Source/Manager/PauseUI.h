#pragma once
#include "../InGame/Actor.h"
#include "Button.h"
#include "Playing.h"
#include "../Utils/Utils.h"
#include "../InGame/PlayerCharacter.h"
#include <array>
#include <unordered_map>
namespace Manager
{
	// Pause menu
	class PauseUI : public InGame::Actor
	{
	public:
		void Init(InGame::PlayerCharacter* InPC);
		void Update();
		void Draw();
		void Destroy();
	private:
		//resume button
		Button resumeButton;
		//return to mainmenu button
		Button mainmenuButton;
		//pause screen black background image
		InGame::Actor pauseDimmer;
		InGame::Actor tagUI;
		InGame::Actor statsUI;
		std::vector<std::string> statsString;
		std::vector<std::string> baseStatsString;
		std::unordered_map<InGame::ItemTag, std::string> tagString;
		//pointer to PC in playing
		InGame::PlayerCharacter* PC;
		std::array<Button, 16> ItemSlot;
		static AEGfxVertexList* slotMesh;
		static AEGfxTexture* slotTexture;
		//white image to rendered at mouse over on itemslot
		InGame::Actor slotWhite;
		s8 pFont;
	};
	extern PauseUI pausePanel;
}