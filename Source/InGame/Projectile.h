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
		static void StaticInit();
		void Spawn(AEVec2 Dir, AEVec2 Pos, class PlayerCharacter* object);
		void Spawn(AEVec2 Dir, AEVec2 Pos, class EnemyCharacter* object);
		void Spawn(AEVec2 Dir, AEVec2 Pos, f32 BulletSpeed, f32 Damage, s32 HitCount, bool isExplosive);
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		static void StaticDestroy();
		void IsOutOfWorld();
		void OnHit();
		void OnHit(class EnemyCharacter* target);
		void Explode(class EnemyCharacter* target);

		AEVec2 LastLoc;
		f32 Damage;
		s32 HitCount;
		f32 BulletSpeed;

		bool isExplosive;

		AEVec2 ExplosionSize;
		AEVec2 ExplosionPos;
		AEVec2 ExplosionOffset;
		f32 TimeAcc = 0;
		
		bool isExplosionStarted = false;
		s32 ExplosionAnimationCount = 0;
		s32 ExplosionMaxAnimationCount = 9;
		bool hasHit = false;
		static AEGfxTexture* PlayerTexture;
		static AEGfxTexture* EnemyMobTexture;
		static AEGfxTexture* Boss1Texture;
		static AEGfxTexture* Boss2Texture;
		static AEGfxTexture* Boss3Texture;
	};
}
