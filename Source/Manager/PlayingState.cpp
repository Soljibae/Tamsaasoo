#include "PlayingState.h"
#include "../Global/GlobalVariables.h"
#include <iostream>
namespace Manager
{
	void Playing::Init()
	{
		if (PC == nullptr)
		{
			PC = new InGame::PlayerCharacter();
			PC->Init();
		}
		WaveTimer = 0.;
		SpawnCount = 0;
	}
	void Playing::Update()
	{
		WaveTimer += global::DeltaTime;
		if (WaveTimer > 10.f)
		{
			WaveTimer = 0;
			SpawnCount++;
			SpawnWave();
		}
		PC->Update();
		for (InGame::Projectile* PP : PPs)
		{
			PP->Update();
		}
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->Update();
		}
		for (InGame::Projectile* EP : EPs)
		{
			EP->Update();
		}
	}
	void Playing::Draw()
	{
		PC->Draw();
		for (InGame::Projectile* PP : PPs)
		{
			PP->Draw();
		}
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->Draw();
		}
		for (InGame::Projectile* EP : EPs)
		{
			EP->Draw();
		}
	}
	void Playing::Destroy()
	{
		PC->Destroy();
		delete PC;
		PC = nullptr;
		for (InGame::Projectile* PP : PPs)
		{
			PP->Destroy();
			delete PP;
		}
		bool bSuccess = PPs.empty();
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->Destroy();
			delete EC;
		}
		bSuccess = ECs.empty();
		for (InGame::Projectile* EP : EPs)
		{
			EP->Destroy();
			delete EP;
		}
		bSuccess = EPs.empty();
	}
	void Playing::SpawnWave()
	{
		std::cout << "Spawn Wave" << std::endl;
		for (u8 i = 0; i < SpawnCount;i++)
		{
			InGame::EnemyCharacter* EC = new InGame::EnemyCharacter();
			EC->Init();
			EC->position.x = 100 * i;
			EC->position.y = 500;
			ECs.push_back(EC);
		}
	}
}