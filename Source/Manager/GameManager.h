#pragma once
#include "AEEngine.h"
#include <iostream>
#include "../Global/GlobalVariables.h"
#include "Intro.h"

namespace manager
{
	enum class GameState
	{
		Intro,
		MainMenu,
	};

	class GameManager
	{
	public:
		void init();
		void update();
		void draw();
		void exit();
		GameState gs;
	};
	extern GameManager gm;
}