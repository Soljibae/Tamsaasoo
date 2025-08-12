#include "MainMenu.h"
#include "../Utils/Utils.h"
#include "GameManager.h"
#include "../InGame/SFX.h"
#include <algorithm>
#include "SettingUI.h"
namespace Manager
{
	f32 MainMenu::alpha = 1.f;
	static AEVec2 buttonSize{ 500.f, 100.f };
	static f32 buttonPosX = static_cast<f32>(global::ScreenWidth) / 2.f - buttonSize.x*0.8f;
	const static s32 fontSize = 72;
	static const f32 textDrawSize = 0.4f;
	// code to reference for buttons index
	static enum class ButtonType
	{
		START = 0,
		SETTING = 1,
		QUIT = 2,
		LASTBUTTON = QUIT
	};
	AEGfxVertexList* Hekirekiissen(f32 start, f32 end)
	{
		s32 c = 0xFFFFFFFF;
		f32 tstart = std::clamp(start + 0.25f, 0.25f, 1.0f);
		f32 tend = std::clamp(end + 0.25f, 0.25f, 1.0f);
		f32 bstart = std::clamp(start, 0.0f, 0.75f);
		f32 bend = std::clamp(end, 0.0f, 0.75f);
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f + bstart, -0.5f, c, bstart, 1.f,
			-0.5f + bend, -0.5f, c, bend, 1.f,
			-0.5f + tstart, 0.5f, c, tstart, 0.f
			);
		AEGfxTriAdd(
			-0.5f + bend, -0.5f, c, bend, 1.f,
			-0.5f + tend, 0.5f, c, tend, 0.f,
			-0.5f + tstart, 0.5f, c, tstart, 0.f
		);
		return AEGfxMeshEnd();
	}
	void MainMenu::Init()
	{
		SFXManager.Init();
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/start.wav", "start");
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/button.wav", "button");
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/buttonover.wav", "buttonover");
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);

		Illust.Mesh = Utils::CreateMesh();
		Illust.Texture = AEGfxTextureLoad("Assets/UI/Illust.png");
		Illust.position = { 0.f, 0.f };
		Illust.size = { w, h };

		Title.Mesh = Utils::CreateMesh();
		Title.Texture = AEGfxTextureLoad("Assets/UI/title2.png");
		Title.size = { 600, 200 };
		Title.position = { w/2.f - Title.size.x/1.5f, h/2.f - Title.size.y};

		Black.Mesh = Utils::CreateMesh();
		Black.Texture = AEGfxTextureLoad("Assets/Black.png");
		Black.position = { 0.f, 0.f };
		Black.size = { w, h };

		buttonMesh = Utils::CreateMesh();
		BbuttonTexture = AEGfxTextureLoad("Assets/Buttons/BButton.png");

		Wbutton.size = buttonSize;
		Wbutton.Mesh = Utils::CreateMesh();
		Wbutton.Texture = AEGfxTextureLoad("Assets/Buttons/WButton.png");

		f32 startY = 0.f;
		f32 space = 10.f;
		f32 spacingY = buttonSize.y + space;
		for (auto& btn : Buttons)
		{
			btn.size = buttonSize;
			btn.position = { buttonPosX, startY };
			startY -= spacingY;
		}
		Buttons[0].Init();
		Buttons[0].SetCallback([]() {
			SFXManager.Play("start");
			});

		Buttons[1].Init();
		Buttons[1].SetCallback([]() {
			SFXManager.Play("button");
			SettingPanel.isSettingOn = true;
			});

		Buttons[2].Init();
		Buttons[2].SetCallback([]() {
			gm.shouldExit = true;
			});
		pFont = AEGfxCreateFont("Assets/Fonts/buggy-font.ttf", fontSize);
	}

	void MainMenu::Update()
	{
		if (fadeInTime < fadeInDuration)
		{
			fadeInTime += global::DeltaTime;
			alpha = 1.0f - (fadeInTime / fadeInDuration);
		}
		else
		{
			alpha = 0;
			Buttons[0].Update();
			static bool shouldstart{ false };
			if (Buttons[0].IsClicked())
			{
				shouldstart = true;
			}
			if (shouldstart)
			{
				alpha = 1.f;
				static f32 startdelaytime = 0.f;
				const f32 delay = 1.f;
				startdelaytime += global::DeltaTime;
				if (startdelaytime > delay)
				{
					gm.SetNextGameState(EGameState::PLAYING);
					startdelaytime = 0.f;
					shouldstart = false;
				}
			}
			Buttons[1].Update();
			Buttons[2].Update();
			SFXManager.Update();
		}
	}
	void MainMenu::Draw()
	{
		Utils::DrawObject(Illust, false);
		if (alpha != 0)
			Utils::DrawObject(Black, false, alpha);
		if (alpha > 0)
			return;
		Utils::DrawObject(Title, false);
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
			if(end < 0.5f)
				espeed += global::DeltaTime/2.f;
			else
				espeed -= global::DeltaTime / 2.f;
			if(animTime > .06f)
			{
				if(start < 0.5f)
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
			target = nullptr;
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
				AEGfxGetPrintSize(pFont, "Start", textDrawSize, &lw, &lh);
				lw *= halfW;
				lh *= halfH;
				AEGfxPrint(pFont, "Start",(btn.position.x - lw/2.f)/halfW, (btn.position.y - lh/2.f)/halfH,textDrawSize,1,1,1,1);
				break;
			case 1:
				AEGfxGetPrintSize(pFont, "Setting", textDrawSize, &lw, &lh);
				lw *= halfW;
				lh *= halfH;
				AEGfxPrint(pFont, "Setting",(btn.position.x - lw/2.f)/halfW, (btn.position.y - lh/2.f)/halfH,textDrawSize,1,1,1,1);
				break;
			case 2:
				AEGfxGetPrintSize(pFont, "Quit", textDrawSize, &lw, &lh);
				lw *= halfW;
				lh *= halfH;
				AEGfxPrint(pFont, "Quit",(btn.position.x - lw/2.f)/halfW, (btn.position.y - lh/2.f)/halfH,textDrawSize,1,1,1,1);
				break;
			}
			idx++;
		}
	}
	void MainMenu::Destroy()
	{
		AEGfxMeshFree(Illust.Mesh);
		AEGfxTextureUnload(Illust.Texture);

		AEGfxMeshFree(Title.Mesh);
		AEGfxTextureUnload(Title.Texture);

		AEGfxMeshFree(Black.Mesh);
		AEGfxTextureUnload(Black.Texture);

		AEGfxMeshFree(buttonMesh);
		AEGfxTextureUnload(BbuttonTexture);

		AEGfxMeshFree(Wbutton.Mesh);
		AEGfxTextureUnload(Wbutton.Texture);

		AEGfxDestroyFont(pFont);

		SFXManager.Destroy();
	}
}