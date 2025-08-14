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
		void ShowStageUpdate();
		void ShowStageDraw();
		void Destroy();
		std::vector<std::string> SplitTextIntoLines(const std::string& text, f32 maxWidth);
		std::vector<std::string> SplitTextIntoLines_UTF8_KR(const std::string& textUTF8, float maxWidthPx, float scale, bool useKerning = true);
		InGame::Actor Coin;
		InGame::Actor* GetPotion();
		f32 PotionFillPercent = 0.f;
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


		InGame::Actor ChamberTimeBar;
		InGame::Actor fireTimeBar;
		InGame::Actor ammoType;

		InGame::Actor Potion;
		InGame::Actor PotionBG;
		InGame::Actor PotionFull;

		InGame::GunType prevGunType;
		s16 prevPotion;
		s32 prevMaxHP;
		f32 prevCurrentHP;
		f32 prevFireRate;
		static InGame::Item* prevItem;
		s8 pFont;
		std::vector<std::string> itemDesc;

		InGame::Actor stageBG;
		InGame::Actor stageMAP;
		InGame::Actor stageArrow;
		std::array<AEGfxVertexList*, 9> bgMesh;
		AEGfxVertexList* m_Mesh;
		AEGfxTexture* stageBGTexture;
		AEGfxTexture* stageMAPTexture;
		
		InGame::Actor Vignetting;
	};

	static struct Tooltip
	{
		InGame::Actor Window;
		std::array<AEGfxVertexList*, 9> WindowMesh;
	};
	extern HUDController HUD;
}