#include "GameManager.h"
#include "Intro.h"
#include "Playing.h"
#include "MainMenu.h"
namespace Manager
{
	GameManager gm;
	InGame::SFXManager SFXManager;
	void GameManager::Init()
	{
		currStateREF = new Intro();
		currStateREF->Init();
		currState = EGameState::TEMP;
		nextState = EGameState::INTRO;
		AEVec2Set(&global::worldMin, -static_cast<f32>(global::ScreenWidth), -static_cast<f32>(global::ScreenHeight));
		AEVec2Set(&global::worldMax, static_cast<f32>(global::ScreenWidth), static_cast<f32>(global::ScreenHeight));
		SFXManager.Init();
	}

	void GameManager::Update()
	{
		AESysFrameStart();
		global::DeltaTime = (f32)AEFrameRateControllerGetFrameTime();

		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		if (currState != nextState || forceRestart)
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
				currStateREF = new MainMenu;
				currStateREF->Init();
				break;
			case EGameState::PLAYING:
				currStateREF = new Playing;
				currStateREF->Init();
				break;
			}
			forceRestart = false;
		}
		currStateREF->Update();
		SFXManager.Update();
	}

	void GameManager::Pause()
	{
		GamePaused = true;
		AEAudioSetGroupVolume(SFXManager.sound_group[InGame::BGM], SFXManager.BGMReduceVol);
	}

	void GameManager::Resume()
	{
		GamePaused = false;
		AEAudioSetGroupVolume(SFXManager.sound_group[InGame::BGM], SFXManager.BGMOriginVol);
	}

	void GameManager::Draw()
	{
		currStateREF->Draw();
		AESysFrameEnd();
	}
	
	void GameManager::Destroy()
	{
		if (currStateREF)
		{
			currStateREF->Destroy();
			delete currStateREF;
			currStateREF = nullptr;
			SFXManager.Destroy();
		}
	}

	void GameManager::SetNextGameState(EGameState state)
	{
		nextState = state;
	}
}