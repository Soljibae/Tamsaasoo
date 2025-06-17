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
	void Gun::Init(Actor* object)
	{
		Mesh = Utils::CreateMesh();
		Texture = AEGfxTextureLoad("Assets/TestBlankWhite.png");
		Source = object;
		PlayerCharacter* player = dynamic_cast<PlayerCharacter*>(object);

		ChamberTime = player->Stats.FireRate;
	}


	void Gun::Update(AEVec2 Dir, AEVec2 Pos, u32 Level)
	{
		FireTimer += global::DeltaTime;
		if (FireTimer > 1.f / ChamberTime)
		{
			FireProjectile(Dir, Pos, Level);
			FireTimer = 0.f;
		}
	}

	void Gun::Draw()
	{
		Utils::DrawObject(*this);
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

	void Gun::FireProjectile(AEVec2 Dir, AEVec2 Pos, u32 Level)
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

