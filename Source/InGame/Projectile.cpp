#include "Projectile.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "../Manager/Playing.h"
#include "../Manager/GameManager.h"

void InGame::Projectile::Init()
{
	Mesh = Utils::CreateMesh();
	Texture = AEGfxTextureLoad("Assets/Bullet.png");
}

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos, PlayerCharacter* object)
{
	size = object->Stats.ProjectileSize;
	direction = Dir;
	position = Pos;
	CollisionRadius = object->Stats.ProjectileCollisionSize;
	bIsPandingKill = false;
	isExplosive = false;
	AEVec2Set(&ExplosionSize, 300.f, 300.f);
	AEVec2Set(&ExplosionOffset, 0.f, 0.f);
	
	BulletSpeed = object->Stats.ProjectileSpeed;
	HitCount = object->Stats.effectiveHitCount ;
	Damage = object->Stats.effectiveDamage;

}

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos, EnemyCharacter* object)
{
	size = object->Stats.ProjectileSize;
	direction = Dir;
	position = Pos;
	CollisionRadius = object->Stats.ProjectileCollisionSize;
	bIsPandingKill = false;
	isExplosive = false;

	BulletSpeed = object->Stats.ProjectileSpeed;
	Damage = object->Stats.Damage;
}

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos, f32 BulletSpeed, f32 Damage, s32 HitCount, bool isExplosive)
{
	size.x = 10;
	size.y = 10;
	direction = Dir;
	position = Pos;
	CollisionRadius = 5;
	bIsPandingKill = false;
	this->isExplosive = isExplosive;

	this->HitCount = HitCount;
	this->BulletSpeed = BulletSpeed;
	this->Damage = Damage;

	isExplosionStarted = false;
	ExplosionAnimationCount = 0;
	TimeAcc = 0.f;
	AEVec2Set(&ExplosionPos, 0.f, 0.f);
	AEVec2Set(&ExplosionSize, 300.f, 300.f);
}

void InGame::Projectile::Update()
{
	position.x += direction.x * BulletSpeed;
	position.y += direction.y * BulletSpeed;

	if (isExplosionStarted)
	{
		TimeAcc += global::DeltaTime;

		if (TimeAcc > 0.1f)
		{
			TimeAcc = 0;
			ExplosionAnimationCount++;
		}
	}
}

void InGame::Projectile::Draw()
{
	if (!isExplosionStarted)
	{
		Utils::DrawObjectWithDirection(*this);
	}

	if (isExplosionStarted)
	{
		if (ExplosionAnimationCount < ExplosionMaxAnimationCount)
		{
			ExplosionOffset.x = 1.f / ExplosionMaxAnimationCount * static_cast<f32>(ExplosionAnimationCount);

			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS && GS->ITRM)
				{
					Utils::DrawObject(ExplosionPos, ExplosionOffset, ExplosionSize, GS->ITRM->explosionTexture, GS->ITRM->explosionMesh, 1.f);
				}
			}
		}
		else
		{
			isExplosionStarted = false; 
			bIsPandingKill = true;  
		}
	}
}

void InGame::Projectile::Destroy()
{
	if (Mesh)
	{
		Utils::DestroyMesh(Mesh);
		Mesh = nullptr;
	}
	AEGfxTextureUnload(Texture);
	Texture = nullptr;
}

void InGame::Projectile::IsOutOfWorld()
{
	if (position.x > global::worldMax.x ||
		position.y > global::worldMax.y ||
		position.x < global::worldMin.x ||
		position.y < global::worldMin.y)
	{
		bIsPandingKill = true;
	}
}

void InGame::Projectile::OnHit()
{
	HitCount--;
	if (HitCount < 1)
	{
		bIsPandingKill = true;
	}
}

void InGame::Projectile::OnHit(EnemyCharacter* target)
{
	if (isExplosionStarted)
	{
		return;
	}

	HitCount--;
	if (isExplosive)
	{
		Explode(target);
		BulletSpeed = 0.f;
		CollisionRadius = 0.f; // 충돌 반경을 0으로 만들어 추가 충돌을 막습니다.
	}
	else if (HitCount < 1)
	{
		bIsPandingKill = true;
	}
}

void InGame::Projectile::Explode(class EnemyCharacter* target)
{
	isExplosionStarted = true;

	AEVec2Set(&ExplosionPos, target->position.x, target->position.y);

	if (Manager::gm.currStateREF)
	{
		Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
		if (GS)
		{
			if (GS->ECs.size() > 0)
			{
				for (size_t i = 0; i < GS->ECs.size(); i++)
				{
					if (Utils::CheckCollision(*GS->ECs[i], ExplosionPos, ExplosionSize.x / 2))
					{
						GS->ECs[i]->adjustHealth(-Damage);
					}
				}
			}
		}
	}
}