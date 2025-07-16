#pragma once
#include "../InGame/Actor.h"
#include "../InGame/PlayerCharacter.h"

namespace Manager
{
	class ExpUI
	{
	public:
		void Init(InGame::PlayerCharacter* InPC);
		void Update();
		void Draw();
		void Destroy();

		s8 pFont;
		f32 fontSize = 72.f;
		f32 textDrawSize = 0.2f;

		InGame::Actor xpBarBackground;
		InGame::Actor xpBarEmpty;
		InGame::Actor xpBarFill;

	private:
		InGame::PlayerCharacter* PC = nullptr;
	};
	extern ExpUI ExpPanel;
}