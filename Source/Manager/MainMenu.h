#pragma once
#include "GameState.h"

namespace Manager
{
	class MainMenu : public AGameState
	{
	public:
		void Init() override;
		void Update() override;
		void Draw() override;
		void Destroy() override;
	};
}