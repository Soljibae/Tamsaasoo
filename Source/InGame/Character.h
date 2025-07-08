#pragma once
#include "Actor.h"
#include "Stat.h"
namespace InGame
{
	class Character : public Actor
	{
	public:
		Stat* Stats;
		virtual ~Character() {};
		virtual void adjustHealth(f32 Amount);
	};
}
