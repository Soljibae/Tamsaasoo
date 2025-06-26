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
		Mesh = Utils::CreateMesh();
		Texture = AEGfxTextureLoad(object->GunData->TextureAddress.c_str());
		Source = object;
		AEVec2Set(&size, 40.f, 40.f);
		RoundPerSec = Source->Stats.FireRate;
		direction.x = 1;
		direction.y = 0;
		gunType = PISTOL;
	}


	void Gun::Update(AEVec2 Dir, AEVec2 Pos)
	{
		RoundPerSec = Source->Stats.effectiveFireRate;
		AEVec2 Vec;
		position = Pos;
		AEVec2Scale(&Vec, &Dir, 40.f);
		AEVec2Add(&position, &position, &Vec);
		direction = Dir;
		
		FireTimer += global::DeltaTime;
		if (FireTimer > 1.f / RoundPerSec && AEInputCheckCurr(AEVK_LBUTTON))
		{
			FireProjectile(Dir, position);
			FireTimer = 0.f;
		}
	}

	void Gun::Draw()
	{
		Utils::DrawObjectWithDirection(*this);
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
				if (GS->PPPool.size() > 0)
				{
					Projectile* PP = GS->PPPool.back();
					GS->PPPool.pop_back();
					PP->Spawn(Dir, Pos, Source);
					GS->PPs.push_back(PP);
				}
			}
		}
		//std::cout << "ProjectileFired" << std::endl;
	}
	
	PistolStruct::PistolStruct()
	{
		Type = GunType::PISTOL;
		RoundPerSec = 2;
		ProjectileSpeed = 20.f;
		ProjectileCollisionSize = 5.f;
		ProjectileDamage = 1.f;
		ProjectileHitCount = 1;
		TextureAddress = "Assets/Pistol.png";
	}

	RifleStruct::RifleStruct()
	{
		Type = GunType::RIFLE;
		RoundPerSec = 5;
		ProjectileSpeed = 45.f;
		ProjectileCollisionSize = 5.f;
		ProjectileDamage = 3.f;
		ProjectileHitCount = 3;
		TextureAddress = "Assets/Rifle.png";
	}

	ShotGunStruct::ShotGunStruct()
	{
		Type = GunType::SHOTGUN;
		RoundPerSec = 1;
		ProjectileSpeed = 15.f;
		ProjectileCollisionSize = 10.f;
		ProjectileDamage = 2.f;
		ProjectileHitCount = 1;
		TextureAddress = "Assets/ShotGun.png";
	}

}

