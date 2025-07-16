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
		f32 bspace = 10.f;
		f32 bspacingY = BH + bspace;
		for (auto& btn : Buttons)
		{
			btn.size = { BW,BH };
			btn.position = { BX, bstartY };
			bstartY -= bspacingY;
		}
		Buttons[0].Init();
		Buttons[0].SetCallback([]() {
			SFXManager.Play("button");
			gm.Resume();
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
		const int columns = 4; // 한 줄에 몇 개
		const float slotWidth = 100.0f;
		const float slotHeight = 100.0f;
		const float spacingX = 10.0f; // 가로 간격
		const float spacingY = 10.0f; // 세로 간격
		const float startX = -215.f; // 전체 위치 조정
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

		PC = InPC;
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
		for (auto& btn : Buttons)
		{
			btn.Update();
		}

		for (size_t i = 0; i < PC->inventory.size(); i++)
		{
			if (ItemSlot[i].IsHovered())
				HUD.TooltipUpdate(*PC->inventory[i].first);
		}
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
				slotWhite.position = ItemSlot[i].position;
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
			Utils::DrawObject(Buttons[i], BbuttonTexture, buttonMesh, buttonAlpha);

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

		for (int i = 0; i < PC->inventory.size(); i++)
		{
			if (ItemSlot[i].IsHovered())
			{
				HUD.ShowTooltip(*PC->inventory[i].first);
			}
		}
	}

	void PauseUI::Destroy()
	{
		AEGfxMeshFree(buttonMesh);
		AEGfxTextureUnload(BbuttonTexture);

		AEGfxMeshFree(Wbutton.Mesh);
		AEGfxTextureUnload(Wbutton.Texture);

		AEGfxMeshFree(pauseDimmer.Mesh);
		AEGfxTextureUnload(pauseDimmer.Texture);

		AEGfxMeshFree(statsUI.Mesh);
		AEGfxTextureUnload(statsUI.Texture);

		AEGfxMeshFree(tagUI.Mesh);
		AEGfxTextureUnload(tagUI.Texture);

		AEGfxMeshFree(slotMesh);
		AEGfxTextureUnload(slotTexture);

		AEGfxMeshFree(slotWhite.Mesh);
		AEGfxTextureUnload(slotWhite.Texture);

		AEGfxDestroyFont(pFont);
		statsString.clear();
		std::vector<std::string>().swap(statsString);
	}
}