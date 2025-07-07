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
		gunType = object->GunData->Type;
		Mesh = Utils::CreateMesh();
		Texture = AEGfxTextureLoad(object->GunData->TextureAddress.c_str());
		Source = object;
		AEVec2Set(&size, 80.f, 40.f);
		RoundPerSec = Source->Stats.FireRate;
		direction.x = 1;
		direction.y = 0;
		AEVec2Set(&ArmOffset, 5.f, 18.f);
	}


	void Gun::Update(AEVec2 Dir, AEVec2 Pos)
	{
		RoundPerSec = Source->Stats.effectiveFireRate;
		AEVec2 Vec;
		position = Pos;
		AEVec2Scale(&Vec, &Dir, 40.f);
		AEVec2Add(&position, &position, &Vec);
		AEVec2Add(&position, &position, &ArmOffset);
		direction = Dir;
		
		FireTimer += global::DeltaTime;
		if (FireTimer > 1.f / RoundPerSec && AEInputCheckCurr(AEVK_LBUTTON))
		{
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
				if (gunType == SHOTGUN)
				{
					const int numPellets = 5;
					const float spreadAngleDeg = 30.0f;
					const float spreadStep = spreadAngleDeg / (numPellets - 1);
					const float startAngle = -spreadAngleDeg / 2.0f;

					AEVec2Normalize(&Dir, &Dir);
					float baseAngle = atan2f(Dir.y, Dir.x);

					for (int i = 0; i < numPellets; ++i)
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
	
	PistolStruct::PistolStruct()
	{
		Type = GunType::PISTOL;
		GuntypeDamageRatio = 1.f;
		GuntypeFireRateRatio = 1.f;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 1;
		TextureAddress = "Assets/Pistol.png";
	}

	RifleStruct::RifleStruct()
	{
		Type = GunType::RIFLE;
		GuntypeDamageRatio = 3.f;
		GuntypeFireRateRatio = 2.5f;
		ProjectileSpeed = 45.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 3;
		TextureAddress = "Assets/Rifle.png";
	}

	ShotGunStruct::ShotGunStruct()
	{
		Type = GunType::SHOTGUN;
		GuntypeDamageRatio = 2.f;
		GuntypeFireRateRatio = 0.5f;
		ProjectileSpeed = 15.f;
		ProjectileCollisionSize = 10.f;
		ProjectileHitCount = 1;
		TextureAddress = "Assets/ShotGun.png";
	}

}

