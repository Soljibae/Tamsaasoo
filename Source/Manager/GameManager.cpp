#include "GameManager.h"
namespace Manager
{
	GameManager gm;
	void GameManager::Init()
	{
		gm.gs = GameState::Game;
		intro.Init();
		Game.Init();
	}

	void GameManager::Update()
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		//std::cout << static_cast<int>(gm.gs);
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
		case GameState::Game:
			Game.Update();
			break;
		}
	}

	void GameManager::Draw()
	{
		switch (gs)
		{
		case GameState::Intro:
			intro.Draw();
			break;
		case GameState::MainMenu:
			break;
		case GameState::Game:
			Game.Draw();
			break;
		}
		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	
	void GameManager::Destroy()
	{

	}
}