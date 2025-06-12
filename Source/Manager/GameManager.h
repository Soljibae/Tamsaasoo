#pragma once
#include "AEEngine.h"
#include <iostream>
#include "../Global/GlobalVariables.h"
#include "Intro.h"
#include "../InGame/Actor.h"

namespace Manager
{
	enum class GameState
	{
		Intro,
		MainMenu,
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
	};
	extern GameManager gm;
}