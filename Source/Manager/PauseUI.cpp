#include "PauseUI.h"
#include "HUDController.h"
#include "../Global/GlobalVariables.h"
#include "GameManager.h"
#include "GameOver.h"
#include <sstream>
#include <iomanip>

namespace Manager
{
	PauseUI pausePanel;
	AEGfxVertexList* PauseUI::slotMesh;
	AEGfxTexture* PauseUI::slotTexture;

	void PauseUI::Init(InGame::PlayerCharacter* InPC)
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		f32 BW{300}, BH{120}; // button width, button height
		resumeButton.Mesh = Utils::CreateMesh();
		resumeButton.Texture = AEGfxTextureLoad("Assets/Buttons/ResumeButton.png");
		resumeButton.position = {-600, 300};
		resumeButton.size = { BW , BH };
		resumeButton.SetCallback([]() { gm.Resume(); });

		mainmenuButton.Mesh = Utils::CreateMesh();
		mainmenuButton.Texture = AEGfxTextureLoad("Assets/Buttons/MainMenuButton.png");
		mainmenuButton.position = {-600, 150};
		mainmenuButton.size = { BW , BH };
		mainmenuButton.SetCallback([]() { gm.SetNextGameState(EGameState::MAINMENU); });

		pauseDimmer.Mesh = Utils::CreateMesh();
		pauseDimmer.Texture = AEGfxTextureLoad("Assets/black.png");
		pauseDimmer.position = {0, 0};
		pauseDimmer.size = {w, h};
		
		slotMesh = Utils::CreateMesh();
		slotTexture = AEGfxTextureLoad("Assets/black.png");
		const int columns = 4; // 한 줄에 몇 개
		const float slotWidth = 100.0f;
		const float slotHeight = 100.0f;
		const float spacingX = 10.0f; // 가로 간격
		const float spacingY = 10.0f; // 세로 간격
		const float startX = -300.f; // 전체 위치 조정
		const float startY = 0.f;

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
		statsUI.position = { (gridRightEdgeX + screenRightEdgeX) / 2.0f, - statsUI.size.y / 2.f + 50.f };

		tagUI.Mesh = Utils::CreateMesh();
		tagUI.Texture = AEGfxTextureLoad("Assets/black.png");
		tagUI.size = { 430.f, 320.f };
		tagUI.position = { (gridRightEdgeX + screenRightEdgeX) / 2.0f, tagUI.size.y / 2.f + 60.f };

		slotWhite.Mesh = Utils::CreateMesh();
		slotWhite.Texture = AEGfxTextureLoad("Assets/white.png");
		slotWhite.size = { slotWidth, slotHeight };

		pFont = AEGfxCreateFont("Assets/buggy-font.ttf", 72.f);

		PC = InPC;
	}

	void PauseUI::Update()
	{
		if (gameOverScreen.isGameOver)
			return;
		resumeButton.Update();
		mainmenuButton.Update();

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
						ss << " (" << "Movement speed + ";
						if (count >= 7)
						{
							ss << "60";
						}
						else if (6 >= count && count >= 5)
						{
							ss << "35";
						}
						else if (4 >= count && count >= 3)
						{
							ss << "20";
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

			for (size_t i = 0; i < PC->inventory.size(); i++)
			{
				if (ItemSlot[i].IsHovered())
					HUD.TooltipUpdate(*PC->inventory[i].first);
			}
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

		//buttons
		Utils::DrawObject(resumeButton, false);
		Utils::DrawObject(mainmenuButton, false);
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
		AEGfxMeshFree(resumeButton.Mesh);
		AEGfxTextureUnload(resumeButton.Texture);

		AEGfxMeshFree(mainmenuButton.Mesh);
		AEGfxTextureUnload(mainmenuButton.Texture);

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