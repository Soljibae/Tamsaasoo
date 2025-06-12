#include "GameManager.h"
namespace Manager
{
	GameManager gm;
	void GameManager::Init()
	{
		AEVec2Set(&global::worldMin, -static_cast<f32>(global::ScreenWidth), -static_cast<f32>(global::ScreenHeight));
		AEVec2Set(&global::worldMax, static_cast<f32>(global::ScreenWidth), static_cast<f32>(global::ScreenHeight));
		gm.gs = GameState::Intro;
		intro.Init();
	}

	void GameManager::Update()
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		global::DeltaTime = (f32)AEFrameRateControllerGetFrameTime();
		// Set the background to black.
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		switch (gs)
		{
		case GameState::Intro:
			intro.Update();
			break;
		case GameState::MainMenu:
			break;
		}
	}

	void GameManager::Draw()
	{
		intro.Draw();
		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	
	void GameManager::Destroy()
	{

	}
}