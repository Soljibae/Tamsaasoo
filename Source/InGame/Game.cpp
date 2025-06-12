#include "Game.h"

namespace InGame
{
	void Game::Init()
	{
		if (PC == nullptr)
		{
			PC = new PlayerCharacter();
			PC->Init();
		}
	}
	void Game::Update()
	{
		PC->Update();
		for (Projectile* PP : PPs)
		{
			PP->Update();
		}
		for (EnemyCharacter* EC : ECs)
		{
			EC->Update();
		}
		for (Projectile* EP : EPs)
		{
			EP->Update();
		}
	}
	void Game::Draw()
	{
		PC->Draw();
		for (Projectile* PP : PPs)
		{
			PP->Draw();
		}
		for (EnemyCharacter* EC : ECs)
		{
			EC->Draw();
		}
		for (Projectile* EP : EPs)
		{
			EP->Draw();
		}
	}
	void Game::Destroy()
	{
		PC->Destroy();
		delete PC;
		PC = nullptr;
		for (Projectile* PP : PPs)
		{
			PP->Destroy();
			delete PP;
		}
		bool bSuccess = PPs.empty();
		for (EnemyCharacter* EC : ECs)
		{
			EC->Destroy();
			delete EC;
		}
		bSuccess = ECs.empty();
		for (Projectile* EP : EPs)
		{
			EP->Destroy();
			delete EP;
		}
		bSuccess = EPs.empty();
	}
}
