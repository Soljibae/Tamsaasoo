#pragma once
#include "Actor.h"
namespace InGame
{
	class Character : public Actor
	{
	public:

		virtual ~Character() {};
		virtual void adjustHealth(f32 Amount);
	};
}
