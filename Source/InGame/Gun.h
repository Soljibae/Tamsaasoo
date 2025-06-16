#pragma once
#include "Actor.h"
#include "Projectile.h"
namespace InGame
{
	class Gun : public Actor
	{
	public:
		virtual void Init() override;
		void Update(AEVec2 Dir, AEVec2 Pos, u32 Level);
		virtual void Draw() override;
		virtual void FireProjectile(AEVec2 Dir, AEVec2 Pos, u32 Level);
		s32 Damage = 1;
		f32 FireTimer = 0.f;
		f32 ChamberTime = 0.2f;
	};
}
