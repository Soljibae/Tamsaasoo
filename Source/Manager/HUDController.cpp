#include "HUDController.h"
#include "../InGame/Gun.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include <iostream>
#include <algorithm>

namespace Manager
{
	HUDController HUD;
	AEGfxVertexList* HUDController::HPMesh = nullptr;
	AEGfxTexture* HUDController::HPTex = nullptr;
	AEGfxTexture* HUDController::HPBGTex = nullptr;
	const float HPWidth = 40.f;
	const float HPHeight = 50.f;

	AEGfxVertexList* FillingMeshUpside(f32 fillPercent)
	{
		f32 fill = std::clamp(fillPercent, 0.f, 1.f);
		f32 uv_top = 1.0f - fill;
		u32 c = 0xFFFFFFFF;
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, c, 0.0f, 1.0f,
			0.5f, -0.5f, c, 1.0f, 1.0f,
			-0.5f, -0.5f + fill, c, 0.0f, uv_top
		);
		AEGfxTriAdd(
			0.5f, -0.5f, c, 1.0f, 1.0f,
			0.5f, -0.5f + fill, c, 1.0f, uv_top,
			-0.5f, -0.5 + fill, c, 0.0f, uv_top
		);

		AEGfxVertexList* Mesh = AEGfxMeshEnd();
		return Mesh;
	}

	void HUDController::Init(InGame::PlayerCharacter* InPC, InGame::Gun* InGUN)
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		PC = InPC;
		GUN = InGUN;
		MaxHP = PC->Stats.MaxHP;
		currentHP = PC->Stats.HP;

		Coin.Mesh = Utils::CreateMesh();
		Coin.Texture = AEGfxTextureLoad("Assets/Coin.png");
		Coin.position = { (w / 2.f) / 3 * 2, (h / 2.f) / 3 * 2 };
		Coin.size = { 30.f, 30.f };
		const float spacingX = 10.0f; // 가로 간격
		const float startX = -(w / 2) + 200.f;
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
			bgobj.position = { startX + col * (HPWidth + spacingX), Y };
			bgobj.size = { HPWidth, HPHeight };
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
			hpobj.size = { HPWidth, HPHeight };
			HP.push_back(hpobj);
		}

		prevGunType = GUN->gunType;
		ChamberTimeBar.Mesh = Utils::CreateMesh();
		ChamberTimeBar.Texture = AEGfxTextureLoad("Assets/FireDelayBG.png");
		fireTimeBar.Mesh = Utils::CreateMesh();
		fireTimeBar.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
		ChamberTimeBar.position = { 0.f, -30.f };
		ChamberTimeBar.size = { 50.f / GUN->RoundPerSec, 14.f };
		fireTimeBar.position.x = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		fireTimeBar.position.y = ChamberTimeBar.position.y;
		fireTimeBar.size = { 10.f, 30.f };
		f32 barStartX = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		f32 barEndX = ChamberTimeBar.position.x + ChamberTimeBar.size.x / 2.f;
		fireTimeBar.MovementSpeed = 0.000016f * (barEndX - barStartX) * global::DeltaTime;

		Potion.position = { -(w/2) + 100.f, h/2 - 100.f };
		Potion.size = { 90.f, 90.f };
		Potion.Mesh = FillingMeshUpside(0);
		Potion.Texture = AEGfxTextureLoad("Assets/Potion.png");
		PotionBG.position = Potion.position;
		PotionBG.size = { 100.f, 100.f };
		PotionBG.Mesh = Utils::CreateMesh();
		PotionBG.Texture = AEGfxTextureLoad("Assets/PotionBG.png");
		prevPotion = PC->Stats.Potion;
		pFont = AEGfxCreateFont("Assets/buggy-font.ttf", 72.f);
	}
	void HUDController::Update()
	{
		if (global::KeyInput(AEVK_F))
		{
			switch (GUN->gunType)
			{
			case InGame::GunType::PISTOL:
				ChamberTimeBar.size = { 50.f / GUN->RoundPerSec, 14.f };
				GUN->gunType = InGame::GunType::RIFLE;
				break;
			case InGame::GunType::RIFLE:
				ChamberTimeBar.size = { 50.f / GUN->RoundPerSec, 14.f };
				GUN->gunType = InGame::GunType::SHOTGUN;
				break;
			case InGame::GunType::SHOTGUN:
				ChamberTimeBar.size = { 50.f / GUN->RoundPerSec, 14.f };
				GUN->gunType = InGame::GunType::PISTOL;
				break;
			}
		}
		while (currentHP > PC->Stats.MaxHP)
		{
			if (!HP.empty())
			{
				HP.pop_back();
				currentHP--;
				PC->Stats.HP--;
			}
		}
		while (MaxHP > PC->Stats.MaxHP)
		{
			if (!HPBG.empty())
			{
				HPBG.pop_back();
				MaxHP--;
			}
		}
		while (currentHP > PC->Stats.HP)
		{
			if (!HP.empty())
				HP.pop_back();
			currentHP--;
		}
		if (currentHP < PC->Stats.HP)
		{
			InGame::Actor hpobj;
			hpobj.Texture = HPBGTex;
			hpobj.Mesh = HPMesh;
			for (int i = 0; i < PC->Stats.HP - currentHP; i++)
			{
				hpobj.position = HPBG[currentHP+i].position;
				hpobj.size = { HPWidth, HPHeight };
				HP.push_back(hpobj);
			}
		}
		MaxHP = PC->Stats.MaxHP;
		currentHP = PC->Stats.HP;

		/*--------Centered can fire UI--------*/
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
		if (GUN->gunType != prevGunType)
		{
			AEGfxTextureUnload(fireTimeBar.Texture);
			prevGunType = GUN->gunType;

			switch (prevGunType)
			{
			case InGame::GunType::PISTOL:
				fireTimeBar.size = { 7.f, 21.f }; // 1:3
				fireTimeBar.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
				break;
			case InGame::GunType::RIFLE:
				fireTimeBar.size = { 7.f, 29.4f }; // 1:4.2
				fireTimeBar.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::SHOTGUN:
				fireTimeBar.size = { 8.f, 21 }; // 8:21
				fireTimeBar.Texture = AEGfxTextureLoad("Assets/ammo-shotgun.png");
				break;
			}
		}

		//can't fire
		if (GUN->FireTimer > 0 && GUN->FireTimer < fireDelay)
		{
			fireTimeBar.position.x = barStartX + (barEndX - barStartX) * fillPercent;
		}
		//can fire
		else if (GUN->FireTimer > fireDelay)
		{
			fireTimeBar.position.x = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		}
		/*--------Centered can fire UI--------*/

		/*-----*DEBUG* show me the money *DEBUG*-----*/
		if (global::KeyInput(AEVK_M))
		{
			PC->Stats.Money += 1000;
		}
		/*-----*DEBUG* show me the money *DEBUG*-----*/
		//DEBUG
		if (global::KeyInput(AEVK_P))
		{
			PC->Stats.Potion += 10;
		}
		//DEBUG
		if (PC->Stats.Potion != prevPotion)
		{
			f32 fillPercent = static_cast<f32>(prevPotion) / 100.f;
			AEGfxMeshFree(Potion.Mesh);
			Potion.Mesh = FillingMeshUpside(fillPercent);
			if (prevPotion > PC->Stats.Potion)
			{
				prevPotion--;
			}
			else if (prevPotion < PC->Stats.Potion)
			{
				prevPotion++;
			}

		}
	}
	void HUDController::Draw()
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);

		for(int i = 0; i < HPBG.size(); i++)
			Utils::DrawObject(HPBG[i], false);
		for (int i = 0; i < HP.size(); i++)
			Utils::DrawObject(HP[i], false);
		Utils::DrawObject(PotionBG, false);
		Utils::DrawObject(Potion, false);
		if (GUN->FireTimer < 1.f / GUN->RoundPerSec)
		{
			Utils::DrawObject(ChamberTimeBar, false);
			Utils::DrawObject(fireTimeBar, false);
		}
		Utils::DrawObject(Coin, false);
		std::string pText = std::to_string(PC->Stats.Money);
		f32 textW, textH;
		AEGfxGetPrintSize(pFont, pText.c_str(), 1.f, &textW, &textH);
		AEGfxPrint(pFont, pText.c_str(), (Coin.position.x / (w / 2))+0.03f, (Coin.position.y / (h / 2))-0.025f, 0.3f, 1, 1, 1, 1);
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

		AEGfxMeshFree(Coin.Mesh);
		AEGfxTextureUnload(Coin.Texture);
		AEGfxDestroyFont(pFont);

		AEGfxMeshFree(Potion.Mesh);
		AEGfxTextureUnload(Potion.Texture);

		AEGfxMeshFree(PotionBG.Mesh);
		AEGfxTextureUnload(PotionBG.Texture);

	}
}