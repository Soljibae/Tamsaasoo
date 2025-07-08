#include "Playing.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include "../Manager/GameManager.h"
#include "../Manager/LevelUpUI.h"
#include "../Manager/HUDController.h"
#include "PauseUI.h"
#include "ExpUI.h"
#include "GunPickUI.h"
#include "GameOver.h"
#include <iostream>
#include <cmath>
#include <random>

namespace Manager
{
	Utils::Camera* CAM = nullptr;

	void Playing::Init()
	{
		if (CurrentStage == nullptr)
		{
			CurrentStage = new InGame::Stage1();
		}
		CurrentStageType = CurrentStage->Type;
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
			InitStage();
		}
		if (ITDB == nullptr)
		{
			ITDB = new ItemDatabase();
			ITDB->Init();
		}
		if (ITRM == nullptr)
		{
			ITRM = new Manager::ItemResourceManager();
			ITRM->Init();
		}
		InGame::Item::StaticInit();
		InGame::SoulOrb::StaticInit();
		for (int i = 0; i < 1000; i++)
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
		pausePanel.Init(PC);
		pickPanel.Init(PC);
		ExpPanel.Init(PC);
		HUD.Init(PC, PC->HoldingGun);
		gunPickPanel.Init(PC);
		gm.GamePaused = false;
		gameOverScreen.Init();
		Utils::TestInit();
		WM.Init();
		bIsJumping = false;
		VFXManager.Init();
	}
	void Playing::Update()
	{
		// Press ESCAPE to pause the game
		ExpPanel.Update();
		if (global::KeyInput(AEVK_G))
		{
			gunPickPanel.Show();
		}
		global::CurrentStageNumber = static_cast<s32>(CurrentStageType) + 1;
		if (global::KeyInput(AEVK_ESCAPE) && !pickPanel.IsActive() && !gunPickPanel.IsActive())
		{
			if (!gm.GamePaused)
			{
				gm.Pause();
			}
			else
			{
				gm.Resume();
			}
		}
		if (!gm.GamePaused)
		{
			if (bIsJumping)
			{
				JumpAnimationTimer += global::DeltaTime;
				Utils::UpdateOffset(*PC);
				if (JumpAnimationTimer >= 0.4f)
				{
					bIsJumping = false;
					ChangeStage();
				}
				return;
			}
			if (global::IsEnemyRecentlyDied)
			{
				static f32 cooldown = 0.f;

				cooldown += global::DeltaTime;
				if (cooldown > 0.5f)
				{
					global::IsEnemyRecentlyDied = false;
					cooldown = 0.f;
				}
			}
			//
			if (global::KeyInput(AEVK_1))
			{
				PC->AddItemToInventory(ITDB->itemList[25]->Clone());
			}
			if (global::KeyInput(AEVK_2))
			{
				PC->AddItemToInventory(ITDB->itemList[24]->Clone());
			}
			if (global::KeyInput(AEVK_3))
			{
				PC->AddItemToInventory(ITDB->itemList[33]->Clone());
			}
			if (global::KeyInput(AEVK_4))
			{
				PC->AddItemToInventory(ITDB->itemList[13]->Clone());
			}
			if (global::KeyInput(AEVK_5))
			{
				PC->AddItemToInventory(ITDB->itemList[15]->Clone());
			}
			if (global::KeyInput(AEVK_6))
			{
				PC->AddItemToInventory(ITDB->itemList[6]->Clone());
			}
			if (global::KeyInput(AEVK_O))
			{
				PC->Stats->MaxHP += 2;
				PC->Stats->HP += 3;
			}
			//
			if (!bIsBossFight)
			{
				WaveTimer += global::DeltaTime;
			}
			/*--------------------------------DEBUG FOR LATENCY--------------------------------*/
			if (global::DeltaTime > 0.02)
			{
				std::cout << global::DeltaTime << std::endl;
			}
			/*--------------------------------DEBUG FOR LATENCY--------------------------------*/
			if (!gameOverScreen.isGameOver)
			{
				if (WaveTimer > 3.f)
				{
					WaveCount++;
					WaveTimer = 0;
					if ((WaveCount > 60 && CurrentStageType == InGame::StageType::LAND) || (WaveCount > 100 && CurrentStageType == InGame::StageType::TOWER) || (WaveCount > 140 && CurrentStageType == InGame::StageType::HEAVEN))
					{//60, 100, 140
						InitBossFight();
					}
					else
					{
						SpawnWave();
					}
				}
			}
			PC->Update();
			global::RecentlyDeadEnemyCount = 0;

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
							PC->OnProjectileHit(EC, false);
							PP->OnHit(EC);
						}
					}
				}
				if (!PP->bIsPandingKill)
				{
					if (Boss)
					{
						if (!Boss->bIsPandingKill)
						{
							if (Utils::CheckCollision(*PP, *Boss))
							{
								Boss->adjustHealth(-PP->Damage * global::additionalDamageToBossRatio);
								PC->OnProjectileHit(Boss, true);
								PP->OnHit(Boss);
							}
						}
					}
				}
			}
			for (InGame::SoulOrb*& SO : SOs)
			{
				SO->Update();
			}
			std::vector<InGame::Character*> PCV;
			PCV.push_back(PC);
			for (InGame::ArealAttack*& EAA : EAAs)
			{
				EAA->Update(PCV);
			}
			std::vector<InGame::Character*> ECV;
			for (InGame::Character* EC : ECs)
			{
				ECV.push_back(EC);
			}
			ECV.push_back(PC);
			for (InGame::ArealAttack*& PAA : PAAs)
			{
				PAA->Update(ECV);
			}
			if (!PC->bIsDashing)
			{
				for (InGame::EnemyCharacter*& EC : ECs)
				{
					if (Utils::CheckCollision(*EC, *PC))
					{
						PC->OnDamaged();
						if (global::isBossBattleStarted)
						{
							PC->adjustHealth(-EC->Stats->Damage * global::additionalDamageFromBossRatio);
						}
						else
						{
							PC->adjustHealth(-EC->Stats->Damage);
						}
						
					}
				}
				for (InGame::Projectile*& EP : EPs)
				{
					if (Utils::CheckCollision(*EP, *PC))
					{
						PC->OnDamaged();
						if (global::isBossBattleStarted)
						{
							PC->adjustHealth(-EP->Damage * global::additionalDamageFromBossRatio);
						}
						else
						{
							PC->adjustHealth(-EP->Damage);
						}
						EP->OnHit();
					}
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
					global::RecentlyDeadEnemyCount++;

					InGame::SoulOrb* orb = new InGame::SoulOrb;
					orb->Init(EC);
					SOs.push_back(orb);

					if (EC->Type == InGame::EnemyType::BOMBER)
					{
						float distToPlayer = AEVec2Distance(&EC->position, &global::PlayerLocation);
						if (distToPlayer <= EC->explosionRadius)
						{
							PC->OnDamaged();
							PC->adjustHealth(-EC->explosionDamage);
						}
						for (InGame::EnemyCharacter* enemy : ECs)
						{
							if (enemy != EC)
							{
								float distToEnemy = AEVec2Distance(&EC->position, &enemy->position);
								if (distToEnemy <= EC->explosionRadius)
								{
									enemy->adjustHealth(-EC->explosionDamage);
								}
							}
						}
					}
					global::IsEnemyRecentlyDied = true;
					global::RecentlyDeadEnemyPosition = EC->position;
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

			for (auto it = EAAs.begin(); it != EAAs.end(); )
			{
				if (it == EAAs.end() || *it == nullptr)
					break;

				InGame::ArealAttack* attack = *it;
				if (attack->bIsPandingKill)
				{
					attack->Destroy();
					delete attack;
					it = EAAs.erase(it);
				}
				else
				{
					++it;
				}
			}
			for (auto it = PAAs.begin(); it != PAAs.end(); )
			{
				if (it == PAAs.end() || *it == nullptr)
					break;

				InGame::ArealAttack* attack = *it;
				if (attack->bIsPandingKill)
				{
					attack->Destroy();
					delete attack;
					it = PAAs.erase(it);
				}
				else
				{
					++it;
				}
			}
			for (size_t i = 0; i < SOs.size();)
			{
				if (SOs[i]->bIsPandingKill)
				{
					delete SOs[i];

					SOs.erase(SOs.begin() + i);
				}
				else
				{
					++i;
				}
			}
			CAM->Update(*PC);

			if(Boss)
			{
				if (Boss->bIsPandingKill)
				{
					Boss->Destroy();
					delete Boss;
					Boss = nullptr;
					bIsBossFight = false;
					FinishBossFight();

				}
				else
				{
					Boss->Update();
					if (Utils::CheckCollision(*Boss, *PC))
					{
						PC->adjustHealth(-Boss->Stats->Damage);
						Boss->OnPlayerHit();
					}
				}
			}

			if (PC->bIsPandingKill)
			{
				gameOverScreen.isGameOver = true;
			}
			HUD.Update();
			VFXManager.Update();
			gameOverScreen.Update();
		}
		else if (pickPanel.IsActive())
		{
			pickPanel.Update();
		}
		else if (gunPickPanel.IsActive())
		{
			gunPickPanel.Update();
		}
		else
		{
			pausePanel.Update();
		}
	}
	void Playing::Draw()
	{
		BG->Draw();
		PC->Draw();
		for (InGame::EnemyCharacter* EC : ECs)
		{
			if (abs(EC->position.x - PC->position.x) < global::ScreenWidth / 2 || abs(EC->position.y - PC->position.y) < global::ScreenHeight / 2)
			{
				EC->Draw();
			}
		}
		for (InGame::Projectile* PP : PPs)
		{
			if (abs(PP->position.x - PC->position.x) < global::ScreenWidth / 2 || abs(PP->position.y - PC->position.y) < global::ScreenHeight / 2)
			{
				PP->Draw();
			}
		}
		for (const auto& item_ptr : PC->inventory)
		{
			item_ptr.first->Draw();
		}
		for (InGame::Projectile* EP : EPs)
		{
			if (abs(EP->position.x - PC->position.x) < global::ScreenWidth / 2 || abs(EP->position.y - PC->position.y) < global::ScreenHeight / 2)
			{
				EP->Draw();
			}
		}
		for (auto it = EAAs.begin(); it != EAAs.end(); )
		{
			if (it == EAAs.end() || *it == nullptr)
			{
				break;
			}
			InGame::ArealAttack* attack = *it;
			(*it)->Draw();
			++it;
		}
		for (auto it = PAAs.begin(); it != PAAs.end(); )
		{
			if (it == PAAs.end() || *it == nullptr)
			{
				break;
			}
			InGame::ArealAttack* attack = *it;
			(*it)->Draw();
			++it;
		}
		if (Boss)
		{
			Boss->Draw();
		}
		VFXManager.Draw();
		HUD.Draw();
		ExpPanel.Draw();
		for (InGame::SoulOrb* SO : SOs)
		{
			SO->Draw();
		}
		if (pickPanel.IsActive())
		{
			pickPanel.Draw();
		}
		else if (gunPickPanel.IsActive())
		{
			gunPickPanel.Draw();
		}
		else if (gm.GamePaused)
		{
			pausePanel.Draw();
		}
		gameOverScreen.Draw();
	}
	void Playing::Destroy()
	{
		VFXManager.Destroy();
		HUD.Destroy();
		gameOverScreen.Destroy();
		for (InGame::Projectile* PP : PPs)
		{
			PP->Destroy();
			delete PP;
		}
		PPs.clear();
		for (InGame::Projectile* PP : PPPool)
		{
			PP->Destroy();
			delete PP;
		}
		PPPool.clear();
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->Destroy();
			delete EC;
		}
		ECs.clear();
		for (InGame::EnemyCharacter* EC : ECPool)
		{
			EC->Destroy();
			delete EC;
		}
		ECPool.clear();
		for (InGame::Projectile* EP : EPs)
		{
			EP->Destroy();
			delete EP;
		}
		EPs.clear();
		for (InGame::Projectile* EP : EPPool)
		{
			EP->Destroy();
			delete EP;
		}
		EPPool.clear();
		for (InGame::ArealAttack* PAA : PAAs)
		{
			PAA->Destroy();
			delete PAA;
		}
		PAAs.clear();
		for (InGame::ArealAttack* EAA : EAAs)
		{
			EAA->Destroy();
			delete EAA;
		}
		for (InGame::SoulOrb* SO : SOs)
		{
			SO->Destroy();
			delete SO;
		}
		EAAs.clear();

		delete CAM;
		CAM = nullptr;
		BG->Destroy();
		delete BG;

		if (Boss)
		{
			Boss->Destroy();
			delete Boss;
			Boss = nullptr;
		}
		if (CurrentStage)
		{
			delete CurrentStage;
			CurrentStage = nullptr;
		}
		delete ITDB;
		ITDB = nullptr;
		ITRM->Destroy();
		delete ITRM;

		ExpPanel.Destroy();
		pausePanel.Destroy();
		pickPanel.Destroy();
		gunPickPanel.Destory();
		Utils::TestDestroy();
		WM.Destroy();
		InGame::SoulOrb::StaticDestroy();

		PC->Destroy();
		delete PC;
		PC = nullptr;
	}
	void Playing::SpawnWave()
	{
		std::cout << "Spawn Wave" << std::endl;
		WM.GetNextList();
		if (ECPool.size() >= std::stoi(WM.CurrList.MinionNum) + std::stoi(WM.CurrList.ArcherNum) + std::stoi(WM.CurrList.DasherNum) + std::stoi(WM.CurrList.TankerNum) + std::stoi(WM.CurrList.BomberNum))
		{
			for (int i = 0; i < std::stoi(WM.CurrList.MinionNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &MinionStruct);
				ECs.push_back(EC);
			}
			for (int i = 0; i < std::stoi(WM.CurrList.ArcherNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &ArcherStruct);
				ECs.push_back(EC);
			}
			for (int i = 0; i < std::stoi(WM.CurrList.DasherNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &DasherStruct);
				ECs.push_back(EC);
			}
			for (int i = 0; i < std::stoi(WM.CurrList.TankerNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &TankerStruct);
				ECs.push_back(EC);
			}
			for (int i = 0; i < std::stoi(WM.CurrList.BomberNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &BomberStruct);
				ECs.push_back(EC);
			}
		}
	}
	void Playing::ClearWave()
	{
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->bIsPandingKill = true;
		}
		for (InGame::Projectile*& EP : EPs)
		{
			EP->bIsPandingKill = true;
		}
	}
	void Playing::InitBossFight()
	{
		ClearWave();
		bIsBossFight = true;
		if (Boss == nullptr)
		{
			switch (CurrentStageType)
			{
			case InGame::StageType::LAND:
				Boss = new InGame::Stage1Boss();
				break;
			case InGame::StageType::TOWER:
				Boss = new InGame::Stage2Boss();
				break;
			case InGame::StageType::HEAVEN:
				Boss = new InGame::Stage3Boss();
				break;
			}
			Boss->Texture = AEGfxTextureLoad(CurrentStage->BossTextureAddress.c_str());
			Boss->Init();
		}

		global::isBossBattleStarted = true;
	}
	void Playing::FinishBossFight()
	{
		//TODO : Select Gun
		gunPickPanel.Show();
		//TODO : Play Jump Animation
		for (size_t i = 0; i < EPs.size(); i++)
		{
			InGame::Projectile*& EP = EPs[i];
			EPPool.push_back(EP);
			EP->bIsPandingKill = false;
		}
		EPs.clear();
		for (size_t i = 0; i < PPs.size(); i++)
		{
			InGame::Projectile*& PP = PPs[i];
			PPPool.push_back(PP);
			PP->bIsPandingKill = false;
		}
		PPs.clear();

		bIsJumping = true;
		JumpAnimationTimer = 0.f;
		PC->AnimationState = InGame::EAnimationState::JUMP;
		PC->AnimationCount = 0;
	}
	void Playing::ChangeStage()
	{
		if (PC)
		{
			AEVec2Set(&PC->position, 0.f, 0.f);
		}
		switch (CurrentStageType)
		{
		case InGame::StageType::LAND:
			CurrentStageType = InGame::StageType::TOWER;
			if (CurrentStage)
			{
				delete CurrentStage;
			}
			CurrentStage = new InGame::Stage2();
			break;
		case InGame::StageType::TOWER:
			CurrentStageType = InGame::StageType::HEAVEN;
			if (CurrentStage)
			{
				delete CurrentStage;
			}
			CurrentStage = new InGame::Stage3();
			break;
		case InGame::StageType::HEAVEN:
			Manager::gm.nextState = EGameState::MAINMENU;
			break;
		}
		WaveCount = 0;
		InitStage();
		global::isBossBattleStarted = false;
	}
	void Playing::InitStage()
	{
		if (CurrentStage)
		{
			if (BG)
			{
				if (BG->Texture)
				{
					AEGfxTextureUnload(BG->Texture);
				}
				BG->Texture = AEGfxTextureLoad(CurrentStage->MapTextureAddress.c_str());
			}
		}
	}
	AEVec2 Playing::GetSpawnLocation()
	{
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
		return SpawnPos;
	}
}