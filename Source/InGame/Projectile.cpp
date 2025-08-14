#include "Projectile.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "../Manager/Playing.h"
#include "../Manager/GameManager.h"

AEGfxTexture* InGame::Projectile::PlayerTexture = nullptr;
AEGfxTexture* InGame::Projectile::EnemyMobTexture = nullptr;
AEGfxTexture* InGame::Projectile::Boss1Texture = nullptr;
AEGfxTexture* InGame::Projectile::Boss2Texture = nullptr;
AEGfxTexture* InGame::Projectile::Boss3Texture = nullptr;

void InGame::Projectile::Init()
{
	Mesh = Utils::CreateMesh();

}

void InGame::Projectile::StaticInit()
{
	PlayerTexture = AEGfxTextureLoad("Assets/Bullet.png");
	EnemyMobTexture = AEGfxTextureLoad("Assets/EnemyMobBullet.png");
	Boss1Texture = AEGfxTextureLoad("Assets/Boss1Bullet.png");
	Boss2Texture = AEGfxTextureLoad("Assets/Boss2Bullet.png");
	Boss3Texture = AEGfxTextureLoad("Assets/Boss3Bullet.png");
}

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos, PlayerCharacter* object)
{
	size = object->Stats->ProjectileSize;
	direction = Dir;
	position = Pos;
	CollisionRadius = object->Stats->ProjectileCollisionSize;
	bIsPandingKill = false;
	isExplosive = false;
	AEVec2Set(&ExplosionSize, global::item32ExplosionSize, global::item32ExplosionSize);
	AEVec2Set(&ExplosionOffset, 0.f, 0.f);
	hasHit = false;

	BulletSpeed = object->Stats->ProjectileSpeed;
	HitCount = object->PS->effectiveHitCount ;
	Damage = object->PS->effectiveDamage;
	Texture = PlayerTexture;

}

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos, EnemyCharacter* object)
{
	size = object->Stats->ProjectileSize;
	direction = Dir;
	position = Pos;
	CollisionRadius = object->Stats->ProjectileCollisionSize;
	bIsPandingKill = false;
	isExplosive = false;
	hasHit = false;

	BulletSpeed = object->Stats->ProjectileSpeed;
	Damage = object->Stats->Damage;
	
	if (dynamic_cast<Stage1Boss*>(object))
	{
		Texture = Boss1Texture;
	}
	else if (dynamic_cast<Stage2Boss*>(object))
	{
		Texture = Boss2Texture;
	}
	else if (dynamic_cast<Stage3Boss*>(object))
	{
		Texture = Boss3Texture;
	}
	else
	{
		Texture = EnemyMobTexture;
	}
}

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos, f32 BulletSpeed, f32 Damage, s32 HitCount, bool isExplosive)
{
	size.x = 10;
	size.y = 10;
	direction = Dir;
	position = Pos;
	LastLoc = position;
	CollisionRadius = 5;
	bIsPandingKill = false;
	this->isExplosive = isExplosive;
	hasHit = false;

	this->HitCount = HitCount;
	this->BulletSpeed = BulletSpeed;
	this->Damage = Damage;

	isExplosionStarted = false;
	ExplosionAnimationCount = 0;
	TimeAcc = 0.f;
	AEVec2Set(&ExplosionPos, 0.f, 0.f);
	AEVec2Set(&ExplosionSize, 300.f, 300.f);
	Texture = PlayerTexture;
}

void InGame::Projectile::Update()
{
	LastLoc = position;
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
	Texture = nullptr;
}

void InGame::Projectile::StaticDestroy()
{
	if (PlayerTexture)
	{
		AEGfxTextureUnload(PlayerTexture);
		PlayerTexture = nullptr;
	}
	if (EnemyMobTexture)
	{
		AEGfxTextureUnload(EnemyMobTexture);
		EnemyMobTexture = nullptr;
	}
	if (Boss1Texture)
	{
		AEGfxTextureUnload(Boss1Texture);
		Boss1Texture = nullptr;
	}
	if (Boss2Texture)
	{
		AEGfxTextureUnload(Boss2Texture);
		Boss2Texture = nullptr;
	}
	if (Boss3Texture)
	{
		AEGfxTextureUnload(Boss3Texture);
		Boss3Texture = nullptr;
	}
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
	for (Actor* HitTarget : HitTargets)
	{
		if (HitTarget == target)
		{
			return;
		}
	}
	HitCount--;
	
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				AEVec2 DrawSize;
				
				if (target->bIsPandingKill)
				{
					AEVec2Set(&DrawSize, target->size.x + 10.f, target->size.y + 10.f);
					GS->VFXManager.AddNewVFX(VFXType::KillSmoke, target->position, DrawSize, 0.5f);
				}
				else
				{
					AEVec2Set(&DrawSize, size.x + 10.f, size.y + 10.f);
					GS->VFXManager.AddNewVFX(VFXType::HitMark, target->position, DrawSize, 0.3f);
				}
			}
		}
	
	if (isExplosive)
	{
		Explode(target);
		BulletSpeed = 0.f;
		CollisionRadius = 0.f;
		HitTargets.push_back(target);
	}
}

void InGame::Projectile::Explode(class EnemyCharacter* target)
{
	Manager::SFXManager.Play("explosion");

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