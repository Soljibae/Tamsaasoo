#include "Playing.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include <iostream>

namespace Manager
{
	Utils::Camera* CAM = nullptr;

	void Playing::Init()
	{
		if (PC == nullptr)
		{
			PC = new InGame::PlayerCharacter();
			PC->Init();
		}

		if (CAM == nullptr)
		{
			CAM = new Utils::Camera();
			CAM->Init(*PC);
		}
		if (BG == nullptr)
		{
			BG = new InGame::Background();
			BG->Init();
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
		for (InGame::Projectile*& PP : PPs)
		{
			PP->Update();
			PP->IsOutOfWorld();
		}
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->Update();
		}
		for (InGame::Projectile*& EP : EPs)
		{
			EP->Update();
			EP->IsOutOfWorld();
		}
		for (InGame::Projectile*& PP : PPs)
		{
			bool bIsHit = false;
			for (InGame::EnemyCharacter*& EC : ECs)
			{
				if (PP->bIsPandingKill)
				{
					break;
				}
				else
				{
					if (Utils::CheckCollision(*PP, *EC))
					{
						EC->adjustHealth(-PP->Damage);
						PP->OnHit();
					}
				}
			}
		}
		for (InGame::Projectile*& EP : EPs)
		{
			if (Utils::CheckCollision(*EP, *PC))
			{
				PC->adjustHealth(-EP->Damage);
				EP->OnHit();
			}
		}
		for (InGame::Projectile*& PP : PPs)
		{
			if (PP->bIsPandingKill)
			{
				PP->Destroy();
				delete PP;
				PP = nullptr;
			}
		}
		PPs.erase(std::remove(PPs.begin(), PPs.end(), nullptr), PPs.end());
		for (InGame::EnemyCharacter*& EC : ECs)
		{
			if (EC->bIsPandingKill)
			{
				EC->Destroy();
				delete EC;
				EC = nullptr;
			}
		}
		ECs.erase(std::remove(ECs.begin(), ECs.end(), nullptr), ECs.end());
		for (InGame::Projectile*& EP : EPs)
		{
			if (EP->bIsPandingKill)
			{
				EP->Destroy();
				delete EP;
				EP = nullptr;
			}
		}
		EPs.erase(std::remove(EPs.begin(), EPs.end(), nullptr), EPs.end());

		CAM->Update(*PC);
	}
	void Playing::Draw()
	{
		BG->Draw();
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
		delete CAM;
		CAM = nullptr;
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