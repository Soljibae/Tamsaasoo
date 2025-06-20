#include "PauseUI.h"


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
	}

	void PauseUI::Draw()
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		//background fading
		Utils::DrawObject(pauseDimmer, false, 0.5f);

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

			PC->inventory[i].first->position = ItemSlot[i].position;
			PC->inventory[i].first->DrawIcon();
			f32 textW, textH;
			AEGfxGetPrintSize(pFont, pText.c_str(), 1.f, &textW, &textH);
			AEGfxPrint(pFont, pText.c_str(),(ItemSlot[i].position.x / (w/2)),(ItemSlot[i].position.y / (h/2))-textH/1.8f, 0.2f, 1, 1, 1, 1);
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

		AEGfxMeshFree(slotMesh);
		AEGfxTextureUnload(slotTexture);

		AEGfxMeshFree(slotWhite.Mesh);
		AEGfxTextureUnload(slotWhite.Texture);

		AEGfxDestroyFont(pFont);
	}
}