#include "GameManager.h"
#include "Intro.h"
#include "Playing.h"
#include "MainMenu.h"
#include "SettingUI.h"

namespace Manager
{
	GameManager gm;
	InGame::SFXManager SFXManager;
	GameManager::Cursor cursor;
	void GameManager::Init()
	{
		cursor.Init();
		currStateREF = new Intro();
		currStateREF->Init();
		currState = EGameState::TEMP;
		nextState = EGameState::INTRO;
		SettingPanel.Init();
		SettingUI::StaticInit();
		AEVec2Set(&global::worldMin, -static_cast<f32>(global::ScreenWidth), -static_cast<f32>(global::ScreenHeight));
		AEVec2Set(&global::worldMax, static_cast<f32>(global::ScreenWidth), static_cast<f32>(global::ScreenHeight));
	}

	void GameManager::Update()
	{
		AESysFrameStart();
		cursor.Update();
		global::DeltaTime = (f32)AEFrameRateControllerGetFrameTime();

		if (SettingPanel.isFullScreen)
		{
			if(!AESysIsFullScreen())
				AESysSetFullScreen(true);
		}
		else
		{
			if (AESysIsFullScreen())
				AESysSetFullScreen(false);
		}

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
		if (AEInputCheckCurr(AEVK_F1)) //to do
			SettingPanel.isSettingOn = true;

		SettingPanel.Update();

		if(!SettingPanel.isSettingOn)
			currStateREF->Update();

	}

	void GameManager::Pause()
	{
		GamePaused = true;
		global::isVolumeReduced = true;
	}

	void GameManager::Resume()
	{
		GamePaused = false;
		global::isVolumeReduced = false;
	}

	void GameManager::Draw()
	{
		currStateREF->Draw();
		if (SettingPanel.isSettingOn)
			SettingPanel.Draw();
		cursor.Draw();
		AESysFrameEnd();
	}
	
	void GameManager::Destroy()
	{
		if (currStateREF)
		{
			currStateREF->Destroy();
			delete currStateREF;
			currStateREF = nullptr;
		}
		cursor.Destroy();
		SettingPanel.Destroy();
		SettingUI::StaticDestory();
	}

	void GameManager::SetNextGameState(EGameState state)
	{
		nextState = state;
	}
}