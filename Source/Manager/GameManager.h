#pragma once
#include "AEEngine.h"
#include <iostream>
#include "../Global/GlobalVariables.h"
#include "GameState.h"
#include "../InGame/Actor.h"
#include "../InGame/Game.h"
namespace Manager
{
	enum class GameState
	{
		Intro,
		MainMenu,
		Game,
	};

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