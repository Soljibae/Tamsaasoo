#pragma once
#include "../InGame/Actor.h"
#include "../Manager/Button.h"
namespace Manager
{
	// Pause menu
	class PauseUI : public InGame::Actor
	{
	public:
		void Init();
		void Update();
		void Draw();
		void Destroy();
	private:
		//resume button
		Button resumeButton;
		//return to mainmenu button
		Button mainmenuButton;
		//pause screen black background image
		InGame::Actor pauseDimmer;
		//item ui
	};
	extern PauseUI pausePanel;
}