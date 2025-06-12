#pragma once
#include "AEEngine.h"
#include <iostream>
#include "Intro.h"
#include "GameState.h"
#include "../Global/GlobalVariables.h"
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
		AGameState* currStateREF;
		EGameState currState;
		EGameState nextState;
	};
	extern GameManager gm;
}