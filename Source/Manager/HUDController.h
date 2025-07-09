#pragma once
#include "Playing.h"
#include <vector>
#include <array>
namespace Manager
{
	class HUDController
	{
	public:
		void Init(InGame::PlayerCharacter* InPC, InGame::Gun* InGUN);
		void Update();
		void Draw();
		void TooltipUpdate(InGame::Item& item);
		void ShowTooltip(InGame::Item& item);
		void Destroy();
		std::vector<std::string> SplitTextIntoLines(const std::string& text, f32 maxWidth);
	private:
		InGame::PlayerCharacter* PC;
		InGame::Gun* GUN;
		//HP HUD
		std::vector<InGame::Actor> HP;
		//HPBackGround HUD
		std::vector<InGame::Actor> HPBG;
		static AEGfxVertexList* HPMesh;
		static AEGfxTexture* HPTex;
		static AEGfxTexture* HPBGTex;

		InGame::Actor Coin;

		InGame::Actor ChamberTimeBar;
		InGame::Actor fireTimeBar;
		
		InGame::Actor Potion;
		InGame::Actor PotionBG;

		InGame::GunType prevGunType;
		s16 prevPotion;
		s32 prevMaxHP;
		f32 prevCurrentHP;
		f32 prevFireRate;
		static InGame::Item* prevItem;
		s8 pFont;
		std::vector<std::string> itemDesc;

		InGame::Actor Vignetting;
	};

	static struct Tooltip
	{
		InGame::Actor Window;
		std::array<AEGfxVertexList*, 9> WindowMesh;
	};
	extern HUDController HUD;
}