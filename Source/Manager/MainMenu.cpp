#include "MainMenu.h"
#include "../Utils/Utils.h"
#include "GameManager.h"
namespace Manager
{
	f32 MainMenu::alpha = 1.f;

	void MainMenu::Init()
	{
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

		startButton.Mesh = Utils::CreateMesh();
		startButton.Texture = AEGfxTextureLoad("Assets/Buttons/StartButton.png");
		startButton.position = { 0.f, 100.f };
		startButton.size = { 300, 120 };
		startButton.SetCallback([]() {gm.SetNextGameState(EGameState::PLAYING); });
		startButton.Init();

		exitButton.Mesh = Utils::CreateMesh();
		exitButton.Texture = AEGfxTextureLoad("Assets/Buttons/ExitButton.png");
		exitButton.position = { 0.f, -100.f };
		exitButton.size = { 300, 120 };
		exitButton.SetCallback([]() {gm.shouldExit = true; });
		exitButton.Init();
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
			startButton.Update();
			exitButton.Update();
		}
	}
	void MainMenu::Draw()
	{
		Utils::DrawObject(Illust, false);
		Utils::DrawObject(startButton, false);
		Utils::DrawObject(exitButton, false);
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

		AEGfxMeshFree(startButton.Mesh);
		AEGfxTextureUnload(startButton.Texture);

		AEGfxMeshFree(exitButton.Mesh);
		AEGfxTextureUnload(exitButton.Texture);
	}
}