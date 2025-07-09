#include "GameOver.h"
#include "../Global/GlobalVariables.h"
#include "GameManager.h"
#include "../Utils/Utils.h"
namespace Manager
{
	f32 w = static_cast<f32>(global::ScreenWidth);
	f32 h = static_cast<f32>(global::ScreenHeight);
	GameOver gameOverScreen;
	static f32 startX = -200.f;
	static f32 startY = -100.f;
	f32 buttonOffset = 400.f;
	f32 fontSize = 120.f;
	f32 textDrawSize = 1.f;
	void GameOver::Init()
	{
		BG.size = { w, h };
		BG.position = { 0,0 };
		BG.Mesh = Utils::CreateMesh();
		BG.Texture = AEGfxTextureLoad("Assets/black.png");
		buttons.MainMenu.size = { 200.f, 100.f };
		buttons.MainMenu.position = { startX, startY};
		buttons.MainMenu.Mesh = Utils::CreateMesh();
		buttons.MainMenu.Texture = AEGfxTextureLoad("Assets/Buttons/MainMenuButton.png");
		buttons.MainMenu.SetCallback([this]() {
			isGameOver = false;
			gm.SetNextGameState(EGameState::MAINMENU);
			});
		buttons.MainMenu.Init();

		buttons.Retry.size = { 300.f, 100.f };
		buttons.Retry.position = { startX + buttonOffset, startY };
		buttons.Retry.Mesh = Utils::CreateMesh();
		buttons.Retry.Texture = AEGfxTextureLoad("Assets/Buttons/RetryButton.png");
		buttons.Retry.SetCallback([this]() {
			isGameOver = false;
			gm.forceRestart = true;
			});
		buttons.Retry.Init();
		BGalpha = 0.f;
		fadeoutTime = 0.f;
		fadeoutDuration = 3.f;
		buttonShowDelay = 2.f;
		pFont = AEGfxCreateFont("Assets/buggy-font.ttf", fontSize);
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
			buttons.MainMenu.Update();
			buttons.Retry.Update();
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
			AEGfxGetPrintSize(pFont, "Game Over", textDrawSize, &lw, &lh);
			AEGfxPrint(pFont, "Game Over", -lw/2.f, 200.f / halfH, textDrawSize, 1.f, 0.f, 0.f, 1.f);
		}
		if (fadeoutTime > fadeoutDuration + buttonShowDelay)
		{
			Utils::DrawObject(buttons.MainMenu, false);
			Utils::DrawObject(buttons.Retry, false);
		}
	}
	void GameOver::Destroy()
	{
		AEGfxMeshFree(BG.Mesh);
		AEGfxTextureUnload(BG.Texture);
		BG.Mesh = nullptr;
		BG.Texture = nullptr;
		AEGfxMeshFree(buttons.MainMenu.Mesh);
		AEGfxTextureUnload(buttons.MainMenu.Texture);
		buttons.MainMenu.Mesh = nullptr;
		buttons.MainMenu.Texture = nullptr;
		AEGfxMeshFree(buttons.Retry.Mesh);
		AEGfxTextureUnload(buttons.Retry.Texture);
		buttons.Retry.Mesh = nullptr;
		buttons.Retry.Texture = nullptr;

		AEGfxDestroyFont(pFont);
	}
}