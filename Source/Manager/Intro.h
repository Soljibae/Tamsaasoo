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
		void Init() override;
		void Update() override;
		void Draw() override;
		void Destroy() override;
	private:
		InGame::Actor Splash;
		static f32 alpha;
		f32 sceneTime{ 0 };
		f32 sceneDuration{ 4.5f };
		f32 halfTime{ 2.f };
	};
}