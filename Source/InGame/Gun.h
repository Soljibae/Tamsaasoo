#pragma once
#include "Actor.h"
#include "Projectile.h"
namespace InGame
{
	class Gun : public Actor
	{
	public:
		void Init(Actor* object);
		void Update(AEVec2 Dir, AEVec2 Pos, u32 Level);
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void FireProjectile(AEVec2 Dir, AEVec2 Pos, u32 Level);
		f32 FireTimer = 0.f;
		f32 ChamberTime;
		Actor* Source;
	};
}
