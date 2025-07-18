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
		//Button resumeButton;
		//Button settingButton;
		//Button mainmenuButton;
		InGame::Actor Wbutton;
		AEGfxVertexList* buttonMesh;
		AEGfxTexture* BbuttonTexture;
		std::array<Button, 3> Buttons;
		f32 buttonAlpha{ 0.3f };
		//pause screen black background image
		InGame::Actor pauseDimmer;
		InGame::Actor tagUI;
		InGame::Actor statsUI;
		std::vector<std::string> statsString;
		std::vector<std::string> baseStatsString;
		std::unordered_map<InGame::ItemTag, std::string> tagString;
		//pointer to PC in playing
		InGame::PlayerCharacter* PC;
		std::array<Button, 32> ItemSlot;
		static AEGfxVertexList* slotMesh;
		static AEGfxTexture* slotTexture;
		//white image to rendered at mouse over on itemslot
		InGame::Actor slotWhite;
		s8 pFont;
	};
	extern PauseUI pausePanel;
}