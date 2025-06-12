#pragma once
#include "Actor.h"
namespace InGame
{
	class Background : public Actor
	{
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
	};
}
