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
#include "BossAppearScene.h"
#include <iostream>
#include <cmath>
#include <random>
#include "SettingUI.h"

namespace Manager
{
	Utils::Camera* CAM = nullptr;
	BossAppearScene bossAppearScene;
	const static f32 fontSize = 72.f;
	const static f32 textDrawSize = 0.35f;
	const static s32 maxWaveCount = 60;
	void Playing::Init()
	{
		Fader.Mesh = Utils::CreateMesh();
		Fader.Texture = AEGfxTextureLoad("Assets/black.png");
		Fader.position = { 0, 0 };
		Fader.size = { static_cast<f32>(global::ScreenWidth), static_cast<f32>(global::ScreenHeight) };
		Fader.Alpha = 1.f;
		SFXManager.Init();
		SFXManager.AddNewSFX(InGame::BGM, "Assets/SFX/BGM/BGM.mp3", "doom");

		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Attack/dasherAttack.wav", "dasherAttack");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/slime.wav", "slime");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/skeleton.wav", "skeleton");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/tanker.wav", "tanker");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/bomber.wav", "bomber");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/dasherDead.wav", "dasherDead");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/sniper.wav", "sniper");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/zigzag.wav", "zigzag");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/orbiter.wav", "orbiter");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/burner.wav", "burner");
		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Enemy/Dead/holer.wav", "holer");

		SFXManager.AddNewSFX(InGame::SFX, "Assets/SFX/Boss/siren.wav", "siren");
		SFXManager.Play("doom");
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
		StageTimer = 3.f * maxWaveCount + global::DeltaTime;
		if (global::DeltaTime > 0.1)
		{
			StageTimer += global::DeltaTime;
		}
		WaveTimer = 0.;
		InGame::EnemyCharacter::StaticInit();
		InGame::Projectile::StaticInit();
		pausePanel.Init(PC);
		pickPanel.Init(PC);
		ExpPanel.Init(PC);
		HUD.Init(PC, PC->HoldingGun);
		gunPickPanel.Init(PC);
		gameOverScreen.Init();
		Utils::TestInit();
		WM.Init();
		bIsJumping = false;
		VFXManager.Init();
		pFont = AEGfxCreateFont("Assets/Fonts/buggy-font.ttf", fontSize);
		gm.GamePaused = false;
	}

	void Playing::Update()
	{
		if (global::DeltaTime > 0.1)
		{
			std::cout << global::DeltaTime << std::endl;
			return;
		}

		if (Fader.Alpha > 0.f)
		{
			Fader.Alpha -= global::DeltaTime * 2.f;
			return;
		}
		else
		{
			Fader.Alpha = 0.f;
			if (!startWeaponPicked)
			{
				startWeaponPicked = true;
				gunPickPanel.Show();
			}
		}

		ExpPanel.Update();
		
		if (global::KeyInput(AEVK_F1))
		{
			WaveCount = maxWaveCount;
		}
		if (global::KeyInput(AEVK_F2))
		{
			PC->PS->ExpCount += 1300;
		}
		global::CurrentStageNumber = static_cast<s32>(CurrentStageType) + 1;

		static bool prevSettingPanelState = false;
		static bool isDelayOn = false;
		static f32 timeAccForDelay = 0.f;

		if (prevSettingPanelState && !SettingPanel.isSettingOn)
		{
			isDelayOn = true;
			timeAccForDelay = 0.f;
		}
			
		bool canPause = global::KeyInput(AEVK_ESCAPE) && !pickPanel.IsActive() && !gunPickPanel.IsActive() && !gameOverScreen.isGameOver;
		if (canPause)
		{
			if (!gm.GamePaused)
			{
				gm.Pause();
			}
			else
			{
				if(!isDelayOn)
					gm.Resume();
			}
		}
		if (isDelayOn)
		{
			timeAccForDelay += global::DeltaTime;
			if (timeAccForDelay > 0.5f)
			{
				isDelayOn = false;
			}
		}
		prevSettingPanelState = SettingPanel.isSettingOn;

		if (!gm.GamePaused)
		{
			if (bIsJumping)
			{
				JumpAnimationTimer += global::DeltaTime;
				Utils::UpdateOffset(*PC);
				if (JumpAnimationTimer >= 2.f)
				{
					bIsJumping = false;
					ChangeStage();
					isChangingStage = true;
				}
				return;
			}
			/*if (global::KeyInput(AEVK_9))
			{
				if (global::isTestMod)
				{
					global::isTestMod = false;
					for (InGame::EnemyCharacter* EC : ECs)
					{
						EC->bIsPandingKill = true;
					}
				}
				else
				{
					global::isTestMod = true;
					for (InGame::EnemyCharacter* EC : ECs)
					{
						EC->bIsPandingKill = true;
					}
				}
			}*/
			//
			if (!bIsBossFight)
			{
				WaveTimer += global::DeltaTime;
				if (!gameOverScreen.isGameOver)
				{
					StageTimer -= global::DeltaTime;
					StageTimer = StageTimer < 0.f ? 0.f : StageTimer;
					if (WaveTimer > 3.f && !global::isTestMod)
					{
						WaveCount++;
						WaveTimer = 0;
						if (WaveCount > maxWaveCount)
						{
							InitBossFight();
							Boss->bossApearing = true;
							bossAppearScene.Init(Boss);
							SFXManager.Play("siren");
						}
						else
						{
							SpawnWave();
						}
					}
				}
			}
			if (!Boss || (Boss && !Boss->bossApearing))
			{
				PC->Update();
			}
			global::RecentlyDeadEnemyCount = 0;

			if (global::isTestMod && ECs.size() == 0)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn({0.f ,0.f}, &MinionStruct);
				ECs.push_back(EC);
				ECs[0]->Stats->MaxHP = 999;
				ECs[0]->Stats->HP = ECs[0]->Stats->MaxHP;
			}

			if (global::isTestMod)
			{
				static f32 prev_hp = ECs[0]->Stats->HP;
				f32 curr_hp = ECs[0]->Stats->HP;
				f32 damage_this_frame = prev_hp - curr_hp;

				static f32 total_damage_in_window = 0.f;
				static f32 time_accumulator = 0.f;
				const f32 dps_update_interval = 1.0f;

				if (damage_this_frame > 0)
				{
					total_damage_in_window += damage_this_frame;
				}

				time_accumulator += global::DeltaTime;

				if (time_accumulator >= dps_update_interval)
				{
					f32 average_dps = total_damage_in_window / time_accumulator;

					std::cout << "Average DPS (last " << time_accumulator << "s): " << average_dps << std::endl;
					std::cout << "HP: " << curr_hp << std::endl;

					time_accumulator = 0.f;
					total_damage_in_window = 0.f;
				}

				prev_hp = curr_hp;
			}
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
			std::vector<InGame::EnemyCharacter*> chars;
			for (InGame::EnemyCharacter* ec : ECs)
			{
				chars.push_back(ec);
			}
			for (InGame::Projectile* PP : PPs)
			{
				Utils::CheckCollision(*PP, chars, *PC, false);
				if (Boss)
				{
					if (!Boss->bIsPandingKill)
					{
						std::vector<InGame::EnemyCharacter*> Bosses;
						Bosses.push_back(Boss);
						Utils::CheckCollision(*PP, Bosses, *PC, true);
					}
				}
/*				bool bIsHit = false;
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
							if (PP->hasHit)
								continue;
							if (PP->isExplosive)
							{
								PP->hasHit = true;
							}
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
				}*/
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
			for (InGame::ArealAttack*& EAA : EAAs)
			{
				EAA->Update(ECV);
			}
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
			if (global::IsEnemyRecentlyDied)
			{
				global::IsEnemyRecentlyDied = false;
			}
			for (size_t i = 0; i < ECs.size(); )
			{
				InGame::EnemyCharacter*& EC = ECs[i];


				if (EC->bIsPandingKill)
				{
					global::RecentlyDeadEnemyCount++;

					for (int i = 0;i < 3;i++)
					{
						InGame::SoulOrb* orb = new InGame::SoulOrb;
						orb->Init(EC);
						SOs.push_back(orb);
					}
					

					float distToPlayer = AEVec2Distance(&EC->position, &global::PlayerLocation);
					switch (EC->Type)
					{
					case InGame::EnemyType::MINION:
						SFXManager.Play("slime");
						break;
					case InGame::EnemyType::ARCHER:
						SFXManager.Play("skeleton");
						break;
					case InGame::EnemyType::DASHER:
						SFXManager.Play("dasherDead");
						break;
					case InGame::EnemyType::TANKER:
						SFXManager.Play("tanker");
						break;
					case InGame::EnemyType::BOMBER:
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
						AEVec2 DrawSize;
						AEVec2Set(&DrawSize, EC->explosionRadius * 2, EC->explosionRadius * 2);
						VFXManager.AddNewVFX(InGame::VFXType::Explosion, EC->position, DrawSize, 1.f);
						SFXManager.Play("bomber");
						break;
					case InGame::EnemyType::ZIGZAG:
						SFXManager.Play("zigzag");
						break;
					case InGame::EnemyType::ORBITER:
						SFXManager.Play("orbiter");
						break;
					case InGame::EnemyType::SNIPER:
						SFXManager.Play("sniper");
						break;
					case InGame::EnemyType::BURNER:
						SFXManager.Play("burner");
						break;
					case InGame::EnemyType::HOLER:
						SFXManager.Play("holer");
						break;
					}
					global::IsEnemyRecentlyDied = true;
					global::RecentlyDeadEnemyPosition = EC->position;
					PC->UpdateKill(EC->Exp);
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
			if(Boss && Boss->bossApearing)
			{ 
				CAM->Update(*Boss);
			}
			else
			{
				CAM->Update(*PC);
			}

			if(Boss)
			{
				if (Boss->bIsPandingKill)
				{
					Boss->Destroy();
					delete Boss;
					Boss = nullptr;
					bIsBossFight = false;
					FinishBossFight();
					bossAppearScene.Destroy();
				}
				else if (Boss->bossApearing)
				{
					bossAppearScene.Update();
				}
				else if(!Boss->bossApearing)
				{
					Boss->Update();
					if (Utils::CheckCollision(*Boss, *PC))
					{
						PC->adjustHealth(-Boss->Stats->Damage);
						Boss->OnPlayerHit();
					}
					bossHPBar.Update();
				}
			}

			if (PC->bIsPandingKill)
			{
				gameOverScreen.isGameOver = true;
			}
			HUD.Update();
			gameOverScreen.Update();
		}
		else if (pausePanel.wasPickingItem)
		{
			if (global::KeyInput(AEVK_ESCAPE))
			{
				pausePanel.wasPickingItem = false;
			}
			pausePanel.Update();
		}
		else if (pickPanel.IsActive())
		{
			if (!pausePanel.wasPickingItem && global::KeyInput(AEVK_ESCAPE))
			{
				pausePanel.wasPickingItem = true;
			}
			pickPanel.Update();
		}
		else if (isChangingStage)
		{
			static f32 stageChangeTimer{ 0.f };
			stageChangeTimer += global::DeltaTime;
			if (stageChangeTimer > 2.f)
			{
				isChangingStage = false;
				stageChangeTimer = 0.f;
			}
		}
		else if (gunPickPanel.IsActive() && !isChangingStage)
		{
			gunPickPanel.Update();
		}
		else
		{
			pausePanel.Update();
		}
		VFXManager.Update();
		SFXManager.Update();
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
			if (!Boss->bossApearing)
			{
				bossHPBar.Draw();
			}
		}
		VFXManager.Draw();
		if (!bIsBossFight && !gm.GamePaused)
		{
			DrawTime(StageTimer);
		}
		if (!Boss || (Boss && !Boss->bossApearing))
		{
			HUD.Draw();
			ExpPanel.Draw();
		}
		for (InGame::SoulOrb* SO : SOs)
		{
			SO->Draw();
		}
		if (pausePanel.wasPickingItem)
		{
			pausePanel.Draw();
		}
		else if (pickPanel.IsActive())
		{
			pickPanel.Draw();
			Utils::DrawObject(HUD.Coin, false);	
			std::string pText = std::to_string(static_cast<s32>(PC->PS->Money));
			f32 textW, textH;
			AEGfxGetPrintSize(pFont, pText.c_str(), textDrawSize, &textW, &textH);
			AEGfxPrint(pFont, pText.c_str(), (HUD.Coin.position.x + HUD.Coin.size.x / 1.5f) / (global::ScreenWidth / 2), (HUD.Coin.position.y - HUD.Coin.size.y / 2.5f) / (global::ScreenHeight / 2), 0.4f, 1, 1, 1, 1);

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
		if(Boss && Boss->bossApearing)
			bossAppearScene.Draw();
		Utils::DrawObject(Fader, false, Fader.Alpha);
	}
	void Playing::Destroy()
	{
		AEGfxMeshFree(Fader.Mesh);
		AEGfxTextureUnload(Fader.Texture);
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
		AEGfxDestroyFont(pFont);
		InGame::EnemyCharacter::StaticDestroy();
		InGame::Projectile::StaticDestroy();
		ExpPanel.Destroy();
		pausePanel.Destroy();
		pickPanel.Destroy();
		gunPickPanel.Destroy();
		Utils::TestDestroy();
		WM.Destroy();
		InGame::SoulOrb::StaticDestroy();
		PC->Destroy();
		delete PC;
		PC = nullptr;
		SFXManager.Destroy();
	}
	void Playing::SpawnWave()
	{
		WM.GetNextList();
		if (ECPool.size() >= std::stoi(WM.CurrList.MinionNum) + std::stoi(WM.CurrList.ArcherNum) + std::stoi(WM.CurrList.DasherNum) + std::stoi(WM.CurrList.TankerNum) + std::stoi(WM.CurrList.BomberNum) + std::stoi(WM.CurrList.ZigZagNum))
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
			for (int i = 0; i < std::stoi(WM.CurrList.ZigZagNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &ZigZagStruct);
				ECs.push_back(EC);
			}
			for (int i = 0; i < std::stoi(WM.CurrList.OrbiterNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &OrbiterStruct);
				ECs.push_back(EC);
			}
			for (int i = 0; i < std::stoi(WM.CurrList.SniperNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &SniperStruct);
				ECs.push_back(EC);
			}
			for (int i = 0; i < std::stoi(WM.CurrList.BurnerNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &BurnerStruct);
				ECs.push_back(EC);
			}
			for (int i = 0; i < std::stoi(WM.CurrList.HolerNum);i++)
			{
				InGame::EnemyCharacter* EC = ECPool.back();
				ECPool.pop_back();
				EC->Spawn(GetSpawnLocation(), &HolerStruct);
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
			bossHPBar.Init(Boss);
		}

		global::isBossBattleStarted = true;
	}
	void Playing::FinishBossFight()
	{
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
		gunPickPanel.Show();
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
		CAM->Init(*PC);
		StageTimer = 3.f * maxWaveCount;
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
			if ((CurrentStageType == InGame::StageType::LAND|| CurrentStageType == InGame::StageType::HEAVEN)&&
				(SpawnPos.x < global::worldMax.x && SpawnPos.x > global::worldMin.x && SpawnPos.y < global::worldMax.y && SpawnPos.y > global::worldMin.y )&&
				(SpawnPos.x < PC->position.x - global::ScreenWidth / 2 || SpawnPos.x > PC->position.x + global::ScreenWidth / 2 || SpawnPos.y < PC->position.y - global::ScreenHeight / 2 || SpawnPos.y > PC->position.y + global::ScreenHeight / 2))
			{
				break;
			}
			else if ((CurrentStageType == InGame::StageType::TOWER)&&
				(SpawnPos.x < global::worldMax.x && SpawnPos.x > global::worldMin.x && SpawnPos.y < global::worldMax.y && SpawnPos.y > global::worldMin.y) &&
				(SpawnPos.x < PC->position.x - global::ScreenWidth / 2 || SpawnPos.x > PC->position.x + global::ScreenWidth / 2 || SpawnPos.y < PC->position.y - global::ScreenHeight / 2 || SpawnPos.y > PC->position.y + global::ScreenHeight / 2)
				)
			{
				float EllipseA = (global::worldMax.x - global::worldMin.x) / 2;
				float EllipseB = (global::worldMax.y - global::worldMin.y) / 2;
				float value = (SpawnPos.x * SpawnPos.x) / (EllipseA * EllipseA) + (SpawnPos.y * SpawnPos.y) / (EllipseB * EllipseB);
				if (value <= 1.0f)
				{
					break;
				}
			}
		}
		return SpawnPos;
	}
	void Playing::DrawTime(f32 time)
	{
		f32 halfW = global::ScreenWidth / 2.f;
		f32 halfH = global::ScreenHeight / 2.f;
		s32 min = static_cast<s32>(time) / 60;
		s32 sec = static_cast<s32>(time) % 60;
		std::string timer{ std::to_string(min) + ":" + (sec<10 ? "0"+std::to_string(sec) : std::to_string(sec))};
		f32 lw, lh;
		AEGfxGetPrintSize(pFont, timer.c_str(), textDrawSize, &lw, &lh);
		AEGfxPrint(pFont, timer.c_str(), (- lw / 2.f + 5.f/ halfW), (halfH - 100.f -2.f) / halfH, textDrawSize, 0.f, 0.f, 0.f, 1.f);
		AEGfxPrint(pFont, timer.c_str(), -lw/2.f, (halfH-100.f) / halfH, textDrawSize, 1.f, 1.f, 1.f, 1.f);
	}
}