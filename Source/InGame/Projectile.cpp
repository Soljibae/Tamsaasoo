#include "Projectile.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"


void InGame::Projectile::Init()
{
	Mesh = Utils::CreateMesh();
	Texture = AEGfxTextureLoad("Assets/icon.ico");
}

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos, PlayerCharacter* object)
{
	size.x = 100;
	size.y = 100;
	direction = Dir;
	position = Pos;
	CollisionRadius = 5;
	bIsPandingKill = false;

	BulletSpeed = object->Stats.BulletSpeed;
	HitCount = object->Stats.HitCount;
	Damage = object->Stats.Damage;

}

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos, EnemyCharacter* object)
{
	size.x = 100;
	size.y = 100;
	direction = Dir;
	position = Pos;
	CollisionRadius = 5;
	bIsPandingKill = false;

	BulletSpeed = object->Stats.BulletSpeed;
	Damage = object->Stats.Damage;
}

void InGame::Projectile::Update()
{
	position.x += direction.x * BulletSpeed;
	position.y += direction.y * BulletSpeed;
}

void InGame::Projectile::Draw()
{
	Utils::DrawObjectWithDirection(*this);
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
