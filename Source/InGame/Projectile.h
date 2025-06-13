#pragma once
#include "Actor.h"
namespace InGame
{
	enum ProjectileType
	{
		
	};

	class Projectile : public Actor
	{
	public:
		void Init(AEVec2 Dir, AEVec2 Pos);
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		void IsOutOfWorld();
		void OnHit();
		s32 Damage = 1;
		s32 HitCount = 1;
	};
}
