#pragma once
#include "AEEngine.h"
#include "GameState.h"
#include "../InGame/Actor.h"

namespace Manager
{
	class EndingCredits : public AGameState
	{
	public:
		void Init() override;
		void Update() override;
		void Draw() override;
		void Destroy() override;

	protected:
		InGame::Actor* EndingImage = nullptr;
		InGame::Actor* PressAnyKeyImage = nullptr;
		float Timer = 0.f;
	};
}