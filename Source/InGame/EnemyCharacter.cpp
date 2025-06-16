#include "EnemyCharacter.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "../Manager/GameManager.h"
#include "../Manager/Playing.h"
namespace InGame
{
	void InGame::EnemyCharacter::Init()
	{
		Mesh = Utils::CreateMesh();
	}

	void EnemyCharacter::Spawn(AEVec2 Pos, EnemyData* InData)
	{
		if (InData->Texture == nullptr)
		{
			InData->Init();
		}
		Type = InData->Type;
		Texture = InData->Texture;
		Damage = InData->Damage;
		Exp = InData->Exp;
		MovementSpeed = InData->MovementSpeed;
		size = InData->DrawSize;
		CollisionRadius = InData->CollisionRadius;
		position = Pos;
	}

	void InGame::EnemyCharacter::Update()
	{
		f32 len = AEVec2Distance(&global::PlayerLocation, &position);
		f32 dx = position.x - global::PlayerLocation.x;
		f32 dy = position.y - global::PlayerLocation.y;
		direction.x = dx / len;
		if ((direction.x > 0 && size.x > 0) || (direction.x < 0 && size.x < 0))
		{
			size.x *= -1;
		}
		direction.y = dy / len;
		switch (Type)
		{
		case EnemyType::MINION:
			position.x -= direction.x * MovementSpeed * global::DeltaTime;
			position.y -= direction.y * MovementSpeed * global::DeltaTime;
			break;
		case EnemyType::ARCHER:
			ProjectileSpawnTimer += global::DeltaTime;
			if (len > 500)
			{
				position.x -= direction.x * MovementSpeed * global::DeltaTime;
				position.y -= direction.y * MovementSpeed * global::DeltaTime;
			}
			else
			{
				if (ProjectileSpawnTimer > ProjectileChamberTimer)
				{
					ProjectileSpawnTimer = 0;
					SpawnProjectile(direction,position);
				}
			}
			break;
		}
		
	}
	void EnemyCharacter::Draw()
	{
		Utils::DrawObject(*this);
	}
	void EnemyCharacter::Destroy()
	{
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
		}
	}
	void EnemyCharacter::SpawnProjectile(AEVec2 Dir, AEVec2 Pos)
	{
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (GS->EPPool.size() > 0)
				{
					Projectile* EP = GS->EPPool.back();
					GS->EPPool.pop_back();
					EP->Spawn(Dir, Pos);
					GS->EPs.push_back(EP);
				}

			}
		}
	}
}