#include "EnemyCharacter.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
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
}