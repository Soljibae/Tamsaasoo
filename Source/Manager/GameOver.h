#pragma once
#include "GameState.h"
#include "../InGame/Actor.h"
#include "Button.h"
namespace Manager
{
	class GameOver
	{
	public:
		void Init();
		void Update();
		void Draw();
		void Destroy();
		bool isGameOver = false;
	private:
		InGame::Actor BG;
		f32 BGalpha;
		struct Buttons
		{
			Button Retry;
			Button MainMenu;
		};
		Buttons buttons;
		f32 fadeoutTime;
		f32 fadeoutDuration;
		f32 buttonShowDelay;
		s8 pFont;
	};
	extern GameOver gameOverScreen;
}