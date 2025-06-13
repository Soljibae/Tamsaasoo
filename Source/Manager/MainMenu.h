#pragma once
#include "GameState.h"
#include "../Global/GlobalVariables.h"
#include "../InGame/Actor.h"
#include "../Utils/Utils.h"

namespace Manager
{
	class MainMenu : public AGameState
	{
	public:
		void Init() override;
		void Update() override;
		void Draw() override;
		void Destroy() override;
	private:
		InGame::Actor Illust;
		InGame::Actor Black;
		static f32 alpha;

		struct Button
		{
			f32 x, y, w, h;
		};
	};
}