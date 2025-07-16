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
		Coin.size = { 30.f, 30.f };
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
		PotionFull.position.x = PotionBG.position.x;
		PotionFull.position.y = PotionBG.position.y+10.f;
		PotionFull.size = PotionBG.size;
		PotionFull.Mesh = Utils::CreateMesh();
		PotionFull.Texture = AEGfxTextureLoad("Assets/HP/PotionFull.png");
		
		prevPotion = PC->PS->Potion;
		prevFireRate = GUN->RoundPerSec;
		pFont = AEGfxCreateFont("Assets/buggy-font.ttf", fontSize);
		tooltip.Window.size = { maxTextW ,200 };
		tooltip.Window.position = { 0, 0 };
		tooltip.Window.Texture = AEGfxTextureLoad("Assets/tooltipBorder.png");
		Vignetting.position = { 0,0 };
		Vignetting.size = { w,h };
		Vignetting.Mesh = Utils::CreateMesh();
		Vignetting.Texture = AEGfxTextureLoad("Assets/Vignetting.png");
		SFXManager.AddNewSFX(InGame::PLAYER, "Assets/SFX/HeartBeat.wav", "heart");
	}

	void HUDController::Update()
	{
		if (prevMaxHP != PC->Stats->MaxHP)
		{
			HPBG.clear();
			const float startX = -(global::ScreenWidth / 2) + 200.f;
			const float Y = (global::ScreenHeight / 2) - 90.f;
			InGame::Actor bgobj;
			bgobj.Texture = HPTex;
			bgobj.Mesh = HPMesh;
			for (int i = 0; i < PC->Stats->MaxHP; ++i)
			{
				int row = i;
				int col = i;
				bgobj.position = { startX + col * (HPWidth + spacingX), Y };
				bgobj.size = { HPWidth, HPHeight };
				HPBG.push_back(bgobj);
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


		if (GUN->gunType != prevGunType)
		{
			AEGfxTextureUnload(ammoType.Texture);
			ammoType.Texture = nullptr;
			GUN = PC->HoldingGun;
			prevGunType = GUN->gunType;

			switch (prevGunType)
			{
			case InGame::GunType::M1911:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
				break;
			case InGame::GunType::CZ75:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
				break;
			case InGame::GunType::DESERTEGLE:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
				break;
			case InGame::GunType::MP5:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
				break;
			case InGame::GunType::MPX:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
				break;
			case InGame::GunType::VECTOR:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
				break;
			case InGame::GunType::BEOWOLF:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-pistol.png");
				break;
			case InGame::GunType::P90:
				ammoType.size = { 7.f, 21.f }; // 1:3
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::MOSINNAGAT:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::M24:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::RAILGUN:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::NITRO700:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::FNFAL:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::M82BARRETT:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::AR15:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::M110:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::BREN:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::MICROGUN:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::M249:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::M2:
				ammoType.size = { 7.f, 29.4f }; // 1:4.2
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-rifle.png");
				break;
			case InGame::GunType::SAWEDOFFSHOTGUN:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-shotgun.png");
				break;
			case InGame::GunType::DOUBLEBARREL:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-shotgun.png");
				break;
			case InGame::GunType::KS23:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-shotgun.png");
				break;
			case InGame::GunType::M1897:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-shotgun.png");
				break;
			case InGame::GunType::BENELLIM4:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-shotgun.png");
				break;
			case InGame::GunType::SAIGA12:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-shotgun.png");
				break;
			case InGame::GunType::AA12:
				ammoType.size = { 8.f, 21 }; // 8:21
				ammoType.Texture = AEGfxTextureLoad("Assets/ammo-shotgun.png");
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
		if (global::KeyInput(AEVK_M))
		{
			PC->PS->Money += 1000000000;
		}
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
				prevPotion++;
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
					AEAudioSetGroupVolume(group.second, SFXManager.BGMReduceVol);
					break;
				case InGame::SFX:
					AEAudioSetGroupVolume(group.second, SFXManager.SFXReduceVol); 
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

			if(!soundReduced)
				SFXManager.Play("heart");
			soundReduced = true;
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
						AEAudioSetGroupVolume(group.second, SFXManager.BGMOriginVol);
						break;
					case InGame::SFX:
						AEAudioSetGroupVolume(group.second, SFXManager.SFXOriginVol);
						break;
					case InGame::PLAYER:
						AEAudioStopGroup(SFXManager.sound_group[InGame::PLAYER]);
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
		Utils::DrawObject(PotionBG, false);
		Utils::DrawObject(Potion, false);
		if (PC->PS->Potion >= global::MaxPotionGauge)
		{
			Utils::DrawObject(PotionFull, false);
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
			(Coin.position.y - Coin.size.y / 2.5f - 2.f) / (h / 2), isBigger ? 0.33f : 0.3f, 0, 0, 0, 1);
		AEGfxPrint(pFont, pText.c_str(), (Coin.position.x + Coin.size.x / 1.5f) / (w / 2),
			(Coin.position.y - Coin.size.y / 2.5f) / (h / 2), isBigger ? 0.33f : 0.3f, 1, 1, 1, 1);
		if (isBigger)
		{
			isBigger = false;
		}
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

	InGame::Actor* HUDController::GetPotion()
	{
		return &Potion;
	}

	void HUDController::TooltipUpdate(InGame::Item& item)
	{
		itemDesc = SplitTextIntoLines(item.description, maxTextW);
		switch (item.tag)
		{
		case InGame::ItemTag::ENVY:
			itemDesc.insert(itemDesc.begin(), "ENVY");
			break;
		case InGame::ItemTag::GLUTTONY:
			itemDesc.insert(itemDesc.begin(), "GLUTTONY");
			break;
		case InGame::ItemTag::GREED:
			itemDesc.insert(itemDesc.begin(), "GREED");
			break;
		case InGame::ItemTag::LUST:
			itemDesc.insert(itemDesc.begin(), "LUST");
			break;
		case InGame::ItemTag::SLOTH:
			itemDesc.insert(itemDesc.begin(), "SLOTH");
			break;
		case InGame::ItemTag::WRATH:
			itemDesc.insert(itemDesc.begin(), "WRATH");
			break;
		case InGame::ItemTag::PRIDE:
			itemDesc.insert(itemDesc.begin(), "PRIDE");
			break;
		default:
			itemDesc.insert(itemDesc.begin(), "NONE");
			break;
		}
		itemDesc.insert(itemDesc.begin(), item.name);
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEVec2 MP;
		MP.x = static_cast<f32>(mouseX) - AEGfxGetWindowWidth() / 2.0f;
		MP.y = AEGfxGetWindowHeight() / 2.0f - static_cast<float>(mouseY);

		f32 lw, lh;
		AEGfxGetPrintSize(pFont, item.description.c_str(), textDrawSize, &lw, &lh);
		lh *= global::ScreenHeight;

		tooltip.Window.position = { MP.x + (tooltip.Window.size.x / 2.f) + padding, MP.y + (tooltip.Window.size.y / 2.f) + padding };
		tooltip.Window.size.y = lh * itemDesc.size();

		if (prevItem != &item)
		{
			prevItem = &item;
			s8 i = 0;
			for (int i = 0; i < tooltip.WindowMesh.size(); i++)
			{
				if (tooltip.WindowMesh[i])
				{
					AEGfxMeshFree(tooltip.WindowMesh[i]);
					tooltip.WindowMesh[i] = nullptr;
				}
			}
			tooltip.WindowMesh = Utils::CreateNinePatchMesh();
		}
	}

	void HUDController::ShowTooltip(InGame::Item& item)
	{
		
		Utils::DrawNinePatchMesh(tooltip.Window, tooltip.Window.Texture, tooltip.WindowMesh, padding);
		f32 tmp, lh;
		AEGfxGetPrintSize(pFont, item.description.c_str(), textDrawSize, &tmp, &lh);
		lh *= global::ScreenHeight;
		f32 px = tooltip.Window.position.x - tooltip.Window.size.x / 2.f;
		f32 py = tooltip.Window.position.y - tooltip.Window.size.y / 2.f;
		f32 boxW = tooltip.Window.size.x;
		f32 boxH = tooltip.Window.size.y;
		f32 halfW = AEGfxGetWindowWidth() * 0.5f;
		f32 halfH = AEGfxGetWindowHeight() * 0.5f;
		f32 baseX = px;
		f32 baseY = py + boxH - lh / 1.5f;
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEVec2 MP;
		MP.x = static_cast<f32>(mouseX) - AEGfxGetWindowWidth() / 2.0f;
		MP.y = AEGfxGetWindowHeight() / 2.0f - static_cast<float>(mouseY);
		f32 r{ 1.f }, g{ 1.f }, b{ 1.f };
		for (int i = 0; i < itemDesc.size(); i++)
		{
			f32 xPix = baseX;
			f32 yPix = baseY - lh * i;
			f32 xN = xPix / halfW;
			f32 yN = yPix / halfH;
			if (i == 1)
			{
				r = 0.5f, g = 0.5f, b = 0.5f;
			}
			else
			{
				r = 1.f, g = 1.f, b = 1.f;
			}
			AEGfxPrint(pFont, itemDesc[i].c_str(), xN, yN, textDrawSize, r, g, b, 1.f);
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
		AEGfxTextureUnload(ChamberTimeBar.Texture);

		AEGfxMeshFree(fireTimeBar.Mesh);
		AEGfxTextureUnload(fireTimeBar.Texture);

		AEGfxMeshFree(ammoType.Mesh);
		AEGfxTextureUnload(ammoType.Texture);

		AEGfxMeshFree(Coin.Mesh);
		AEGfxTextureUnload(Coin.Texture);

		AEGfxMeshFree(Potion.Mesh);
		AEGfxTextureUnload(Potion.Texture);

		AEGfxMeshFree(PotionBG.Mesh);
		AEGfxTextureUnload(PotionBG.Texture);

		AEGfxMeshFree(PotionFull.Mesh);
		AEGfxTextureUnload(PotionFull.Texture);

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

		AEGfxMeshFree(Vignetting.Mesh);
		AEGfxTextureUnload(Vignetting.Texture);
	}

}