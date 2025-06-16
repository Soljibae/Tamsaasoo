#include "Playing.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include "../Manager/GameManager.h"
#include <iostream>
#include <cmath>
#include <random>

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
		for (int i = 0;i < 1000;i++)
		{
			InGame::Projectile* PP = new InGame::Projectile();
			PP->Init();
			PPPool.push_back(PP);
			InGame::Projectile* EP = new InGame::Projectile();
			EP->Init();
			EPPool.push_back(EP);
			InGame::EnemyCharacter* EC = new InGame::EnemyCharacter();
			EC->Init();
			ECPool.push_back(EC);
		}
		WaveTimer = 0.;
		SpawnCount = 10;
	}
	void Playing::Update()
	{
		WaveTimer += global::DeltaTime;
		if (global::DeltaTime > 0.02)
		{
			std::cout << global::DeltaTime << std::endl;
		}
		if (WaveTimer > 10.f)
		{
			WaveTimer = 0;
			SpawnCount+=10;
			if (SpawnCount > 50)
			{
				SpawnCount = 10;
				SpawningEnemyType = InGame::GetNextEnemyType(SpawningEnemyType);
			}
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
				else if (EC->bIsPandingKill)
				{
					continue;
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
		for (InGame::EnemyCharacter*& EC : ECs)
		{
			if (Utils::CheckCollision(*EC, *PC))
			{
				PC->adjustHealth(-EC->Stats.Damage);
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
		for (size_t i = 0; i < PPs.size(); )
		{
			InGame::Projectile*& PP = PPs[i];

			if (PP->bIsPandingKill)
			{
				PPPool.push_back(PP);
				PP->bIsPandingKill = false;
				PPs[i] = PPs.back();
				PPs.pop_back();
			}
			else
			{
				++i;
			}
		}
		for (size_t i = 0; i < ECs.size(); )
		{
			InGame::EnemyCharacter*& EC = ECs[i];

			if (EC->bIsPandingKill)
			{
				PC->UpdateKill(EC->Exp);
				EC->bIsPandingKill = false;
				ECPool.push_back(EC);
				ECs[i] = ECs.back();
				ECs.pop_back();
			}
			else
			{
				++i;
			}
		}
		for (size_t i = 0; i < EPs.size(); )
		{
			InGame::Projectile*& EP = EPs[i];

			if (EP->bIsPandingKill)
			{
				EPPool.push_back(EP);
				EP->bIsPandingKill = false;
				EPs[i] = EPs.back();
				EPs.pop_back();
			}
			else
			{
				++i;
			}
		}
		CAM->Update(*PC);
		if (PC->bIsPandingKill)
		{
			Manager::gm.nextState = EGameState::MAINMENU;
		}
	}
	void Playing::Draw()
	{
		BG->Draw();
		PC->Draw();
		for (InGame::Projectile* PP : PPs)
		{
			if (abs(PP->position.x - PC->position.x) < global::ScreenWidth / 2 || abs(PP->position.y - PC->position.y) < global::ScreenHeight / 2)
			{
				PP->Draw();
			}
		}
		for (InGame::EnemyCharacter* EC : ECs)
		{
			if (abs(EC->position.x - PC->position.x) < global::ScreenWidth / 2 || abs(EC->position.y - PC->position.y) < global::ScreenHeight / 2)
			{
				EC->Draw();
			}
		}
		for (InGame::Projectile* EP : EPs)
		{
			if (abs(EP->position.x - PC->position.x) < global::ScreenWidth / 2 || abs(EP->position.y - PC->position.y) < global::ScreenHeight / 2)
			{
				EP->Draw();
			}
		}
	}
	void Playing::Destroy()
	{
		PC->Destroy();
		delete PC;
		PC = nullptr;
		bool bSuccess = false;
		for (InGame::Projectile* PP : PPs)
		{
			PP->Destroy();
			delete PP;
		}
		bSuccess = PPs.empty();
		for (InGame::Projectile* PP : PPPool)
		{
			PP->Destroy();
			delete PP;
		}
		bSuccess = PPPool.empty();
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->Destroy();
			delete EC;
		}
		bSuccess = ECs.empty();
		for (InGame::EnemyCharacter* EC : ECPool)
		{
			EC->Destroy();
			delete EC;
		}
		bSuccess = ECPool.empty();
		for (InGame::Projectile* EP : EPs)
		{
			EP->Destroy();
			delete EP;
		}
		bSuccess = EPs.empty();
		for (InGame::Projectile* EP : EPPool)
		{
			EP->Destroy();
			delete EP;
		}
		bSuccess = EPPool.empty();
		delete CAM;
		CAM = nullptr;
		BG->Destroy();
		delete BG;
	}
	void Playing::SpawnWave()
	{
		std::cout << "Spawn Wave" << std::endl;
		for (u8 i = 0; i < SpawnCount;i++)
		{
			if (ECPool.size() == 0)
			{
				break;
			}
			InGame::EnemyCharacter* EC = ECPool.back();
			ECPool.pop_back();
			AEVec2 SpawnPos;
			std::uniform_real_distribution<> angleDist(0.0, 2 * 3.141592);
			std::uniform_real_distribution<> radiusDist(0.0, 1.0);
			while (true)
			{
				static std::random_device rd;
				static std::mt19937 gen(rd());
				double theta = angleDist(gen);
				double distance = global::ScreenWidth * 1.5 * std::sqrt(radiusDist(gen));

				SpawnPos.x = PC->position.x + distance * std::cos(theta);
				SpawnPos.y = PC->position.y + distance * std::sin(theta);
				if (SpawnPos.x < global::worldMax.x &&
					SpawnPos.x > global::worldMin.x &&
					SpawnPos.y < global::worldMax.y &&
					SpawnPos.y > global::worldMin.y &&
					(SpawnPos.x < PC->position.x - global::ScreenWidth / 2 || SpawnPos.x > PC->position.x + global::ScreenWidth / 2 ||
						SpawnPos.y < PC->position.y - global::ScreenHeight / 2 || SpawnPos.y > PC->position.y + global::ScreenHeight / 2)
					)
				{
					break;
				}
			}
			switch (SpawningEnemyType)
			{
			case InGame::EnemyType::MINION:
				EC->Spawn(SpawnPos, &MinionStruct);
				break;
			case InGame::EnemyType::ARCHER:
				EC->Spawn(SpawnPos, &ArcherStruct);
				break;
			}
			ECs.push_back(EC);
		}
	}
}