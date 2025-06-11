#pragma once
#include "AEEngine.h"
#include <iostream>
#include "../Global/GlobalVariables.h"

namespace manager
{
	enum GameState
	{
		temp,
	};
	class GameManager
	{
	public:
		void init();
		void update();
		void draw();
		GameState gs;
	};
	extern GameManager gm;
}