#pragma once
#include "AEEngine.h"
#include <iostream>
#include "../Global/GlobalVariables.h"
#include "Intro.h"
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
		GameState gs;
		Intro intro;
		InGame::Game Game;
	};
	extern GameManager gm;
}