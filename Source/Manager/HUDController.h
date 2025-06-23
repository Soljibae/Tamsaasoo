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
		s32 MaxHP;
		s32 currentHP;
		f32 fireTimer;
	};
	
	extern HUDController HUD;
}