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
		void Draw();
		void Destroy();
		void SetNextGameState(EGameState state);

		AGameState* currStateREF;
		EGameState currState;
		EGameState nextState;
	};
	extern GameManager gm;
}