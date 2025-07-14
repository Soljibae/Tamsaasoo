#pragma once
#include "Actor.h"
#include "Projectile.h"
#include <string>
namespace InGame
{
	enum GunType
	{
		NOGUN = 0,
		SAWEDOFFSHOTGUN = 1,
		DOUBLEBARREL = 2,
		KS23 = 3,
		M1897 = 4,
		SAIGA12 = 5,
		AA12 = 6,
		M1911 = 7,
		MOSINNAGAT = 8,
		M24 = 9,
		RAILGUN = 10,
		DESERTEGLE = 11,
		BEOWOLF = 12,
		NITRO700 = 13,
		FNFAL = 14,
		M182BARRET = 15,
		CZ75 = 16,
		AR15 = 17,
		M110 = 18,
		MP5 = 19,
		MPX = 20,
		VECTOR = 21,
		P90 = 22,
		BREN = 23,
		MICROGUN = 24,
		M249 = 25,
		M2 = 26,
		LAST = M2,
		PISTOL,
		RIFLE,
		SHOTGUN,
	};

	static void GetNextType(GunType InGunType, GunType& OutGunType1, GunType& OutGunType2, GunType OutGunType3)
	{
		switch (InGunType)
		{
		defualt:
			OutGunType1 = NOGUN;
			OutGunType2 = NOGUN;
			OutGunType3 = NOGUN;
			break;
		}
	}

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

	struct M1911Struct : public GunStruct
	{
	public:
		M1911Struct();
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
