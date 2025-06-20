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
		void Init();
		void Spawn(AEVec2 Dir, AEVec2 Pos, class PlayerCharacter* object);
		void Spawn(AEVec2 Dir, AEVec2 Pos, class EnemyCharacter* object);
		void Spawn(AEVec2 Dir, AEVec2 Pos, f32 BulletSpeed, s32 Damage);
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		void IsOutOfWorld();
		void OnHit();
		s32 Damage;
		s32 HitCount;
		f32 BulletSpeed;
	};
}
