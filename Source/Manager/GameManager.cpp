#include "GameManager.h"
namespace Manager
{
	GameManager gm;

	void GameManager::Init()
	{
		currStateREF = new Intro();
		currStateREF->Init();
		currState = EGameState::TEMP;
		nextState = EGameState::INTRO;
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