#include "PauseUI.h"
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
		resumeButton.Texture = AEGfxTextureLoad("Assets/ResumeButton.png");
		resumeButton.position = {-600, 300};
		resumeButton.size = { BW , BH };
		resumeButton.SetCallback([]() { gm.Resume(); });

		mainmenuButton.Mesh = Utils::CreateMesh();
		mainmenuButton.Texture = AEGfxTextureLoad("Assets/MainMenuButton.png");
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
		statsUI.size = { 320.f, 430.f };
		statsUI.position = { (gridRightEdgeX + screenRightEdgeX) / 2.0f, - statsUI.size.y / 2.f + 50.f };

		slotWhite.Mesh = Utils::CreateMesh();
		slotWhite.Texture = AEGfxTextureLoad("Assets/white.png");
		slotWhite.size = { slotWidth, slotHeight };

		pFont = AEGfxCreateFont("Assets/buggy-font.ttf", 72.f);

		PC = InPC;
	}

	void PauseUI::Update()
	{
		resumeButton.Update();
		mainmenuButton.Update();

		std::stringstream ss;

		statsString.clear();

		ss << "Damage: " << std::fixed << std::setprecision(2) << PC->Stats.effectiveDamage;
		statsString.push_back(ss.str());
		ss.str("");
		
		ss << "Fire Rate: " << PC->Stats.effectiveFireRate;
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Movement Speed: " << static_cast<s32>(PC->Stats.effectiveMovementSpeed);
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Hit Count: " << static_cast<s32>(PC->Stats.effectiveHitCount);
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Burn Damage: " << std::fixed << std::setprecision(2) << global::effectiveBurnDamage;
		statsString.push_back(ss.str());
		ss.str("");

		ss << "Burn Rate: " <<  global::effectiveBurnRate;
		statsString.push_back(ss.str());
		ss.str("");
	}

	void PauseUI::Draw()
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		//background fading
		Utils::DrawObject(pauseDimmer, false, 0.5f);
		Utils::DrawObject(statsUI, false, 0.5f);

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
			f32 space = 10.f;
			f32 textW, textH;
			AEGfxGetPrintSize(pFont, statsString[i].c_str(), 0.22f, &textW, &textH);
			AEGfxPrint(pFont, statsString[i].c_str(), ((statsUI.position.x - statsUI.size.x / 2.f + space) / (w / 2.f)), (((statsUI.position.y + statsUI.size.y / 2.f - space * (i + 1)) / (h / 2.f)) - textH * (i + 1)), 0.22f, 1, 1, 1, 1);
			
		}

		//buttons
		Utils::DrawObject(resumeButton, false);
		Utils::DrawObject(mainmenuButton, false);
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

		AEGfxMeshFree(slotMesh);
		AEGfxTextureUnload(slotTexture);

		AEGfxMeshFree(slotWhite.Mesh);
		AEGfxTextureUnload(slotWhite.Texture);

		AEGfxDestroyFont(pFont);

		statsString.clear();
	}
}