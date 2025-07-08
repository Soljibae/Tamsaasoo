#pragma once
#include "AEEngine.h"
#include <iostream>
#include "../Global/GlobalVariables.h"
#include "GameState.h"
#include "../InGame/Actor.h"
namespace Manager
{
	class GameManager
	{
	public:
		void Init();
		void Update();
		void Pause();
		void Resume();
		void Draw();
		void Destroy();
		void SetNextGameState(EGameState state);
		
		AGameState* currStateREF;
		EGameState currState;
		EGameState nextState;
		bool GamePaused{ false };
		bool forceRestart{ false };
		bool shouldExit{ false };
	};
	extern GameManager gm;
}