#pragma once
#include "Actor.h"
namespace InGame
{
	class Item : public Actor
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;


	};
}
