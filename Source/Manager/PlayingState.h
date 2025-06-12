#pragma once
#include "GameState.h"

namespace Manager
{
	class Playing : public AGameState
	{
	public:
		void Init();
		void Update();
		void Draw();
		void Destroy();
	};
}