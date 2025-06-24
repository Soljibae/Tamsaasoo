#pragma once
#include "Actor.h"
namespace InGame
{
	class Character : public Actor
	{
	public:
		virtual void adjustHealth(f32 Amount);
	};
}
