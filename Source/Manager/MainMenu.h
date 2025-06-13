#pragma once
#include "GameState.h"
#include "../Global/GlobalVariables.h"
#include "../InGame/Actor.h"
#include "../Utils/Utils.h"

namespace Manager
{
	static struct Button : public InGame::Actor
	{
		bool press, release;
	};
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
		f32 fadeInTime{ 0 };
		const f32 fadeInDuration{ 1.f };
		static f32 alpha;
		Button button;
	};
}