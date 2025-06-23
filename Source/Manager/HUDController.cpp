#include "HUDController.h"
#include "../InGame/Gun.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include <iostream>

namespace Manager
{
	HUDController HUD;
	AEGfxVertexList* HUDController::HPMesh = nullptr;
	AEGfxTexture* HUDController::HPTex = nullptr;
	AEGfxTexture* HUDController::HPBGTex = nullptr;
	AEGfxVertexList* MChamberTime{ nullptr };
	AEGfxTexture* TChamberTime{ nullptr };
	AEGfxVertexList* MfireTimer{ nullptr };
	AEGfxTexture* TfireTimer{ nullptr };

	void HUDController::Init(InGame::PlayerCharacter* InPC, InGame::Gun* InGUN)
	{
		PC = InPC;
		GUN = InGUN;
		MaxHP = PC->Stats.MaxHP;
		currentHP = PC->Stats.HP;
		int w = global::ScreenWidth;
		int h = global::ScreenHeight;
		const float actorWidth = 60.f;
		const float actorHeight = 60.f;
		const float spacingX = 15.0f; // 가로 간격
		const float startX = -(w / 2) + 100.f;
		const float Y = (h / 2) - 100.f;
		
		HPMesh = Utils::CreateMesh();
		HPTex = AEGfxTextureLoad("Assets/HPBG.png");
		//HPBG HUD init
		InGame::Actor bgobj;
		bgobj.Texture = HPTex;
		bgobj.Mesh = HPMesh;
		for (int i = 0; i < MaxHP; ++i)
		{
			int row = i;
			int col = i;
			bgobj.position = { startX + col * (actorWidth + spacingX), Y };
			bgobj.size = { actorWidth, actorHeight };
			HPBG.push_back(bgobj);
		}
		//HP HUD init
		HPBGTex = AEGfxTextureLoad("Assets/HP.png");
		InGame::Actor hpobj;
		hpobj.Texture = HPBGTex;
		hpobj.Mesh = HPMesh;
		for (int i = 0; i < currentHP; i++)
		{
			hpobj.position = HPBG[i].position;
			hpobj.size = { actorWidth, actorHeight };
			HP.push_back(hpobj);
		}
		prevGunType = GUN->gunType;
		ChamberTimeBar.Mesh = Utils::CreateMesh();
		ChamberTimeBar.Texture = AEGfxTextureLoad("Assets/square.png");
		fireTimeBar.Mesh = Utils::CreateMesh();
		fireTimeBar.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
		ChamberTimeBar.position = { 0.f, -30.f };
		ChamberTimeBar.size = { 50.f / GUN->RoundPerSec, 4.f };
		fireTimeBar.position.x = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		fireTimeBar.position.y = ChamberTimeBar.position.y;
		fireTimeBar.size = { 5.f, 15.f };

		f32 fillPercent = GUN->RoundPerSec;
		if (fillPercent > 1.f) fillPercent = 1.f;
		f32 barStartX = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		f32 barEndX = ChamberTimeBar.position.x + ChamberTimeBar.size.x / 2.f;
		fireTimeBar.MovementSpeed = fillPercent * (barEndX - barStartX) * global::DeltaTime;

	}
	void HUDController::Update()
	{
		if (global::KeyInput(AEVK_F))
		{
			switch (GUN->gunType)
			{
			case InGame::GunType::PISTOL:
				GUN->gunType = InGame::GunType::RIFLE;
				break;
			case InGame::GunType::RIFLE:
				GUN->gunType = InGame::GunType::SHOTGUN;
				break;
			case InGame::GunType::SHOTGUN:
				GUN->gunType = InGame::GunType::PISTOL;
				break;
			}
		}
		while (currentHP > PC->Stats.HP)
		{
			if(!HP.empty())
				HP.pop_back();
			currentHP--;
		}
		currentHP = PC->Stats.HP;
		if (fireTimeBar.position.x > ChamberTimeBar.position.x + ChamberTimeBar.size.x / 2.f)
		{
			fireTimeBar.position.x = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		}

		f32 fireDelay = 1.0f / GUN->RoundPerSec;
		f32 fillPercent = GUN->FireTimer / fireDelay;
		if (fillPercent > 1.f) fillPercent = 1.f;
		f32 barStartX = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		f32 barEndX = ChamberTimeBar.position.x + ChamberTimeBar.size.x / 2.f;
		fireTimeBar.MovementSpeed = fillPercent * (barEndX - barStartX) * global::DeltaTime;
		ChamberTimeBar.size = { 50.f / GUN->RoundPerSec, 5.f };
		if (GUN->gunType != prevGunType)
		{
			AEGfxTextureUnload(fireTimeBar.Texture);
			prevGunType = GUN->gunType;

			switch (prevGunType)
			{
			case InGame::GunType::PISTOL:
				fireTimeBar.size = { 5.f, 15.f };
				fireTimeBar.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
				break;
			case InGame::GunType::RIFLE:
				fireTimeBar.size = { 5.f, 21.f };
				fireTimeBar.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::SHOTGUN:
				fireTimeBar.size = { 8.f, 21.f };
				fireTimeBar.Texture = AEGfxTextureLoad("Assets/ammo-shotgun.png");
				break;
			}
		}
		//can't fire
		if (GUN->FireTimer> 0 && GUN->FireTimer < fireDelay)
		{
			fireTimeBar.position.x = barStartX + (barEndX - barStartX) * fillPercent;
		}
		//can fire
		else if (GUN->FireTimer > fireDelay)
		{
			fireTimeBar.position.x = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		}
	}
	void HUDController::Draw()
	{
		for(int i = 0; i < HPBG.size(); i++)
			Utils::DrawObject(HPBG[i], false);
		for (int i = 0; i < HP.size(); i++)
			Utils::DrawObject(HP[i], false);
		if (GUN->FireTimer < 1.f / GUN->RoundPerSec)
		{
			Utils::DrawObject(ChamberTimeBar, false);
			Utils::DrawObject(fireTimeBar, false);
		}
	}
	void HUDController::Destroy()
	{
		AEGfxMeshFree(HPMesh);
		AEGfxTextureUnload(HPTex);
		AEGfxTextureUnload(HPBGTex);
		HPBG.clear();
		HP.clear();
		AEGfxMeshFree(ChamberTimeBar.Mesh);
		AEGfxMeshFree(fireTimeBar.Mesh);
		AEGfxTextureUnload(ChamberTimeBar.Texture);
		AEGfxTextureUnload(fireTimeBar.Texture);
	}
}