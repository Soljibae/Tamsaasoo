#include "PauseUI.h"


namespace Manager
{
	PauseUI pausePanel;
	AEGfxVertexList* PauseUI::slotMesh;
	AEGfxVertexList* PauseUI::slotTexture;

	void PauseUI::Init(InGame::PlayerCharacter* InPC)
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		f32 BW{300}, BH{120}; // button width, button height
		resumeButton.Mesh = Utils::CreateMesh();
		resumeButton.Texture = AEGfxTextureLoad("Assets/ResumeButton.png");
		resumeButton.position = {-500, 300};
		resumeButton.size = { BW / 3, BH / 3 };
		resumeButton.SetCallback([]() { gm.Resume(); });

		mainmenuButton.Mesh = Utils::CreateMesh();
		mainmenuButton.Texture = AEGfxTextureLoad("Assets/MainMenuButton.png");
		mainmenuButton.position = {-500, 150};
		mainmenuButton.size = { BW / 3, BH / 3 };
		mainmenuButton.SetCallback([]() { gm.SetNextGameState(EGameState::MAINMENU); });

		pauseDimmer.Mesh = Utils::CreateMesh();
		pauseDimmer.Texture = AEGfxTextureLoad("Assets/black.png");
		pauseDimmer.position = {0, 0};
		pauseDimmer.size = {w, h};

		slotMesh = Utils::CreateMesh();
		slotTexture = AEGfxTextureLoad("Assets/white.png");
		for (size_t i = 0; i < ItemSlot.size(); i++)
		{
			ItemSlot[i].position = {0, 0};
			ItemSlot[i].size = {50, 50};
		}
		PC = InPC;
	}

	void PauseUI::Update()
	{
		resumeButton.Update();
		mainmenuButton.Update();
	}

	void PauseUI::Draw()
	{
		Utils::DrawObject(pauseDimmer, false, 0.5f);

		for (size_t i = 0; i < ItemSlot.size(); i++)
		{
			Utils::DrawObject(ItemSlot[i], slotTexture, slotMesh);
			if (ItemSlot[i].IsHovered())
			{
				slotWhite.position = ItemSlot[i].position;
				Utils::DrawObject(slotWhite, false, 0.3f);
			}
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
	}
}