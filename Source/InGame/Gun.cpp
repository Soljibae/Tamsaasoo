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
		Texture = AEGfxTextureLoad("Assets/Pistol.png");
		Source = object;
		size.x = 40;
		size.y = 40;
		ChamberTime = Source->Stats.FireRate;
		direction.x = 1;
		direction.y = 0;
	}


	void Gun::Update(AEVec2 Dir, AEVec2 Pos)
	{
		ChamberTime = Source->Stats.FireRate;
		AEVec2 Vec;
		position = Pos;
		AEVec2Scale(&Vec, &Dir, 40.f);
		AEVec2Add(&position, &position, &Vec);
		direction = Dir;
		
		FireTimer += global::DeltaTime;
		if (FireTimer > 1.f / ChamberTime && AEInputCheckCurr(AEVK_LBUTTON))
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
}

