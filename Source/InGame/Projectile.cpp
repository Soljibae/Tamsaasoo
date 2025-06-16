#include "Projectile.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"

void InGame::Projectile::Init()
{
	Mesh = Utils::CreateMesh();
	Texture = AEGfxTextureLoad("Assets/TestBlankBlue.png");
}

void InGame::Projectile::Spawn(AEVec2 Dir, AEVec2 Pos)
{
	size.x = 10;
	size.y = 10;
	direction = Dir;
	position = Pos;
	CollisionRadius = 5;
	bIsPandingKill = false;
}

void InGame::Projectile::Update()
{
	position.x += direction.x * 30.f;
	position.y += direction.y * 30.f;
}

void InGame::Projectile::Draw()
{
	Utils::DrawObject(*this);
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
