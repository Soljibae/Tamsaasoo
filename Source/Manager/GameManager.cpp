#include "GameManager.h"
#include "Intro.h"
#include "Playing.h"
namespace Manager
{
	GameManager gm;
	void GameManager::Init()
	{
		currStateREF = new Intro();
		currStateREF->Init();
		currState = EGameState::TEMP;
		nextState = EGameState::PLAYING;
		AEVec2Set(&global::worldMin, -static_cast<f32>(global::ScreenWidth), -static_cast<f32>(global::ScreenHeight));
		AEVec2Set(&global::worldMax, static_cast<f32>(global::ScreenWidth), static_cast<f32>(global::ScreenHeight));
	}

	void GameManager::Update()
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		global::DeltaTime = (f32)AEFrameRateControllerGetFrameTime();
		// Set the background to black.
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		if (currState != nextState)
		{
			currStateREF->Destroy();
			delete currStateREF;
			currState = nextState;
			switch (currState)
			{
			case EGameState::INTRO:
				currStateREF = new Intro;
				currStateREF->Init();
				break;
			case EGameState::MAINMENU:
				break;
			case EGameState::PLAYING:
				currStateREF = new Playing;
				currStateREF->Init();
				break;
			}
			
		}
		currStateREF->Update();
	}

	void GameManager::Draw()
	{
		currStateREF->Draw();
		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	
	void GameManager::Destroy()
	{

	}
}