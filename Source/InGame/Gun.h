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
		BENELLIM4 = 5,
		SAIGA12 = 6,
		AA12 = 7,
		M1911 = 8,
		MOSINNAGAT = 9,
		M24 = 10,
		RAILGUN = 11,
		DESERTEGLE = 12,
		BEOWOLF = 13,
		NITRO700 = 14,
		FNFAL = 15,
		M82BARRETT = 16,
		CZ75 = 17,
		AR15 = 18,
		M110 = 19,
		MP5 = 20,
		MPX = 21,
		VECTOR = 22,
		P90 = 23,
		BREN = 24,
		MICROGUN = 25,
		M249 = 26,
		M2 = 27,
		LAST = M2,
	};

	static void GetNextType(GunType InGunType, GunType& OutGunType1, GunType& OutGunType2, GunType OutGunType3)
	{
		switch (InGunType)
		{
		NOGUN:												//Init
			OutGunType1 = M1911;
			OutGunType2 = SAWEDOFFSHOTGUN;
			OutGunType3 = CZ75;
			break;
		M1911:												//Tier1
			OutGunType1 = MOSINNAGAT;
			OutGunType2 = DESERTEGLE;
			OutGunType3 = FNFAL;
			break;
		SAWEDOFFSHOTGUN:
			OutGunType1 = DOUBLEBARREL;
			OutGunType2 = M1897;
			OutGunType3 = SAIGA12;
			break;
		CZ75:
			OutGunType1 = AR15;
			OutGunType2 = MP5;
			OutGunType3 = BREN;
			break;
		DOUBLEBARREL:										//Tier2
			OutGunType1 = KS23;
			OutGunType2 = NOGUN;
			OutGunType3 = NOGUN;
			break;
		M1897:
			OutGunType1 = BENELLIM4;
			OutGunType2 = NOGUN;
			OutGunType3 = NOGUN;
			break;
		SAIGA12:
			OutGunType1 = AA12;
			OutGunType2 = NOGUN;
			OutGunType3 = NOGUN;
			break;
		MOSINNAGAT:											
			OutGunType1 = M24;
			OutGunType2 = RAILGUN;
			OutGunType3 = NOGUN;
			break;
		DESERTEGLE:
			OutGunType1 = BEOWOLF;
			OutGunType2 = NITRO700;
			OutGunType3 = NOGUN;
			break;
		FNFAL:
			OutGunType1 = M82BARRETT;
			OutGunType2 = NOGUN;
			OutGunType3 = NOGUN;
			break;
		AR15:
			OutGunType1 = M110;
			OutGunType2 = NOGUN;
			OutGunType3 = NOGUN;
			break;
		MP5:
			OutGunType1 = MPX;
			OutGunType2 = VECTOR;
			OutGunType3 = P90;
			break;
		BREN:
			OutGunType1 = MICROGUN;
			OutGunType2 = M249;
			OutGunType3 = M2;
			break;
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
	struct SAWEDOFFSHOTGUNStruct : public GunStruct
	{
	public:
		SAWEDOFFSHOTGUNStruct();
	};
	struct DOUBLEBARRELStruct : public GunStruct
	{
	public:
		DOUBLEBARRELStruct();
	};
	struct KS23Struct : public GunStruct
	{
	public:
		KS23Struct();
	};
	struct M1897Struct : public GunStruct
	{
	public:
		M1897Struct();
	};
	struct BENELLIM4Struct : public GunStruct
	{
	public:
		BENELLIM4Struct();
	};
	struct SAIGA12Struct : public GunStruct
	{
	public:
		SAIGA12Struct();
	};
	struct AA12Struct : public GunStruct
	{
	public:
		AA12Struct();
	};
	struct M1911Struct : public GunStruct
	{
	public:
		M1911Struct();
	};
	struct MOSINNAGATStruct : public GunStruct
	{
	public:
		MOSINNAGATStruct();
	};
	struct M24Struct : public GunStruct
	{
	public:
		M24Struct();
	};
	struct RAILGUNStruct : public GunStruct
	{
	public:
		RAILGUNStruct();
	};
	struct DESERTEGLEStruct : public GunStruct
	{
	public:
		DESERTEGLEStruct();
	};
	struct BEOWOLFStruct : public GunStruct
	{
	public:
		BEOWOLFStruct();
	};
	struct NITRO700Struct : public GunStruct
	{
	public:
		NITRO700Struct();
	};
	struct FNFALStruct : public GunStruct
	{
	public:
		FNFALStruct();
	};
	struct M82BARRETTStruct : public GunStruct
	{
	public:
		M82BARRETTStruct();
	};
	struct CZ75Struct : public GunStruct
	{
	public:
		CZ75Struct();
	};
	struct AR15Struct : public GunStruct
	{
	public:
		AR15Struct();
	};
	struct M110Struct : public GunStruct
	{
	public:
		M110Struct();
	};
	struct MP5Struct : public GunStruct
	{
	public:
		MP5Struct();
	};
	struct MPXStruct : public GunStruct
	{
	public:
		MPXStruct();
	};
	struct VECTORStruct : public GunStruct
	{
	public:
		VECTORStruct();
	};
	struct P90Struct : public GunStruct
	{
	public:
		P90Struct();
	};
	struct BRENStruct : public GunStruct
	{
	public:
		BRENStruct();
	};
	struct MICROGUNStruct : public GunStruct
	{
	public:
		MICROGUNStruct();
	};
	struct M249Struct : public GunStruct
	{
	public:
		M249Struct();
	};
	struct M2Struct : public GunStruct
	{
	public:
		M2Struct();
	};
}
