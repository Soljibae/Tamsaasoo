#pragma once
#include "../InGame/Actor.h"

namespace Manager
{
	class UI : public InGame::Actor
	{
	public:
		virtual void Init();
		virtual void Update();
		virtual void Draw();
		virtual void Destroy();
		
	};
}