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
	struct GunStat;

	enum StatsForUI {
		DAMAGE,
		FIRE_RATE,
		MOVEMENT_SPEED,
		HIT_COUNT,
		BURN_DAMAGE,
		BURN_RATE,
		EXP_GAINED,
		GOLD_GAINED,
		PROC_CHANCE,
		MINION_DAMAGE,
		HP,
		DAMAGE_RATIO,
		FIRE_RATE_RATIO,
		PROJECTILE_COUNT,
		STAT_LAST
	};
	// Pause menu
	class PauseUI : public InGame::Actor
	{
	public:
		void Init(InGame::PlayerCharacter* InPC);
		void InitGunStats();
		void Update();
		void Draw();
		void Destroy();
		bool isActive{ false };
		bool tapPressed{ false };
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
		std::map<StatsForUI, std::string> statsNameKR;
		std::vector<std::string> baseStatsString;
		std::unordered_map<InGame::ItemTag, std::string> tagString;
		std::unordered_map<InGame::ItemTag, std::string> tagStringColorPart;
		//pointer to PC in playing
		InGame::PlayerCharacter* PC;
		std::array<Button, 32> ItemSlot;
		static AEGfxVertexList* slotMesh;
		static AEGfxTexture* slotTexture;
		//white image to rendered at mouse over on itemslot
		InGame::Actor slotWhite;
		s8 pFont;
		void UpdateGun();
		void DrawGunName();
		std::string m_gunName;
		InGame::Actor m_gunIcon;
		Button m_gunSlot;
		bool shouldShowGunName;
		InGame::GunType prevGunType;
		std::map<InGame::GunType, GunStat> gunStats;
	};
	extern PauseUI pausePanel;
}