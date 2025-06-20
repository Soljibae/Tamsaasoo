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
		~HUDController() { if (Mesh)AEGfxMeshFree(Mesh); };
	private:
		InGame::PlayerCharacter* PC;
		InGame::Gun* GUN;
		//HP HUD
		std::vector<InGame::Actor> HP;
		//HPBackGround HUD
		std::vector<InGame::Actor> HPBG;
		static AEGfxTexture* HPTexture;
		static AEGfxTexture* HPBGTexture;
		s32 MaxHP;
		s32 currentHP;
		f32 fireTimer;
	};
	
	extern HUDController HUD;
}