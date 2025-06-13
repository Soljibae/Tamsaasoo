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
		bool IsOutOfWorld();
		s32 Damage = 1;
		
	};
}
