#include "HUDController.h"
#include "../InGame/Gun.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "GameOver.h"
#include "../InGame/SFX.h"
#include "GameManager.h"
#include "BossHPBar.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include "SettingUI.h"

namespace Manager
{
	BossHPBar BossHP;
	HUDController HUD;
	AEGfxVertexList* HUDController::HPMesh = nullptr;
	AEGfxTexture* HUDController::HPTex = nullptr;
	AEGfxTexture* HUDController::HPBGTex = nullptr;
	InGame::Item* HUDController::prevItem{ nullptr };
	const static f32 HPWidth = 40.f;
	const static f32 HPHeight = 50.f;
	const static f32 textDrawSize = 0.2f;
	const static f32 fontSize = 72.f;
	const static f32 maxTextW = 400.0f;
	const static f32 padding = 20.f;
	Tooltip tooltip;
	const AEVec2 PotionAsset{20.f, 16.f};
	const AEVec2 PotionBGAsset{24.f, 28.f};
	const AEVec2 PotionSize{ 5.f, 5.f };
	const static float spacingX = 3.f;
	const static f32 vignettingRedValue = 0.5f;
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
		prevMaxHP = PC->Stats->MaxHP;
		prevCurrentHP = PC->Stats->HP;

		Coin.Mesh = Utils::CreateMesh();
		Coin.Texture = AEGfxTextureLoad("Assets/Coin.png");
		Coin.position = { (w / 2.f) / 3 * 2, (h / 2.f) / 3 * 2.35f };
		Coin.size = { 50.f, 50.f };
		float startX = -(w / 2) + 200.f;
		float Y = (h / 2) - 90.f;

		HPMesh = Utils::CreateMesh();
		HPTex = AEGfxTextureLoad("Assets/HP/HPBG.png");
		//HPBG HUD init
		InGame::Actor bgobj;
		bgobj.Texture = HPTex;
		bgobj.Mesh = HPMesh;
		for (int i = 0; i < prevMaxHP; ++i)
		{
			int row = i;
			int col = i;
			bgobj.position = { startX + col * (HPWidth+ spacingX), Y };
			bgobj.size = { HPWidth, HPHeight };
			HPBG.push_back(bgobj);
		}
		//HP HUD init
		HPBGTex = AEGfxTextureLoad("Assets/HP/HP.png");
		InGame::Actor hpobj;
		hpobj.Texture = HPBGTex;
		hpobj.Mesh = HPMesh;
		for (int i = 0; i < prevCurrentHP; i++)
		{
			hpobj.position = HPBG[i].position;
			hpobj.size = { HPWidth, HPHeight };
			HP.push_back(hpobj);
		}

		prevGunType = GUN->gunType;
		ChamberTimeBar.position = { 0.f, -30.f };
		ChamberTimeBar.size = { 66.f, 7.f };
		ChamberTimeBar.Mesh = Utils::CreateMesh();
		ChamberTimeBar.Texture = AEGfxTextureLoad("Assets/UI/FireDelayBG.png");
		fireTimeBar.position = ChamberTimeBar.position;
		fireTimeBar.size = ChamberTimeBar.size;
		fireTimeBar.Mesh = Utils::CreateMesh();
		fireTimeBar.Texture = AEGfxTextureLoad("Assets/UI/FireTimeBar.png");
		ammoType.position.x = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		ammoType.position.y = ChamberTimeBar.position.y;
		ammoType.Mesh = Utils::CreateMesh();
		ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-pistol.png");
		f32 barStartX = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		f32 barEndX = ChamberTimeBar.position.x + ChamberTimeBar.size.x / 2.f;

		Potion.position = { -(w / 2) + 100.f, h / 2 - 130.f };
		Potion.size = { PotionSize.x * PotionAsset.x, PotionSize.y * PotionAsset.y };
		PotionFillPercent = 0.f;
		Potion.Mesh = FillingMeshUpside(PotionFillPercent);
		Potion.Texture = AEGfxTextureLoad("Assets/HP/Potion.png");
		PotionBG.position = { Potion.position.x, Potion.position.y + 20.f };
		PotionBG.size = { PotionSize.x * PotionBGAsset.x, PotionSize.y * PotionBGAsset.y };
		PotionBG.Mesh = Utils::CreateMesh();
		PotionBG.Texture = AEGfxTextureLoad("Assets/HP/PotionBG.png");

		PotionFull.size = { 40.f,40.f };
		PotionFull.position = Potion.position;
		//PotionFull.position.x = PotionBG.position.x;
		//PotionFull.position.y = PotionBG.position.y - PotionBG.size.y / 2.f - PotionFull.size.y;
		PotionFull.Mesh = Utils::CreateMesh();
		PotionFull.Texture = AEGfxTextureLoad("Assets/HP/Q.png");

		plzUsePotion.size.x = PotionBG.size.x * 1.1f;
		plzUsePotion.size.y = PotionBG.size.y * 1.1f;
		plzUsePotion.position = PotionBG.position;
		plzUsePotion.Mesh = PotionBG.Mesh;
		plzUsePotion.Texture = AEGfxTextureLoad("Assets/HP/PotionBG.png");

		prevPotion = PC->PS->Potion;
		prevFireRate = GUN->RoundPerSec;
		pFont = AEGfxCreateFont("Assets/Fonts/buggy-font.ttf", fontSize);

		tooltip.Window.size = { maxTextW ,200 };
		tooltip.Window.position = { 0, 0 };
		tooltip.WindowMesh = Utils::CreateNinePatchMesh();
		tooltip.Window.Texture = AEGfxTextureLoad("Assets/tooltipBorder.png");

		Vignetting.position = { 0,0 };
		Vignetting.size = { w,h };
		Vignetting.Mesh = Utils::CreateMesh();
		Vignetting.Texture = AEGfxTextureLoad("Assets/Vignetting.png");

		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/HeartBeat.wav", "heart");
	}

	void HUDController::Update()
	{
		if (prevMaxHP != PC->Stats->MaxHP)
		{
			HPBG.clear();
			const float startX = -(global::ScreenWidth / 2) + 200.f;
			float Y = (global::ScreenHeight / 2) - 90.f;
			InGame::Actor bgobj;
			bgobj.Texture = HPTex;
			bgobj.Mesh = HPMesh;

			int col = 0;
			for (int i = 0; i < PC->Stats->MaxHP; ++i)
			{
				if (PC->Stats->MaxHP > 7)
				{
					if (col > 7)
					{
						col = 0;
						Y += 30;
					}
				}
				bgobj.position = { startX + col * (HPWidth + spacingX), Y };
				HPBG.push_back(bgobj);
				col++;
				bgobj.size = { HPWidth, HPHeight };
			}
		}

		if (prevCurrentHP != PC->Stats->HP)
		{
			HP.clear();
			InGame::Actor hpobj;
			hpobj.Texture = HPBGTex;
			hpobj.Mesh = HPMesh;
			for (int i = 0; i < PC->Stats->HP; i++)
			{
				hpobj.position = HPBG[i].position;
				hpobj.size = { HPWidth, HPHeight };
				HP.push_back(hpobj);
			}
		}

		prevMaxHP = PC->Stats->MaxHP;
		prevCurrentHP = PC->Stats->HP;

		if (PC->HoldingGun->gunType != prevGunType)
		{
			AEGfxTextureUnload(ammoType.Texture);
			ammoType.Texture = nullptr;
			GUN = PC->HoldingGun;
			prevGunType = GUN->gunType;

			switch (prevGunType)
			{
			case InGame::GunType::M1911:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-pistol.png");
				break;
			case InGame::GunType::CZ75:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-pistol.png");
				break;
			case InGame::GunType::DESERTEGLE:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-pistol.png");
				break;
			case InGame::GunType::MP5:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-pistol.png");
				break;
			case InGame::GunType::MPX:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-pistol.png");
				break;
			case InGame::GunType::VECTOR:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-pistol.png");
				break;
			case InGame::GunType::BEOWOLF:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-pistol.png");
				break;
			case InGame::GunType::P90:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::MOSINNAGAT:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::M24:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::RAILGUN:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::NITRO700:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::FNFAL:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::M82BARRETT:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::AR15:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::M110:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::BREN:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::MICROGUN:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::M249:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::M2:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-rifle.png");
				break;
			case InGame::GunType::SAWEDOFFSHOTGUN:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-shotgun.png");
				break;
			case InGame::GunType::DOUBLEBARREL:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-shotgun.png");
				break;
			case InGame::GunType::KS23:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-shotgun.png");
				break;
			case InGame::GunType::M1897:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-shotgun.png");
				break;
			case InGame::GunType::BENELLIM4:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-shotgun.png");
				break;
			case InGame::GunType::SAIGA12:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-shotgun.png");
				break;
			case InGame::GunType::AA12:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/UI/ammo-shotgun.png");
				break;
			}
		}
		/*--------Centered can fire UI--------*/
		//if (fireTimeBar.position.x > ChamberTimeBar.position.x + ChamberTimeBar.size.x / 2.f)
		//{
		//	fireTimeBar.position.x = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		//}


		//f32 barStartX = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
		//f32 barEndX = ChamberTimeBar.position.x + ChamberTimeBar.size.x / 2.f;
		f32 fireDelay = 1.0f / GUN->RoundPerSec;
		f32 fillPercent = GUN->FireTimer / fireDelay;
		if (fillPercent > 1.f) fillPercent = 1.f;
		f32 decrease = 1.f - fillPercent;
		//can't fire
		if (GUN->FireTimer > 0 && GUN->FireTimer < fireDelay)
		{
			//fireTimeBar.position.x = barStartX + (barEndX - barStartX) * fillPercent;
			
			AEGfxMeshFree(fireTimeBar.Mesh);
			fireTimeBar.Mesh = nullptr;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-0.5f, -0.5f, 0xFFFFFFFF, 0.f, 1.f,
				0.5f - decrease, -0.5f, 0xFFFFFFFF, fillPercent, 1.f,
				-0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f
			);
			AEGfxTriAdd(
				0.5f - decrease, -0.5f, 0xFFFFFFFF, fillPercent, 1.f,
				0.5f - decrease, 0.5f, 0xFFFFFFFF, fillPercent, 0.f,
				-0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f
			);
			fireTimeBar.Mesh = AEGfxMeshEnd();
		}
		//can fire
		else if (GUN->FireTimer > fireDelay)
		{
			//fireTimeBar.position.x = ChamberTimeBar.position.x - ChamberTimeBar.size.x / 2.f;
			fillPercent = 0.f;
		}
		prevFireRate = GUN->RoundPerSec;
		/*--------Centered can fire UI--------*/

		/*-----*DEBUG* show me the money *DEBUG*-----*/
		//if (global::KeyInput(AEVK_M))
		//{
		//	PC->PS->Money += 10;
		//	PC->PS->Potion += 100;
		//}
		/*-----*DEBUG* show me the money *DEBUG*-----*/
		if (PC->PS->Potion != prevPotion)
		{
			PotionFillPercent = static_cast<f32>(prevPotion) / static_cast<f32>(global::MaxPotionGauge);
			AEGfxMeshFree(Potion.Mesh);
			Potion.Mesh = FillingMeshUpside(PotionFillPercent);
			if (prevPotion > PC->PS->Potion)
			{
				prevPotion -= 3;
				if (prevPotion < PC->PS->Potion)
					prevPotion = PC->PS->Potion;
			}
			else if (prevPotion < PC->PS->Potion)
			{
				prevPotion += 2;
			}
		}
	}
	void HUDController::Draw()
	{
		static bool soundReduced{ false };
		// Set color to red and reset to origin color
		if (PC->Stats->HP > 0 && PC->Stats->HP < 2)
		{
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

			AEGfxSetBlendMode(AE_GFX_BM_BLEND);

			AEGfxSetTransparency(1.0f);

			AEGfxTextureSet(Vignetting.Texture, 0.f, 0.f);
			for(auto group : SFXManager.sound_group)
			{
				switch (group.first)
				{
				case InGame::BGM:
					global::isVolumeReduced = true;
					break;
				case InGame::SFX:
					global::isVolumeReduced = true;
					break;
				}
			}
			AEMtx33 scale;
			AEMtx33Scale(&scale, Vignetting.size.x, Vignetting.size.y);
			AEMtx33 tran;
			AEMtx33Trans(&tran, Vignetting.position.x, Vignetting.position.y);
			AEMtx33 transform;

			AEMtx33Concat(&transform, &tran, &scale);

			AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 0.f);

			AEGfxSetColorToAdd(vignettingRedValue, 0.f, 0.f, 0.5f);

			AEGfxSetTransform(transform.m);

			AEGfxMeshDraw(Vignetting.Mesh, AE_GFX_MDM_TRIANGLES);

			AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 0.f);

			AEGfxSetColorToAdd(1.f, 1.f, 1.f, 1.f);

			if (!soundReduced)
			{
				SFXManager.Play("heart");
				soundReduced = true;
			}
		}
		else
		{
			if (soundReduced)
			{
				for (auto group : SFXManager.sound_group)
				{
					switch (group.first)
					{
					case InGame::BGM:
						global::isVolumeReduced = false;
						break;
					case InGame::SFX:
						global::isVolumeReduced = false;
						break;
					}
				}
				soundReduced = false;
			}
		}
		if (gameOverScreen.isGameOver)
		{
			return;
		}
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);

		for (int i = 0; i < HPBG.size(); i++)
			Utils::DrawObject(HPBG[i], false);
		for (int i = 0; i < HP.size(); i++)
			Utils::DrawObject(HP[i], false);

		if (PC->PS->Potion >= global::MaxPotionGauge)
		{
			static f32 g{ 0.f };
			static f32 timer{ 0.f };
			static f32 timerQ{ 0.f };
			static s8 colorDir{ 1 };
			timer += global::DeltaTime;
			timerQ += global::DeltaTime;
			g += global::DeltaTime * 2.f * colorDir;
			g = std::clamp(g, 0.f, 1.f);

			if (timer > 0.5f)
			{
				if (colorDir > 0.0f)
				{
					colorDir = -1;
				}
				else
				{
					colorDir = 1;
				}
				timer = 0.f;
			}

			//if (timerQ > 0.2f)
			//{
			//	if (PotionFull.Alpha > 0.5f)
			//	{
			//		PotionFull.Alpha = 0.f;
			//	}
			//	else
			//	{
			//		PotionFull.Alpha = 1.f;
			//	}
			//	timerQ = 0.f;
			//}

			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

			AEGfxSetBlendMode(AE_GFX_BM_BLEND);

			AEGfxSetTransparency(1.0f);

			AEMtx33 scale;
			AEMtx33Scale(&scale, plzUsePotion.size.x, plzUsePotion.size.y);
			AEMtx33 tran;
			AEMtx33Trans(&tran, plzUsePotion.position.x, plzUsePotion.position.y);
			AEMtx33 transform;

			AEGfxTextureSet(plzUsePotion.Texture, 0.f, 0.f);

			AEMtx33Concat(&transform, &tran, &scale);
			AEGfxSetTransform(transform.m);

			AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 0.f);
			AEGfxSetColorToAdd(1.f, g, 0.f, 1.f);

			AEGfxMeshDraw(plzUsePotion.Mesh, AE_GFX_MDM_TRIANGLES);

			AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 0.f);
			AEGfxSetColorToAdd(1.f, 1.f, 1.f, 1.f);

		}

		Utils::DrawObject(Potion, false);
		Utils::DrawObject(PotionBG, false);

		if (PC->PS->Potion >= global::MaxPotionGauge)
		{
			Utils::DrawObject(PotionFull, false);//, PotionFull.Alpha);
		}

		if (GUN->FireTimer < 1.f / GUN->RoundPerSec)
		{
			Utils::DrawObject(ChamberTimeBar, false);
			Utils::DrawObject(fireTimeBar, false);
			Utils::DrawObject(ammoType, false);
		}
		Utils::DrawObject(Coin, false);
		std::string pText = std::to_string(static_cast<s32>(PC->PS->Money));
		f32 textW, textH;
		AEGfxGetPrintSize(pFont, pText.c_str(), textDrawSize, &textW, &textH);
		static bool isBigger{ false };
		static f64 prevMoney{ PC->PS->Money };
		if (prevMoney != PC->PS->Money)
		{
			isBigger = true;
			prevMoney = PC->PS->Money;
		}

		AEGfxPrint(pFont, pText.c_str(), (Coin.position.x + Coin.size.x / 1.5f + 5.f) / (w / 2),
			(Coin.position.y - Coin.size.y / 2.5f - 2.f) / (h / 2), isBigger ? 0.43f : 0.4f, 0, 0, 0, 1);
		AEGfxPrint(pFont, pText.c_str(), (Coin.position.x + Coin.size.x / 1.5f) / (w / 2),
			(Coin.position.y - Coin.size.y / 2.5f) / (h / 2), isBigger ? 0.43f : 0.4f, 1, 1, 1, 1);
		if (isBigger)
		{
			isBigger = false;
		}
	}

	static std::vector<std::string>
		BreakWordUTF8ByWidth(const std::string& utf8,
			float maxWidthPx,
			float scale,
			bool useKerning)
	{
		std::vector<std::string> out;
		if (utf8.empty()) return out;

		auto cps = DecodeUTF8(utf8);
		std::string cur;
		float curW = 0.0f;

		auto widthOf = [&](const std::string& s)->float {
			return Atlas.GetPrintMetricsUTF8(s, scale, useKerning).width;
			};

		auto isDisallowedLineStart = [](uint32_t cp)->bool {
			switch (cp) {
			case U')': case U']': case U'}':
			case U'.': case U',': case U'!': case U'?': case U':': case U';':
			case 0x2026/*…*/: case 0x00B7/*·*/:
			case 0x3001/*、*/: case 0xFF0C/*，*/: case 0x3002/*。*/:
			case 0xFF01/*！*/: case 0xFF1F/*？*/: case 0xFF1A/*：*/:
				return true;
			default: return false;
			}
			};

		std::string pending;

		for (size_t i = 0; i < cps.size(); ++i) {
			uint32_t cp = cps[i];
			std::string ch;
			if (cp <= 0x7F) {
				ch.push_back(char(cp));
			}
			else if (cp <= 0x7FF) {
				ch.push_back(char(0xC0 | ((cp >> 6) & 0x1F)));
				ch.push_back(char(0x80 | (cp & 0x3F)));
			}
			else if (cp <= 0xFFFF) {
				ch.push_back(char(0xE0 | ((cp >> 12) & 0x0F)));
				ch.push_back(char(0x80 | ((cp >> 6) & 0x3F)));
				ch.push_back(char(0x80 | (cp & 0x3F)));
			}
			else {
				ch.push_back(char(0xF0 | ((cp >> 18) & 0x07)));
				ch.push_back(char(0x80 | ((cp >> 12) & 0x3F)));
				ch.push_back(char(0x80 | ((cp >> 6) & 0x3F)));
				ch.push_back(char(0x80 | (cp & 0x3F)));
			}

			float wCh = widthOf(ch);

			if (cur.empty()) {
				if (isDisallowedLineStart(cp) && !out.empty()) {
					pending = ch;
					continue;
				}
			}

			if (curW + wCh <= maxWidthPx) {
				cur += pending;
				cur += ch;
				curW += widthOf(pending) + wCh;
				pending.clear();
			}
			else {
				if (!cur.empty()) out.push_back(cur);
				cur = ch;
				curW = wCh;
			}
		}

		if (!pending.empty()) {
			if (curW + Atlas.GetPrintMetricsUTF8(pending, scale, useKerning).width <= maxWidthPx) {
				cur += pending;
			}
			else {
				if (!cur.empty()) out.push_back(cur);
				cur = pending;
			}
		}
		if (!cur.empty()) out.push_back(cur);
		return out;
	}

	std::vector<std::string> HUDController::SplitTextIntoLines(const std::string& text, f32 maxWidth)
	{
		std::vector<std::string> lines;
		std::istringstream paraStream(text);
		std::string paragraph;
		while (std::getline(paraStream, paragraph))
		{
			std::istringstream wordStream(paragraph);
			std::string word, currentLine;
			f32 lineWidth = 0;
			f32 spaceWidth, spaceHeight;
			AEGfxGetPrintSize(pFont, " ", textDrawSize, &spaceWidth, &spaceHeight);
			spaceWidth *= global::ScreenWidth / 2.f;

			while (wordStream >> word)
			{

				f32 w, h;
				AEGfxGetPrintSize(pFont, word.c_str(), textDrawSize, &w, &h);
				w *= global::ScreenWidth / 2.f;
				if (lineWidth + (currentLine.empty() ? 0 : spaceWidth) + w <= maxWidth)
				{
					if (!currentLine.empty())
					{
						currentLine += " ";
						lineWidth += spaceWidth;
					}
					currentLine += word;
					lineWidth += w;
				}
				else
				{
					lines.push_back(currentLine);
					currentLine = word;
					lineWidth = w;
				}
			}
			if (!currentLine.empty())
			{
				lines.push_back(currentLine);
			}
		}
		return lines;
	}

	std::vector<std::string>
		HUDController::SplitTextIntoLines_UTF8_KR(const std::string& textUTF8,
			float maxWidthPx,
			float scale,
			bool useKerning)
	{
		std::vector<std::string> lines;
		std::istringstream paraStream(textUTF8);
		std::string paragraph;

		auto widthOf = [&](const std::string& s)->float {
			return Atlas.GetPrintMetricsUTF8(s, scale, useKerning).width;
			};

		const float spaceW = widthOf(" ");

		while (std::getline(paraStream, paragraph)) {
			std::istringstream wordStream(paragraph);
			std::string word, curLine;
			float curW = 0.0f;

			while (wordStream >> word) {
				float wWord = widthOf(word);

				std::string candidate = curLine.empty() ? word : (curLine + " " + word);
				float candW = widthOf(candidate);

				const float EPS = 0.5f;

				if (candW <= maxWidthPx + EPS) {
					curLine.swap(candidate);
					curW = candW;
					continue;
				}

				if (!curLine.empty()) {
					lines.push_back(curLine);
					curLine.clear();
					curW = 0.0f;
				}

				if (wWord > maxWidthPx + EPS) {
					auto chunks = BreakWordUTF8ByWidth(word, maxWidthPx, scale, useKerning);
					for (size_t i = 0; i + 1 < chunks.size(); ++i)
						lines.push_back(chunks[i]);
					if (!chunks.empty()) {
						curLine = chunks.back();
						curW = widthOf(curLine);
					}
				}
				else {
					curLine = word;
					curW = wWord;
				}
			}

			if (!curLine.empty())
				lines.push_back(curLine);
		}
		return lines;
	}

	InGame::Actor* HUDController::GetPotion()
	{
		return &Potion;
	}

	void HUDController::TooltipUpdate(InGame::Item& item)
	{
		itemDesc = SplitTextIntoLines_UTF8_KR(item.description, maxTextW, 1.f);

		const char* tagStr = "NONE";
		switch (item.tag) {
		case InGame::ItemTag::ENVY:     tagStr = "ENVY";     break;
		case InGame::ItemTag::GLUTTONY: tagStr = "GLUTTONY"; break;
		case InGame::ItemTag::GREED:    tagStr = "GREED";    break;
		case InGame::ItemTag::LUST:     tagStr = "LUST";     break;
		case InGame::ItemTag::SLOTH:    tagStr = "SLOTH";    break;
		case InGame::ItemTag::WRATH:    tagStr = "WRATH";    break;
		case InGame::ItemTag::PRIDE:    tagStr = "PRIDE";    break;
		default:                         tagStr = "NONE";     break;
		}
		itemDesc.insert(itemDesc.begin(), tagStr);
		itemDesc.insert(itemDesc.begin(), item.name);

		const float s = 1.f;
		auto m = Manager::Atlas.GetPrintMetricsUTF8(global::stringForKRGap, s);
		float lineH = m.lineHeight;
		float asc = m.ascender;
		float desc = m.descender;

		float wMax = 0.f;
		for (size_t i = 0; i < itemDesc.size(); ++i) {
			float w = Manager::Atlas.GetPrintMetricsUTF8(itemDesc[i], s, true).width;
			if (w > wMax) wMax = w;
		}

		const float frame = padding;
		const float textPad = 8.f;

		float contentW = wMax;
		float contentH = asc + ((int)itemDesc.size() - 1) * lineH + desc;

		tooltip.Window.size.x = 2.f * (frame + textPad) + contentW;
		tooltip.Window.size.y = 2.f * (frame + textPad) + contentH;

		s32 mouseX, mouseY; AEInputGetCursorPosition(&mouseX, &mouseY);
		AEVec2 MP;
		MP.x = (float)mouseX - AEGfxGetWindowWidth() * 0.5f;
		MP.y = AEGfxGetWindowHeight() * 0.5f - (float)mouseY;

		tooltip.Window.position = {
			MP.x + tooltip.Window.size.x * 0.5f + frame,
			MP.y + tooltip.Window.size.y * 0.5f + frame
		};

		if (prevItem != &item) {
			prevItem = &item;
			for (int i = 0; i < tooltip.WindowMesh.size(); i++) {
				if (tooltip.WindowMesh[i]) { AEGfxMeshFree(tooltip.WindowMesh[i]); tooltip.WindowMesh[i] = nullptr; }
			}
			tooltip.WindowMesh = Utils::CreateNinePatchMesh();
		}
	}

	static inline u32 RGBA8(float r, float g, float b, float a = 1.f) {
		auto c = [](float v)->u32 { v = v < 0 ? 0 : (v > 1 ? 1 : v); return (u32)(v * 255.f + 0.5f); };
		return (c(a) << 24) | (c(r) << 16) | (c(g) << 8) | c(b);
	}

	void HUDController::ShowTooltip(InGame::Item& item)
	{
		if (SettingPanel.isSettingOn) return;

		Utils::DrawNinePatchMesh(tooltip.Window, tooltip.Window.Texture, tooltip.WindowMesh, padding);

		float px = tooltip.Window.position.x - tooltip.Window.size.x * 0.5f;
		float py = tooltip.Window.position.y - tooltip.Window.size.y * 0.5f;
		float boxW = tooltip.Window.size.x;
		float boxH = tooltip.Window.size.y;

		const float frame = padding;
		const float textPad = 8.f;
		float xLeft = px + frame + textPad;
		float yTop = py + boxH - frame - textPad;

		const float s = 1.f;
		auto m = Manager::Atlas.GetPrintMetricsUTF8(global::stringForKRGap, s);
		float lineH = m.lineHeight;
		float asc = m.ascender;

		float curY = yTop - asc;

		for (size_t i = 0; i < itemDesc.size(); ++i) {
			u32 col = 0xFFFFFFFF;
			if (i == 1)
			{
				if (itemDesc[1] == u8"ENVY")
				{
					col = 0x4B0082FF;
				}
				else if (itemDesc[1] == u8"GLUTTONY")
				{
					col = 0x008000FF;
				}
				else if (itemDesc[1] == u8"GREED")
				{
					col = 0x0000FFFF;
				}
				else if (itemDesc[1] == u8"LUST")
				{
					col = 0x0D00A6FF;
				}
				else if (itemDesc[1] == u8"SLOTH")
				{
					col = 0xFFFF00FF;
				}
				else if (itemDesc[1] == u8"WRATH")
				{
					col = 0xFF8000FF;
				}
				else if (itemDesc[1] == u8"PRIDE")
				{
					col = 0xFF0000FF;
				}
				else if (itemDesc[1] == u8"NONE")
				{
					col = 0x555555FF;
				}

			}
			Manager::Atlas.RenderTextUTF8(itemDesc[i], xLeft, curY, s, col);
			curY -= lineH;
		}
	}

	void HUDController::Destroy()
	{
		if (HPMesh)
		{
			AEGfxMeshFree(HPMesh);
			HPMesh = nullptr;
		}
		if (HPTex)
		{
			AEGfxTextureUnload(HPTex);
			HPTex = nullptr;
		}
		if (HPBGTex)
		{
			AEGfxTextureUnload(HPBGTex);
			HPBGTex = nullptr;
		}

		HPBG.clear();
		HP.clear();

		if (ChamberTimeBar.Mesh)
		{
			AEGfxMeshFree(ChamberTimeBar.Mesh);
			ChamberTimeBar.Mesh = nullptr;
		}
		if (ChamberTimeBar.Texture)
		{
			AEGfxTextureUnload(ChamberTimeBar.Texture);
			ChamberTimeBar.Texture = nullptr;
		}

		if (fireTimeBar.Mesh)
		{
			AEGfxMeshFree(fireTimeBar.Mesh);
			fireTimeBar.Mesh = nullptr;
		}
		if (fireTimeBar.Texture)
		{
			AEGfxTextureUnload(fireTimeBar.Texture);
			fireTimeBar.Texture = nullptr;
		}

		if (ammoType.Mesh)
		{
			AEGfxMeshFree(ammoType.Mesh);
			ammoType.Mesh = nullptr;
		}
		if (ammoType.Texture)
		{
			AEGfxTextureUnload(ammoType.Texture);
			ammoType.Texture = nullptr;
		}

		if (Coin.Mesh)
		{
			AEGfxMeshFree(Coin.Mesh);
			Coin.Mesh = nullptr;
		}
		if (Coin.Texture)
		{
			AEGfxTextureUnload(Coin.Texture);
			Coin.Texture = nullptr;
		}

		if (Potion.Mesh)
		{
			AEGfxMeshFree(Potion.Mesh);
			Potion.Mesh = nullptr;
		}
		if (Potion.Texture)
		{
			AEGfxTextureUnload(Potion.Texture);
			Potion.Texture = nullptr;
		}

		if (PotionBG.Mesh)
		{
			AEGfxMeshFree(PotionBG.Mesh);
			PotionBG.Mesh = nullptr;
		}
		if (PotionBG.Texture)
		{
			AEGfxTextureUnload(PotionBG.Texture);
			PotionBG.Texture = nullptr;
		}

		if (PotionFull.Mesh)
		{
			AEGfxMeshFree(PotionFull.Mesh);
			PotionFull.Mesh = nullptr;
		}
		if (PotionFull.Texture)
		{
			AEGfxTextureUnload(PotionFull.Texture);
			PotionFull.Texture = nullptr;
		}

		if (plzUsePotion.Texture)
		{
			AEGfxTextureUnload(plzUsePotion.Texture);
			plzUsePotion.Texture = nullptr;
		}

		for (int i = 0; i < tooltip.WindowMesh.size(); i++)
		{
			if (tooltip.WindowMesh[i])
			{
				AEGfxMeshFree(tooltip.WindowMesh[i]);
				tooltip.WindowMesh[i] = nullptr;
			}
		}
		AEGfxTextureUnload(tooltip.Window.Texture);

		AEGfxDestroyFont(pFont);

		if (Vignetting.Mesh)
		{
			AEGfxMeshFree(Vignetting.Mesh);
			Vignetting.Mesh = nullptr;
		}
		
		if (Vignetting.Texture)
		{
			AEGfxTextureUnload(Vignetting.Texture);
			Vignetting.Texture = nullptr;
		}
	}

}