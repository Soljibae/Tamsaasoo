#pragma once
#include "Actor.h"
#include "Projectile.h"
namespace InGame
{
	class Gun : public Actor
	{
	public:
		virtual void FireProjectile();
		s32 Damage = 1;
		f32 FireTimer = 0.f;
		Projectile* ProjectilePtr = nullptr;
	};
}
