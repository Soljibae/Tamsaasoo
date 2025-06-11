#pragma once
#include "Actor.h"
namespace InGame
{
	class Character : public Actor
	{
	public:
		virtual void FireProjectile();
		s32 Health = 1;
	};
}
