#include "GameOver.h"
#include "../Global/GlobalVariables.h"
#include "GameManager.h"
#include "../Utils/Utils.h"
#include "MainMenu.h"
#include <algorithm>
namespace Manager
{
	f32 w = static_cast<f32>(global::ScreenWidth);
	f32 h = static_cast<f32>(global::ScreenHeight);
	GameOver gameOverScreen;
	f32 buttonOffset = 400.f;
	static f32 overFontSize = 120.f;
	static f32 overTextDrawSize = 1.f;
	static f32 fontSize = 72.f;
	static f32 textDrawSize = 0.25f;
	static AEVec2 buttonSize = { 400,100 };
	void GameOver::Init()
	{
		BG.size = { w, h };
		BG.position = { 0,0 };
		BG.Mesh = Utils::CreateMesh();
		BG.Texture = AEGfxTextureLoad("Assets/black.png");
		buttonMesh = Utils::CreateMesh();
		BbuttonTexture = AEGfxTextureLoad("Assets/Buttons/BButton.png");

		Wbutton.size = buttonSize;
		Wbutton.Mesh = Utils::CreateMesh();
		Wbutton.Texture = AEGfxTextureLoad("Assets/Buttons/WButton.png");

		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/button.wav", "button");
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/buttonover.wav", "buttonover");

		f32 startX = -200.f;
		f32 startY = -100.f;
		f32 spacingX = 500.f;
		for (auto& btn : Buttons)
		{
			btn.size = buttonSize;
			btn.position = { startX, startY };
			startX += spacingX;
		}
		Buttons[0].Init();
		Buttons[0].SetCallback([this]() {
			SFXManager.Play("button");
			isGameOver = false;
			gm.SetNextGameState(EGameState::MAINMENU);
			});

		Buttons[1].Init();
		Buttons[1].SetCallback([this]() {
			SFXManager.Play("button");
			isGameOver = false;
			gm.forceRestart = true;
			});

		BGalpha = 0.f;
		fadeoutTime = 0.f;
		fadeoutDuration = 3.f;
		buttonShowDelay = 2.f;
		pFont = AEGfxCreateFont("Assets/Fonts/buggy-font.ttf", overFontSize);
	}
	void GameOver::Update()
	{
		if (!isGameOver)
			return;
		///////
		if (w != global::ScreenWidth || h != global::ScreenHeight)
		{
			w = global::ScreenWidth;
			h = global::ScreenHeight;
			BG.size = { w, h };
		}
		///////

		fadeoutTime += global::DeltaTime;
		if (fadeoutTime < fadeoutDuration)
		{
			BGalpha += global::DeltaTime / (fadeoutDuration * 1.5f);
		}
		if (fadeoutTime > fadeoutDuration + buttonShowDelay)
		{
			for (auto& btn : Buttons) { btn.Update(); }
		}
	}
	void GameOver::Draw()
	{
		if (!isGameOver)
			return;
		Utils::DrawObject(BG, false, BGalpha);
		if (fadeoutTime > fadeoutDuration)
		{
			f32 halfW = w / 2.f, halfH = h / 2.f;
			f32 lw{ 0 }, lh{ 0 };
			AEGfxGetPrintSize(pFont, "Game Over", overTextDrawSize, &lw, &lh);
			AEGfxPrint(pFont, "Game Over", -lw/2.f, 200.f / halfH, overTextDrawSize, 1.f, 0.f, 0.f, 1.f);
		}
		if (fadeoutTime > fadeoutDuration + buttonShowDelay)
		{
			for (auto& btn : Buttons) { Utils::DrawObject(btn, BbuttonTexture, buttonMesh, buttonAlpha); }
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
				Wbutton.Mesh = ButtonGlossing(start, end);

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
					AEGfxGetPrintSize(pFont, "GiveUp", textDrawSize, &lw, &lh);
					lw *= halfW;
					lh *= halfH;
					AEGfxPrint(pFont, "GiveUp", (btn.position.x - lw / 2.f) / halfW, (btn.position.y - lh / 2.f) / halfH, textDrawSize, 1, 1, 1, 1);
					break;
				case 1:
					AEGfxGetPrintSize(pFont, "Retry", textDrawSize, &lw, &lh);
					lw *= halfW;
					lh *= halfH;
					AEGfxPrint(pFont, "Retry", (btn.position.x - lw / 2.f) / halfW, (btn.position.y - lh / 2.f) / halfH, textDrawSize, 1, 1, 1, 1);
					break;
				}
				idx++;
			}

		}
	}
	void GameOver::Destroy()
	{
		AEGfxMeshFree(BG.Mesh);
		AEGfxTextureUnload(BG.Texture);
		BG.Mesh = nullptr;
		BG.Texture = nullptr;

		AEGfxMeshFree(buttonMesh);
		AEGfxTextureUnload(BbuttonTexture);

		AEGfxMeshFree(Wbutton.Mesh);
		AEGfxTextureUnload(Wbutton.Texture);

		AEGfxDestroyFont(pFont);
	}
}