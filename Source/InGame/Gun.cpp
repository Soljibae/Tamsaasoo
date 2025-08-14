#include "Gun.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include <iostream>
#include "../Manager/GameManager.h"
#include "Projectile.h"
#include "../Manager/Playing.h"
#include "PlayerCharacter.h"

namespace InGame
{
	void Gun::Init(PlayerCharacter* object)
	{
		Manager::SFXManager.AddNewSFX(SFX, "Assets/SFX/Shot/pistol.wav", "pistol");
		Manager::SFXManager.AddNewSFX(SFX, "Assets/SFX/Shot/rifle.wav", "rifle");
		Manager::SFXManager.AddNewSFX(SFX, "Assets/SFX/Shot/shotgun.wav", "shotgun");

		gunType = object->GunData->Type;
		ProjectileSpawnCount = object->GunData->ProjectileSpawnCount;
		Mesh = Utils::CreateMesh();
		Texture = AEGfxTextureLoad(object->GunData->TextureAddress.c_str());
		Source = object;
		AEVec2Set(&size, 80.f, 40.f);
		RoundPerSec = Source->Stats->FireRate;
		direction.x = 1;
		direction.y = 0;
		AEVec2Set(&ArmOffset, 5.f, 18.f);
	}


	void Gun::Update(AEVec2 Dir, AEVec2 Pos)
	{
		RoundPerSec = dynamic_cast<PlayerStat*>(Source->Stats)->effectiveFireRate;
		AEVec2 Vec;
		position = Pos;
		AEVec2Scale(&Vec, &Dir, 40.f);
		AEVec2Add(&position, &position, &Vec);
		AEVec2Add(&position, &position, &ArmOffset);
		direction = Dir;
		
		FireTimer += global::DeltaTime;
		if (FireTimer > 1.f / RoundPerSec && AEInputCheckCurr(AEVK_LBUTTON))
		{
			if(gunType == M1911 || gunType == CZ75 || gunType == DESERTEGLE || gunType == MP5 || gunType == MPX || gunType == VECTOR || gunType == P90)
				Manager::SFXManager.Play("pistol");
			else if(gunType == BEOWOLF || gunType == MOSINNAGAT || gunType == M24 || gunType == RAILGUN || gunType == NITRO700 || gunType == FNFAL || gunType == M82BARRETT || gunType == AR15 || gunType == M110 || gunType == BREN || gunType == MICROGUN || gunType == M249 || gunType == M2)
				Manager::SFXManager.Play("rifle");
			else if(gunType == SAWEDOFFSHOTGUN || gunType == DOUBLEBARREL || gunType == KS23 || gunType == M1897 || gunType == BENELLIM4)
				Manager::SFXManager.Play("shotgun");
			else if(gunType == SAIGA12 || gunType == AA12)
				Manager::SFXManager.Play("shotgun");

			FireProjectile(Dir, position);
			FireTimer = 0.f;
		}
	}

	void Gun::Draw(f32 alpha)
	{
		Utils::DrawObjectWithDirection(*this, alpha);
	}

	void Gun::Destroy()
	{
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
		}
		if (Texture)
		{
			AEGfxTextureUnload(Texture);
		}
	}

	void Gun::FireProjectile(AEVec2 Dir, AEVec2 Pos)
	{
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (gunType == SAWEDOFFSHOTGUN || gunType == DOUBLEBARREL || gunType == KS23 || gunType == M1897 || gunType == BENELLIM4 || gunType == SAIGA12 || gunType == AA12)
				{
					float spreadAngleDeg = 30.0f;
					float spreadStep = spreadAngleDeg / (ProjectileSpawnCount - 1);
					float startAngle = -spreadAngleDeg / 2.0f;

					AEVec2Normalize(&Dir, &Dir);
					float baseAngle = atan2f(Dir.y, Dir.x);

					for (int i = 0; i < ProjectileSpawnCount; ++i)
					{
						if (GS->PPPool.empty())
						{
							break;
						}

						float angleOffsetDeg = startAngle + i * spreadStep;
						float angleOffsetRad = angleOffsetDeg * (3.1415926f / 180.0f);

						float newAngle = baseAngle + angleOffsetRad;
						AEVec2 pelletDir;
						AEVec2Set(&pelletDir, cosf(newAngle), sinf(newAngle));

						Projectile* PP = GS->PPPool.back();
						GS->PPPool.pop_back();
						PP->Spawn(pelletDir, Pos, Source);
						GS->PPs.push_back(PP);
					}
				}
				else
				{
					if (GS->PPPool.size() > 0)
					{
						Projectile* PP = GS->PPPool.back();
						GS->PPPool.pop_back();
						PP->Spawn(Dir, Pos, Source);
						GS->PPs.push_back(PP);
					}
				}
			}
		}
		//std::cout << "ProjectileFired" << std::endl;
	}
	
	NOGUNStruct::NOGUNStruct()
	{
		Type = GunType::NOGUN;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 1.f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 1;
		TextureAddress = "Assets/Arm.png";
	}
	SAWEDOFFSHOTGUNStruct::SAWEDOFFSHOTGUNStruct()
	{
		Type = GunType::SAWEDOFFSHOTGUN;
		GuntypeDamageRatio = 1.1f;
		GuntypeFireRateRatio = 0.7f;
		ProjectileSpeed = 10.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 3;
		TextureAddress = "Assets/Guns/Character_SAWEDOFFSHOTGUN.png";
	}
	DOUBLEBARRELStruct::DOUBLEBARRELStruct()
	{
		Type = GunType::DOUBLEBARREL;
		GuntypeDamageRatio = 2.f;
		GuntypeFireRateRatio = 1.0f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 5;
		TextureAddress = "Assets/Guns/Character_DOUBLEBARREL.png";
	}
	KS23Struct::KS23Struct()
	{
		Type = GunType::KS23;
		GuntypeDamageRatio = 5.f;
		GuntypeFireRateRatio = 0.2f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 40.f;
		ProjectileHitCount = 3;
		ProjectileSpawnCount = 5;
		TextureAddress = "Assets/Guns/Character_KS23.png";
	}
	M1897Struct::M1897Struct()
	{
		Type = GunType::M1897;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 2.0f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 5;
		TextureAddress = "Assets/Guns/Character_M1897.png";
	}
	BENELLIM4Struct::BENELLIM4Struct()
	{
		Type = GunType::BENELLIM4;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 3.f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 5;
		TextureAddress = "Assets/Guns/Character_BENELLIM4.png";
	}
	SAIGA12Struct::SAIGA12Struct()
	{
		Type = GunType::SAIGA12;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 2.0f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 5;
		TextureAddress = "Assets/Guns/Character_SAIGA12.png";
	}
	AA12Struct::AA12Struct()
	{
		Type = GunType::AA12;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 3.5f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 5;
		TextureAddress = "Assets/Guns/Character_AA12.png";
	}
	M1911Struct::M1911Struct()
	{
		Type = GunType::M1911;
		GuntypeDamageRatio = 1.2f;
		GuntypeFireRateRatio = 1.3f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 2;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_M1911.png";
	}
	MOSINNAGATStruct::MOSINNAGATStruct()
	{
		Type = GunType::MOSINNAGAT;
		GuntypeDamageRatio = 3.f;
		GuntypeFireRateRatio = 1.0f;
		ProjectileSpeed = 30.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 5;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_MOSINNAGAT.png";
	}
	M24Struct::M24Struct()
	{
		Type = GunType::M24;
		GuntypeDamageRatio = 5.f;
		GuntypeFireRateRatio = 2.f;
		ProjectileSpeed = 30.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 10;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_M24.png";
	}
	RAILGUNStruct::RAILGUNStruct()
	{
		Type = GunType::RAILGUN;
		GuntypeDamageRatio = 10.f;
		GuntypeFireRateRatio = 1.f;
		ProjectileSpeed = 40.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 20;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_RAILGUN.png";
	}
	DESERTEGLEStruct::DESERTEGLEStruct()
	{
		Type = GunType::DESERTEGLE;
		GuntypeDamageRatio = 3.f;
		GuntypeFireRateRatio = 1.f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 3;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_DESERTEGLE.png";
	}
	BEOWOLFStruct::BEOWOLFStruct()
	{
		Type = GunType::BEOWOLF;
		GuntypeDamageRatio = 5.f;
		GuntypeFireRateRatio = 2.f;
		ProjectileSpeed = 30.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 5;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_BEOWOLF.png";
	}
	NITRO700Struct::NITRO700Struct()
	{
		Type = GunType::NITRO700;
		GuntypeDamageRatio = 20.f;
		GuntypeFireRateRatio = 0.5f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 50.f;
		ProjectileHitCount = 40;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_NITRO700.png";
	}
	FNFALStruct::FNFALStruct()
	{
		Type = GunType::FNFAL;
		GuntypeDamageRatio = 3.f;
		GuntypeFireRateRatio = 1.0f;
		ProjectileSpeed = 30.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_FNFAL.png";
	}
	M82BARRETTStruct::M82BARRETTStruct()
	{
		Type = GunType::M82BARRETT;
		GuntypeDamageRatio = 10.f;
		GuntypeFireRateRatio = 0.5f;
		ProjectileSpeed = 40.f;
		ProjectileCollisionSize = 30.f;
		ProjectileHitCount = 10;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_M82BARRETT.png";
	}
	CZ75Struct::CZ75Struct()
	{
		Type = GunType::CZ75;
		GuntypeDamageRatio = 0.75f;
		GuntypeFireRateRatio = 2.5f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_CZ75.png";
	}
	AR15Struct::AR15Struct()
	{
		Type = GunType::AR15;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 2.4f;
		ProjectileSpeed = 50.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 2;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_AR15.png";
	}
	M110Struct::M110Struct()
	{
		Type = GunType::M110;
		GuntypeDamageRatio = 1.6f;
		GuntypeFireRateRatio = 1.6f;
		ProjectileSpeed = 50.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 5;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_M110.png";
	}
	MP5Struct::MP5Struct()
	{
		Type = GunType::MP5;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 3.0f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_MP5.png";
	}
	MPXStruct::MPXStruct()
	{
		Type = GunType::MPX;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 3.0f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 2;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_MPX.png";
	}
	VECTORStruct::VECTORStruct()
	{
		Type = GunType::VECTOR;
		GuntypeDamageRatio = 0.5f;
		GuntypeFireRateRatio = 10.f;
		ProjectileSpeed = 10.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_VECTOR.png";
	}
	P90Struct::P90Struct()
	{
		Type = GunType::P90;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 5.f;
		ProjectileSpeed = 25.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 3;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_P90.png";
	}
	BRENStruct::BRENStruct()
	{
		Type = GunType::BREN;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 2.f;
		ProjectileSpeed = 30.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 3;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_BREN.png";
	}
	MICROGUNStruct::MICROGUNStruct()
	{
		Type = GunType::MICROGUN;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 10.f;
		ProjectileSpeed = 30.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 1;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_MICROGUN.png";
	}
	M249Struct::M249Struct()
	{
		Type = GunType::M249;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 3.f;
		ProjectileSpeed = 30.f;
		ProjectileCollisionSize = 20.f;
		ProjectileHitCount = 2;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_M249.png";
	}
	M2Struct::M2Struct()
	{
		Type = GunType::M2;
		GuntypeDamageRatio = 10.f;
		GuntypeFireRateRatio = 1.f;
		ProjectileSpeed = 40.f;
		ProjectileCollisionSize = 30.f;
		ProjectileHitCount = 5;
		ProjectileSpawnCount = 1;
		TextureAddress = "Assets/Guns/Character_M2.png";
	}
	
}

