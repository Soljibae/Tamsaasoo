#include "Projectile.h"
#include "../Utils/Utils.h"

void InGame::Projectile::Init(AEVec2 Dir, AEVec2 Pos)
{
	Mesh = Utils::CreateMesh();
	Texture = AEGfxTextureLoad("Assets/TestBlankWhite.png");
	size.x = 10;
	size.y = 10;
	direction = Dir;
	position = Pos;
}

void InGame::Projectile::Update()
{
	position.x += direction.x;
	position.y += direction.y;
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
