#include "MainMenu.h"
#include "../Utils/Utils.h"
#include "GameManager.h"
#include "../InGame/SFX.h"
namespace Manager
{
	f32 MainMenu::alpha = 1.f;
	static AEVec2 buttonSize{ 500.f, 100.f };
	static f32 buttonPosX = static_cast<f32>(global::ScreenWidth) / 2.f - buttonSize.x*0.8f;
	// code to reference for buttons index
	static enum class ButtonType
	{
		START = 0,
		SETTING = 1,
		QUIT = 2,
		LASTBUTTON = QUIT
	};
	static AEGfxVertexList* Hekirekiissen(f32 start, f32 end)
	{
		s32 c = 0xFFFFFFFF;
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, c, start, 1.f,
			0.5f, -0.5f, c, end, 1.f,
			-0.5f, 0.5f, c, start, 0.f
			);
		AEGfxTriAdd(
			0.5f, -0.5f, c, end, 1.f,
			0.5f, 0.5f, c, end, 0.f,
			-0.5f, 0.5f, c, start, 0.f
		);
		return AEGfxMeshEnd();
	}
	void MainMenu::Init()
	{
		SFXManager.Init();
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/button.wav", "button");
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);

		Illust.Mesh = Utils::CreateMesh();
		Illust.Texture = AEGfxTextureLoad("Assets/MainTemp.png");
		Illust.position = { 0.f, 0.f };
		Illust.size = { w, h };

		Black.Mesh = Utils::CreateMesh();
		Black.Texture = AEGfxTextureLoad("Assets/Black.png");
		Black.position = { 0.f, 0.f };
		Black.size = { w, h };

		buttonMesh = Utils::CreateMesh();
		BbuttonTexture = AEGfxTextureLoad("Assets/Buttons/BButton.png");

		Wbutton.size = buttonSize;
		Wbutton.Mesh = Utils::CreateMesh();
		Wbutton.Texture = AEGfxTextureLoad("Assets/Buttons/WButton.png");

		Buttons[0].size = buttonSize;
		Buttons[0].position = { buttonPosX, 100.f };
		Buttons[0].SetCallback([]() {
			SFXManager.Play("button");
			});
		Buttons[0].Init();

		Buttons[1].position = { buttonPosX, -100.f };
		Buttons[1].size = buttonSize;
		Buttons[1].SetCallback([]() {
			SFXManager.Play("button");
			gm.shouldExit = true;
			});
		Buttons[1].Init();

		Buttons[2].position = { buttonPosX, -300.f };
		Buttons[2].size = buttonSize;
		Buttons[2].SetCallback([]() {
			SFXManager.Play("button");
			gm.shouldExit = true;
			});
		Buttons[2].Init();
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
		Utils::DrawObject(Buttons[0], BbuttonTexture, buttonMesh, buttonAlpha);
		Utils::DrawObject(Buttons[1], BbuttonTexture, buttonMesh, buttonAlpha);
		Utils::DrawObject(Buttons[2], BbuttonTexture, buttonMesh, buttonAlpha);
		static f32 animateTime{ 0.f };
		static AEVec2 lightRange{0.f, 0.f};
		static AEVec2 lightSpeed{0.f, 0.f};
		for (auto button : Buttons)
		{
			if (button.IsHovered())
			{
				animateTime += global::DeltaTime;
				AEGfxMeshFree(Wbutton.Mesh);
				Wbutton.Mesh = Hekirekiissen(lightRange.x, lightRange.y);
				if (animateTime < 2.f)
				{
					lightRange.x += lightSpeed.x;
					lightRange.y += lightSpeed.y;
				}
				Wbutton.position = button.position;
				Wbutton.size = button.size;
				Utils::DrawObject(Wbutton, Wbutton.Texture, Wbutton.Mesh, buttonAlpha);
			}
			else
			{
				animateTime = 0.f;

				static AEVec2 lightRange{ 0.f, 0.f };
			}
		}

		if (alpha != 0)
		{
			Utils::DrawObject(Black, false, alpha);
		}
	}
	void MainMenu::Destroy()
	{
		AEGfxMeshFree(Illust.Mesh);
		AEGfxTextureUnload(Illust.Texture);

		AEGfxMeshFree(Black.Mesh);
		AEGfxTextureUnload(Black.Texture);

		AEGfxMeshFree(buttonMesh);
		AEGfxTextureUnload(BbuttonTexture);
		AEGfxTextureUnload(Wbutton.Texture);

		SFXManager.Destroy();
	}
}