#pragma once
#include "../Utils/Utils.h"
#include "../InGame/Actor.h"

namespace Manager
{
	class Intro : public InGame::Actor
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		Actor Splash;
	};
}