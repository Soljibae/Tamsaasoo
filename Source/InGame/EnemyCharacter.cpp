#include "EnemyCharacter.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
namespace InGame
{
	void InGame::EnemyCharacter::Init()
	{
		Mesh = Utils::CreateMesh();
		Texture = AEGfxTextureLoad("Assets/TestBlankRed.png");
		MovementSpeed = 100.f;
		size.x = 40;
		size.y = 40;
		CollisionRadius = 20;

		Stats.HP = 1;
		Stats.MovementSpeed = MovementSpeed;
		Stats.FireRate = 1.f;
		Stats.BulletSpeed = 30.f;
		Stats.Damage = 1;
	}

	void InGame::EnemyCharacter::Update()
	{
		global::PlayerLocation;
		f32 len = AEVec2Distance(&global::PlayerLocation, &position);
		f32 dx = position.x - global::PlayerLocation.x;
		f32 dy = position.y - global::PlayerLocation.y;
		direction.x = dx / len;
		direction.y = dy / len;
		position.x -= direction.x * Stats.MovementSpeed * global::DeltaTime;
		position.y -= direction.y * Stats.MovementSpeed * global::DeltaTime;
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
	void EnemyCharacter::adjustHealth(s32 Amount)
	{
		Stats.HP += Amount;
		if (Stats.HP <= 0)
		{
			bIsPandingKill = true;
		}
	}
}
