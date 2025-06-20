#pragma once
#include "Actor.h"
#include "Projectile.h"
namespace InGame
{
	class Gun : public Actor
	{
	public:
		void Init(class PlayerCharacter* object);
		void Update(AEVec2 Dir, AEVec2 Pos);
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual void FireProjectile(AEVec2 Dir, AEVec2 Pos);
		f32 FireTimer = 0.f;
		f32 ChamberTime;
		class PlayerCharacter* Source;
	};
}
