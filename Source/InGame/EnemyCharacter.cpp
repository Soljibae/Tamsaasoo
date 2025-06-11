#include "EnemyCharacter.h"
#include "../Utils/Utils.h"
namespace InGame
{
	void InGame::EnemyCharacter::Init()
	{
		Mesh = Utils::CreateMesh();
	}

	void InGame::EnemyCharacter::Update()
	{

	}
	void EnemyCharacter::Draw()
	{
	}
	void EnemyCharacter::Destroy()
	{
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
		}
	}
}