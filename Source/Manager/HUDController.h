#pragma once
#include "Playing.h"
#include <vector>
namespace Manager
{
	class HUDController : public InGame::Actor
	{
	public:
		void Init(InGame::PlayerCharacter* InPC, InGame::Gun* InGUN);
		void Update();
		void Draw();
		void Destroy();
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
		s8 pFont;
	};
	
	extern HUDController HUD;
}