#pragma once
#include "../Utils/Utils.h"
#include "../InGame/Actor.h"
#include <iostream>
#include "GameState.h"

namespace Manager
{
	class Intro : public InGame::Actor, public AGameState
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		Actor Splash;
		static f32 alpha;
	};
}