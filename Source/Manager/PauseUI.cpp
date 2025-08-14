#include "PauseUI.h"
#include "HUDController.h"
#include "../Global/GlobalVariables.h"
#include "GameManager.h"
#include "GameOver.h"
#include <sstream>
#include <iomanip>
#include "SettingUI.h"
#include <algorithm>
#include "MainMenu.h"
namespace Manager
{
	PauseUI pausePanel;
	AEGfxVertexList* PauseUI::slotMesh;
	AEGfxTexture* PauseUI::slotTexture;
	static s32 fontSize = 72;
	static f32 textDrawSize = 0.3f;

	void PauseUI::Init(InGame::PlayerCharacter* InPC)
	{
		PC = InPC;

		isActive = false;
		tapPressed = false;
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/button.wav", "button");
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/buttonover.wav", "buttonover");
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		f32 BW{ 400 }, BH{ 90 }, BX{-w/2.f+BW/1.5f}; // button width, button height


		pauseDimmer.Mesh = Utils::CreateMesh();
		pauseDimmer.Texture = AEGfxTextureLoad("Assets/black.png");
		pauseDimmer.position = {0, 0};
		pauseDimmer.size = {w, h};

		buttonMesh = Utils::CreateMesh();
		BbuttonTexture = AEGfxTextureLoad("Assets/Buttons/BButton.png");

		Wbutton.size = { BW,BH };
		Wbutton.Mesh = Utils::CreateMesh();
		Wbutton.Texture = AEGfxTextureLoad("Assets/Buttons/WButton.png");

		f32 bstartY = 200.f;
		f32 bspace = 20.f;
		f32 bspacingY = BH + bspace;
		for (auto& btn : Buttons)
		{
			btn.size = { BW,BH };
			btn.position = { BX, bstartY };
			bstartY -= bspacingY;
		}
		Buttons[0].Init();
		Buttons[0].SetCallback([this]() {
			SFXManager.Play("button");
				gm.Resume();
				isActive = false;
			});

		Buttons[1].Init();
		Buttons[1].SetCallback([]() {
			SFXManager.Play("button");
			SettingPanel.isSettingOn = true;
			});

		Buttons[2].Init();
		Buttons[2].SetCallback([]() {
			gm.SetNextGameState(EGameState::MAINMENU);
			});

		slotMesh = Utils::CreateMesh();
		slotTexture = AEGfxTextureLoad("Assets/black.png");
		const int columns = 4; 
		const float slotWidth = 100.0f;
		const float slotHeight = 100.0f;
		const float spacingX = 10.0f;
		const float spacingY = 10.0f;
		const float startX = -215.f;
		const float startY = -400.f + spacingY;

		for (size_t i = 0; i < ItemSlot.size(); ++i)
		{
			int row = static_cast<int>(i) / columns;
			int col = static_cast<int>(i) % columns;

			ItemSlot[i].position = {
				startX + col * (slotWidth + spacingX),
				startY + row * (slotHeight + spacingY)
			};

			ItemSlot[i].size = { slotWidth, slotHeight };
		}

		float gridRightEdgeX = startX + (columns - 1) * (slotWidth + spacingX) + (slotWidth / 2.0f);
		float screenRightEdgeX = w / 2.0f;

		statsUI.Mesh = Utils::CreateMesh();
		statsUI.Texture = AEGfxTextureLoad("Assets/black.png");
		statsUI.size = { 430.f, 430.f };
		statsUI.position = { (gridRightEdgeX + screenRightEdgeX) / 2.0f, -400.f + statsUI.size.y / 2.f - 40.f};

		tagUI.Mesh = Utils::CreateMesh();
		tagUI.Texture = AEGfxTextureLoad("Assets/black.png");
		tagUI.size = { 430.f, 430.f };
		tagUI.position = { (gridRightEdgeX + screenRightEdgeX) / 2.0f, statsUI.position.y + statsUI.size.y + 10.f};

		slotWhite.Mesh = Utils::CreateMesh();
		slotWhite.Texture = AEGfxTextureLoad("Assets/white.png");
		slotWhite.size = { slotWidth, slotHeight };

		pFont = AEGfxCreateFont("Assets/Fonts/buggy-font.ttf", fontSize);

		m_gunSlot.Mesh = slotMesh;
		m_gunSlot.Texture = slotTexture;
		m_gunSlot.size = { 300.f,150.f };
		m_gunSlot.position = { -w / 2.f + m_gunSlot.size.x/1.2f, -h/2.f + m_gunSlot.size.y/1.2f};

		m_gunIcon.Mesh = slotMesh;
		m_gunIcon.Texture = AEGfxTextureLoad("Assets/test.png");
		m_gunIcon.size = m_gunSlot.size;
		m_gunIcon.position = m_gunSlot.position;

		shouldShowGunName = false;
		prevGunType = PC->HoldingGun->gunType;
	}

	void PauseUI::Update()
	{
		if (gameOverScreen.isGameOver)
			return;

		std::stringstream ss;

		statsString.clear();
		baseStatsString.clear();

		ss << "Damage: " << std::fixed << std::setprecision(2) << PC->PS->effectiveDamage;
		statsString.push_back(ss.str());
		ss.str("");

		ss << " / " << PC->Stats->Damage * PC->GunData->GuntypeDamageRatio;
		baseStatsString.push_back(ss.str());
		ss.str("");

		ss << "Fire Rate: " << PC->PS->effectiveFireRate;
		statsString.push_back(ss.str());
		ss.str("");

		ss << " / " << PC->Stats->FireRate * PC->GunData->GuntypeFireRateRatio;
		baseStatsString.push_back(ss.str());
		ss.str("");

		ss << "Movement Speed: " << static_cast<s32>(PC->PS->effectiveMovementSpeed);
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Hit Count: " << static_cast<s32>(PC->PS->effectiveHitCount);
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Burn Damage: " << std::fixed << std::setprecision(2) << global::effectiveBurnDamage;
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Burn Rate: " << global::effectiveBurnRate;
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Exp Gained: " << global::additionalExpGainedRatio * global::StageExpGainedRatio[global::CurrentStageNumber -1];
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Gold Gained: " << global::additionalGoldGainedRatio * global::StageGoldGainedRatio[global::CurrentStageNumber - 1];
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Additional ProcChance: " << global::additionalProcChanceRatio;
		statsString.push_back(ss.str());
		ss.str("");

		std::map<InGame::ItemTag, s32> itemTagCount;
		tagString.clear();


		for (int i = static_cast<int>(InGame::ENVY); i <= static_cast<int>(InGame::PRIDE); ++i)
		{
			itemTagCount[static_cast<InGame::ItemTag>(i)] = 0;
		}
		for (const auto& item_ptr : PC->inventory)
		{
			itemTagCount[item_ptr.first->tag] += item_ptr.second;
		}
		for (int i = static_cast<int>(InGame::ENVY); i <= static_cast<int>(InGame::PRIDE); ++i)
		{
			if (itemTagCount[static_cast<InGame::ItemTag>(i)] > 0)
			{
				s32 count = itemTagCount[static_cast<InGame::ItemTag>(i)];
				switch (i) {
				case 1:
					if (count >= 3)
					{
						ss << "ENVY+";
						if (count >= 7)
						{
							ss << "7";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "5";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "3";
						}
						ss << " (" << "Minion Damage + ";
						if (count >= 7)
						{
							ss << "1.2";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "0.8";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "0.5";
						}
						ss << ")";
						tagString[static_cast<InGame::ItemTag>(i)] = ss.str();
						ss.str("");
					}

					break;
				case 2:
					if (count >= 3)
					{
						ss << "GLUTTONY+";
						if (count >= 7)
						{
							ss << "7";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "5";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "3";
						}
						ss << " (" << "HP + ";
						if (count >= 7)
						{
							ss << "3";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "2";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "1";
						}
						ss << ")";
						tagString[static_cast<InGame::ItemTag>(i)] = ss.str();
						ss.str("");
					}
					break;
				case 3:
					if (count >= 3)
					{
						ss << "GREED+";
						if (count >= 7)
						{
							ss << "7";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "5";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "3";
						}
						ss << " (" << "Gold Gained + ";
						if (count >= 7)
						{
							ss << "0.4";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "0.25";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "0.1";
						}
						ss << ")";
						tagString[static_cast<InGame::ItemTag>(i)] = ss.str();
						ss.str("");
					}
					break;
				case 4:
					if (count >= 3)
					{
						ss << "LUST+";
						if (count >= 7)
						{
							ss << "7";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "5";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "3";
						}
						ss << " (" << "ProcChance + ";
						if (count >= 7)
						{
							ss << "20%";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "15%";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "7%";
						}
						ss << ")";
						tagString[static_cast<InGame::ItemTag>(i)] = ss.str();
						ss.str("");
					}
					break;
				case 5:
					if (count >= 3)
					{
						ss << "SLOTH+";
						if (count >= 7)
						{
							ss << "7";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "5";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "3";
						}
						ss << " (" << "Hit Count + ";
						if (count >= 7)
						{
							ss << "3";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "2";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "1";
						}
						ss << ")";
						tagString[static_cast<InGame::ItemTag>(i)] = ss.str();
						ss.str("");
					}
					break;
				case 6:
					if (count >= 3)
					{
						ss << "WRATH+";
						if (count >= 7)
						{
							ss << "7";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "5";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "3";
						}
						ss << " (" << "Burn Damage + ";
						if (count >= 7)
						{
							ss << "0.08";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "0.05";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "0.02";
						}
						ss << ")";
						tagString[static_cast<InGame::ItemTag>(i)] = ss.str();
						ss.str("");
					}
					break;
				case 7:
					if (count >= 3)
					{
						ss << "PRIDE+";
						if (count >= 7)
						{
							ss << "7";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "5";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "3";
						}
						ss << " (" << "Fire Rate + ";
						if (count >= 7)
						{
							ss << "0.8";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "0.5";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "0.2";
						}
						ss << ")";
						tagString[static_cast<InGame::ItemTag>(i)] = ss.str();
						ss.str("");
					}
					break;
				default:
					break;
				}
			}
		}
		for (s8 i = 0; i < 3; i++)
		{
			if (!isActive&&tapPressed)
				break;
			Buttons[i].Update();
		}

		for (size_t i = 0; i < ItemSlot.size(); i++)
		{
			if (ItemSlot[i].IsHovered())
			{
				slotWhite.size = ItemSlot[i].size;
				slotWhite.position = ItemSlot[i].position;
			}
		}
		for (size_t i = 0; i < PC->inventory.size(); i++)
		{
			if (ItemSlot[i].IsHovered())
			{
				HUD.TooltipUpdate(*PC->inventory[i].first);
			}
		}

		if (m_gunSlot.IsHovered())
		{
			slotWhite.size = m_gunSlot.size;
			slotWhite.position = m_gunSlot.position;
			shouldShowGunName = true;
		}
		else
		{
			shouldShowGunName = false;
		}
		UpdateGun();
	}

	void PauseUI::Draw()
	{
		if (gameOverScreen.isGameOver)
			return;

		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		//background fading
		Utils::DrawObject(pauseDimmer, false, 0.5f);
		Utils::DrawObject(statsUI, false, 0.5f);
		Utils::DrawObject(tagUI, false, 0.5f);

		for (size_t i = 0; i < ItemSlot.size(); i++)
		{
			Utils::DrawObject(ItemSlot[i], slotTexture, slotMesh, 0.5f);

			if (ItemSlot[i].IsHovered())
			{
				Utils::DrawObject(slotWhite, false, 0.2f);
			}
		}

		for (size_t i = 0; i < PC->inventory.size(); i++)
		{

			std::string pText = "x" + std::to_string(PC->inventory[i].second);

			PC->inventory[i].first->iconPosition = ItemSlot[i].position;
			PC->inventory[i].first->DrawIcon();
			f32 textW, textH;
			AEGfxGetPrintSize(pFont, pText.c_str(), 1.f, &textW, &textH);
			AEGfxPrint(pFont, pText.c_str(),(ItemSlot[i].position.x / (w/2)),(ItemSlot[i].position.y / (h/2))-textH/1.8f, 0.2f, 1, 1, 1, 1);
		}

		for (size_t i = 0; i < statsString.size(); i++)
		{
			f32 spaceX = 10.f;
			f32 spaceY = 20.f;
			f32 textW, textH;
			AEGfxGetPrintSize(pFont, statsString[i].c_str(), 0.2f, &textW, &textH);
			AEGfxPrint(pFont, statsString[i].c_str(), ((statsUI.position.x - statsUI.size.x / 2.f + spaceX) / (w / 2.f)), (((statsUI.position.y + statsUI.size.y / 2.f - spaceX - spaceY  * i) / (h / 2.f)) - textH * (i + 1)), 0.2f, 1, 1, 1, 1);

			if (i <= 1)
			{
				AEGfxPrint(pFont, baseStatsString[i].c_str(), ((statsUI.position.x - statsUI.size.x / 2.f + spaceX) / (w / 2.f) + textW), (((statsUI.position.y + statsUI.size.y / 2.f - spaceX - spaceY * i) / (h / 2.f)) - textH * (i + 1)), 0.2f, 0.5f, 0.5f, 0.5f, 1);
			}
		}

		s32 count = 0;

		for (int i = static_cast<int>(InGame::ENVY); i <= static_cast<int>(InGame::PRIDE); ++i)
		{
			if (tagString[static_cast<InGame::ItemTag>(i)] != "")
			{
				f32 space = 15.f;
				f32 textW, textH;
				AEGfxGetPrintSize(pFont, tagString[static_cast<InGame::ItemTag>(i)].c_str(), 0.2f, &textW, &textH);
				AEGfxPrint(pFont, tagString[static_cast<InGame::ItemTag>(i)].c_str(), ((tagUI.position.x - tagUI.size.x / 2.f + space) / (w / 2.f)), (((tagUI.position.y + tagUI.size.y / 2.f - space * (count + 1)) / (h / 2.f)) - textH * (count + 1)), 0.2f, 1, 1, 1, 1);
				count++;
			}	
		}
		for (int i = 0; i < 3; ++i)
		{
			if (!isActive&&tapPressed)
				break;
			Utils::DrawObject(Buttons[i], BbuttonTexture, buttonMesh, buttonAlpha);
		}

		if (isActive&&!tapPressed)
		{
			bool hovered = false;
			static bool played = false;
			static Button* target = nullptr;
			static Button* prevtarget = nullptr;
			prevtarget = target;
			for (auto& btn : Buttons) {
				if (btn.IsHovered())
				{
					hovered = true; target = &btn; break;
				}
			}

			static f32 start{ 0.f }, end{ 0.f }, sspeed{ 0.f }, espeed{ 0.f };
			static f32 animTime{ 0.f };
			if (target != prevtarget)
			{
				played = false;
				animTime = start = end = sspeed = espeed = 0.f;
			}
			if (target && target == prevtarget && target->IsHovered() && animTime < 1.f)
			{
				if (!played)
				{
					played = true;
					SFXManager.Play("buttonover");
				}
				animTime += global::DeltaTime;
				if (end < 0.5f)
					espeed += global::DeltaTime / 2.f;
				else
					espeed -= global::DeltaTime / 2.f;
				if (animTime > .06f)
				{
					if (start < 0.5f)
						sspeed += global::DeltaTime / 2.f;
					else
						sspeed -= global::DeltaTime / 2.f;
				}
				sspeed = std::clamp(sspeed, 0.01f, 1.f);
				espeed = std::clamp(espeed, 0.01f, 1.f);
				start += sspeed;
				end += espeed;
				start = std::clamp(start, 0.f, 1.f);
				end = std::clamp(end, 0.f, 1.f);
				AEGfxMeshFree(Wbutton.Mesh);
				Wbutton.Mesh = Hekirekiissen(start, end);

				Wbutton.position = target->position;
				Wbutton.size = target->size;
				Utils::DrawObject(Wbutton, Wbutton.Texture, Wbutton.Mesh, 0.5f);
			}
			else if (!hovered)
			{
				played = false;
				animTime = start = end = sspeed = espeed = 0.f;
			}
			int idx = 0;
			f32 lw, lh;
			f32 halfW{ static_cast<f32>(global::ScreenWidth) / 2.f }, halfH{ static_cast<f32>(global::ScreenHeight) / 2.f };
			for (auto& btn : Buttons)
			{
				switch (idx)
				{
				case 0:
					AEGfxGetPrintSize(pFont, "Resume", textDrawSize, &lw, &lh);
					lw *= halfW;
					lh *= halfH;
					AEGfxPrint(pFont, "Resume", (btn.position.x - lw / 2.f) / halfW, (btn.position.y - lh / 2.f) / halfH, textDrawSize, 1, 1, 1, 1);
					break;
				case 1:
					AEGfxGetPrintSize(pFont, "Setting", textDrawSize, &lw, &lh);
					lw *= halfW;
					lh *= halfH;
					AEGfxPrint(pFont, "Setting", (btn.position.x - lw / 2.f) / halfW, (btn.position.y - lh / 2.f) / halfH, textDrawSize, 1, 1, 1, 1);
					break;
				case 2:
					AEGfxGetPrintSize(pFont, "Mainmenu", textDrawSize, &lw, &lh);
					lw *= halfW;
					lh *= halfH;
					AEGfxPrint(pFont, "Mainmenu", (btn.position.x - lw / 2.f) / halfW, (btn.position.y - lh / 2.f) / halfH, textDrawSize, 1, 1, 1, 1);
					break;
				}
				idx++;
			}
		}
		for (int i = 0; i < PC->inventory.size(); i++)
		{
			if (ItemSlot[i].IsHovered())
			{
				HUD.ShowTooltip(*PC->inventory[i].first);
			}
		}
		Utils::DrawObject(m_gunSlot, false, 0.5f);
		Utils::DrawObject(m_gunIcon,false);
		if (shouldShowGunName)
		{
			Utils::DrawObject(slotWhite, false, 0.2f);
			DrawGunName();
			shouldShowGunName = false;
		}
	}

	void PauseUI::UpdateGun()
	{
		if (PC->HoldingGun)
		{
			if (prevGunType != PC->HoldingGun->gunType)
			{
				prevGunType = PC->HoldingGun->gunType;

				if (m_gunIcon.Texture)
				{
					AEGfxTextureUnload(m_gunIcon.Texture);
					m_gunIcon.Texture = nullptr;
				}
				switch (PC->HoldingGun->gunType)
				{
				case InGame::GunType::M1911:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/M1911.png");
					m_gunName = "M1911";
					break;
				case InGame::GunType::CZ75:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/CZ75.png");
					m_gunName = "CZ75";
					break;
				case InGame::GunType::DESERTEGLE:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/DESERTEGLE.png");
					m_gunName = "DESERT EGLE";
					break;
				case InGame::GunType::MP5:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/MP5.png");
					m_gunName = "MP5";
					break;
				case InGame::GunType::MPX:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/MPX.png");
					m_gunName = "MPX";
					break;
				case InGame::GunType::VECTOR:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/VECTOR.png");
					m_gunName = "VECTOR";
					break;
				case InGame::GunType::BEOWOLF:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/BEOWOLF.png");
					m_gunName = "BEOWOLF";
					break;
				case InGame::GunType::P90:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/P90.png");
					m_gunName = "P90";
					break;
				case InGame::GunType::MOSINNAGAT:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/MOSINNAGAT.png");
					m_gunName = "MOSINNAGAT";
					break;
				case InGame::GunType::M24:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/M24.png");
					m_gunName = "M24";
					break;
				case InGame::GunType::RAILGUN:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/RAILGUN.png");
					m_gunName = "RAILGUN";
					break;
				case InGame::GunType::NITRO700:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/NITRO700.png");
					m_gunName = "NITRO700";
					break;
				case InGame::GunType::FNFAL:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/FNFAL.png");
					m_gunName = "FNFAL";
					break;
				case InGame::GunType::M82BARRETT:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/M82BARRETT.png");
					m_gunName = "M82BARRETT";
					break;
				case InGame::GunType::AR15:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/AR15.png");
					m_gunName = "AR15";
					break;
				case InGame::GunType::M110:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/M110.png");
					m_gunName = "M110";
					break;
				case InGame::GunType::BREN:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/BREN.png");
					m_gunName = "BREN";
					break;
				case InGame::GunType::MICROGUN:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/MICROGUN.png");
					m_gunName = "MICROGUN";
					break;
				case InGame::GunType::M249:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/M249.png");
					m_gunName = "M249";
					break;
				case InGame::GunType::M2:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/M2.png");
					m_gunName = "M2";
					break;
				case InGame::GunType::SAWEDOFFSHOTGUN:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/SAWEDOFFSHOTGUN.png");
					m_gunName = "SHORTY";
					break;
				case InGame::GunType::DOUBLEBARREL:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/DOUBLEBARREL.png");
					m_gunName = "DOUBLEBARREL";
					break;
				case InGame::GunType::KS23:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/KS23.png");
					m_gunName = "KS23";
					break;
				case InGame::GunType::M1897:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/M1897.png");
					m_gunName = "M1897";
					break;
				case InGame::GunType::BENELLIM4:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/BENELLIM4.png");
					m_gunName = "BENELLIM4";
					break;
				case InGame::GunType::SAIGA12:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/SAIGA12.png");
					m_gunName = "SAIGA12";
					break;
				case InGame::GunType::AA12:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/Guns/AA12.png");
					m_gunName = "AA12";
					break;
				default:
					m_gunIcon.Texture = AEGfxTextureLoad("Assets/test.png");
					m_gunName = "Gun Name Not Found!!";
					break;
				}
			}
		}
	}

	void PauseUI::DrawGunName()
	{
		f32 lw, lh;
		AEGfxGetPrintSize(pFont, m_gunName.c_str(), textDrawSize, &lw, &lh);
		/*colors[0] = { 1.0f, 1.0f, 1.0f }
		colors[1] = { 0.7f, 0.7f, 1.0f }
		colors[2] = { 1.0f, 0.5f, 1.0f }
		colors[3] = { 0, 0, 0 }*/
		f32 r{ 1.f }, g{ 1.f }, b{ 1.f };
		switch (global::CurrentStageNumber)
		{
		case 1:
			r = 1.f, g = 1.f, b = 1.f;
			break;
		case 2:
			r = 0.7f, g = 0.7f, b = 1.f;
			break;	
		case 3:
			r = 1.0f, g = 0.5f, b = 1.0f;
			break;
		default:
			r = 1.f, g = 1.f, b = 1.f;
			break;
		}
		AEGfxPrint(pFont, m_gunName.c_str(), m_gunIcon.position.x / (global::ScreenWidth / 2.f) - lw/2.f, (m_gunIcon.position.y-m_gunSlot.size.y/2.f + 5.f) / (global::ScreenHeight / 2.f), textDrawSize, r, g, b, 1);
	}

	void PauseUI::Destroy()
	{
		if (buttonMesh)
		{
			AEGfxMeshFree(buttonMesh);
			buttonMesh = nullptr;
		}
		if (BbuttonTexture)
		{
			AEGfxTextureUnload(BbuttonTexture);
			BbuttonTexture = nullptr;
		}

		if (Wbutton.Mesh)
		{
			AEGfxMeshFree(Wbutton.Mesh);
			Wbutton.Mesh = nullptr;
		}
		if (Wbutton.Texture)
		{
			AEGfxTextureUnload(Wbutton.Texture);
			Wbutton.Texture = nullptr;
		}

		if (pauseDimmer.Mesh)
		{
			AEGfxMeshFree(pauseDimmer.Mesh);
			pauseDimmer.Mesh = nullptr;
		}
		if (pauseDimmer.Texture)
		{
			AEGfxTextureUnload(pauseDimmer.Texture);
			pauseDimmer.Texture = nullptr;
		}

		if (statsUI.Mesh)
		{
			AEGfxMeshFree(statsUI.Mesh);
			statsUI.Mesh = nullptr;
		}
		if (statsUI.Texture)
		{
			AEGfxTextureUnload(statsUI.Texture);
			statsUI.Texture = nullptr;
		}

		if (tagUI.Mesh)
		{
			AEGfxMeshFree(tagUI.Mesh);
			tagUI.Mesh = nullptr;
		}
		if (tagUI.Texture)
		{
			AEGfxTextureUnload(tagUI.Texture);
			tagUI.Texture = nullptr;
		}

		if (slotMesh)
		{
			AEGfxMeshFree(slotMesh);
			slotMesh = nullptr;
		}
		if (slotTexture)
		{
			AEGfxTextureUnload(slotTexture);
			slotTexture = nullptr;
		}

		if (slotWhite.Mesh)
		{
			AEGfxMeshFree(slotWhite.Mesh);
			slotWhite.Mesh = nullptr;
		}
		if (slotWhite.Texture)
		{
			AEGfxTextureUnload(slotWhite.Texture);
			slotWhite.Texture = nullptr;
		}

		if (m_gunIcon.Texture)
		{
			AEGfxTextureUnload(m_gunIcon.Texture);
			m_gunIcon.Texture = nullptr;
		}		

		AEGfxDestroyFont(pFont);
		statsString.clear();
		std::vector<std::string>().swap(statsString);
	}
}