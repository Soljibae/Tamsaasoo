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

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos, Actor* object)
{
	size.x = 10;
	size.y = 10;
	direction = Dir;
	position = Pos;
	CollisionRadius = 5;
	bIsPandingKill = false;

	PlayerCharacter* player = dynamic_cast<PlayerCharacter*>(object);
	EnemyCharacter* enemy = dynamic_cast<EnemyCharacter*>(object);

	if (player)
	{
		BulletSpeed = player->Stats.BulletSpeed;
		HitCount = player->Stats.HitCount;
		Damage = player->Stats.Damage;
	}
	else if (enemy)
	{
		BulletSpeed = enemy->Stats.BulletSpeed;
		Damage = enemy->Stats.Damage;
	}
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
	Utils::DestroyMesh(Mesh);
	Mesh = nullptr;
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
