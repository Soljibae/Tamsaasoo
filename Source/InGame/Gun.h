#pragma once
#include "Actor.h"
#include "Projectile.h"
#include <string>
namespace InGame
{
	enum GunType
	{
		PISTOL,
		RIFLE,
		SHOTGUN,
	};
	class Gun : public Actor
	{
	public:
		void Init(class PlayerCharacter* object);
		void Update(AEVec2 Dir, AEVec2 Pos);
		virtual void Draw(f32 alpha = 1.f);
		virtual void Destroy() override;
		virtual void FireProjectile(AEVec2 Dir, AEVec2 Pos);
		f32 FireTimer = 0.f;
		f32 RoundPerSec = 0.f;
		GunType gunType;
		AEVec2 ArmOffset;
		class PlayerCharacter* Source;
	};

	struct GunStruct
	{
	public:
		GunType Type;
		f32 ProjectileSpeed;
		f32 ProjectileCollisionSize;
		s32 ProjectileHitCount;
		f32 GuntypeDamageRatio;
		f32 GuntypeFireRateRatio;
		std::string TextureAddress;
	};

	struct PistolStruct : public GunStruct
	{
	public:
		PistolStruct();
	};
	struct RifleStruct : public GunStruct
	{
	public:
		RifleStruct();
	};
	struct ShotGunStruct : public GunStruct
	{
	public:
		ShotGunStruct();
	};
}
