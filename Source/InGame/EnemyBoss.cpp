#include "EnemyBoss.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "../Manager/GameManager.h"
#include "../Manager/Playing.h"
#include "ArealAttack.h"
#include <random>
#include <cmath>



namespace InGame
{
	void EnemyBoss::Init()
	{
		Stats = new Stat;
		row = 6;
		column = 2;
		AnimationCount = 0;
		Mesh = Utils::CreateMesh(6,2);
		bIsPandingKill = false;
		Utils::InitOffset(*this);
		FrameTime = 1.0f;
		bossApearTime = 0.f;
		Manager::SFXManager.AddNewSFX(SFX, "Assets/SFX/KnifeThrow.mp3", "BossAttackKnife");
		Manager::SFXManager.AddNewSFX(SFX, "Assets/SFX/FireMagic.mp3", "BossAttackMagic");
	}
	void EnemyBoss::Update()
	{
		Utils::UpdateOffset(*this);
	}
	void EnemyBoss::Draw()
	{
		Utils::DrawObject(*this);
	}
	void EnemyBoss::Destroy()
	{
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
			Mesh = nullptr;
		}
		if (Texture)
		{
			AEGfxTextureUnload(Texture);
			Texture = nullptr;
		}
		if (Stats)
		{
			delete Stats;
			Stats = nullptr;
		}
	}
	void EnemyBoss::OnPlayerHit()
	{
	}
	void Stage1Boss::Init()
	{
		EnemyBoss::Init();
		name = "Eldros";
		epithet = "the Spellweaver";//or Spellbinder
		Stats->MovementSpeed = 100.f;
		size.x = 105;
		size.y = 192;
		position.x = 0;
		position.y = 0;
		CollisionRadius = 50;
		AEVec2Set(&Stats->ProjectileSize, 30, 30);
		ProjectileSpawnTimer = 0;
		ProjectileChamberTimer = 5;
		bIsWaving = false;

		Stats->HP =	400;
		Stats->MaxHP = 400;
		Stats->FireRate = 5.f;
		Stats->ProjectileSpeed = 3.f;
		Stats->Damage = 1;
		Stats->ProjectileCollisionSize = 15.f;
		AnimationState = IDLE;
		MaxAnimationCount[IDLE] = 2;
	}
	void Stage1Boss::Update()
	{
		EnemyBoss::Update();
		UpdateEffectTime();

		static bool angleOffsetToggle = false;

		if (Stats->StatusEffectTimer[BURN] > 0)
		{
			BurnTimer += global::DeltaTime;
			if (BurnTimer >= global::effectiveBurnRate)
			{
				BurnTimer = 0.f;
				adjustHealth(-Stats->MaxHP * global::effectiveBurnDamage / 5);
			}
		}

		if (!(Stats->StatusEffectTimer[STUN] > 0))
		{
			ProjectileSpawnTimer += global::DeltaTime;
			WaveAttackSpawnTimer += global::DeltaTime;

			if (ProjectileSpawnTimer > ProjectileChamberTimer)
			{
				ProjectileSpawnTimer = 0;

				const int numProjectiles = 24;
				const float angleStep = 15.0f * (3.1415926f / 180.0f);
				const float radius = 1.0f;

				float angleOffset = angleOffsetToggle ? (7.5f * (3.1415926f / 180.0f)) : 0.0f;

				for (int i = 0; i < numProjectiles; ++i)
				{
					float angle = i * angleStep + angleOffset;
					AEVec2 dir;
					dir.x = cosf(angle) * radius;
					dir.y = sinf(angle) * radius;

					this->SpawnProjectile(dir, position);
				}

				Manager::SFXManager.Play("BossAttackMagic");
				angleOffsetToggle = !angleOffsetToggle;
			}

			if (WaveAttackSpawnTimer > WaveAttackChamberTimer)
			{
				bIsWaving = true;
				if (WaveAttackSpawnTimer > WaveAttackChamberTimer * 2)
				{
					bIsWaving = false;
					WaveAttackSpawnTimer = 0;
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_int_distribution<> dist(0, 4);
					int randIndex = dist(gen);
					switch (randIndex)
					{
					case 0:
						AEVec2Set(&position, 0.f, 0.f);
						break;
					case 1:
						AEVec2Set(&position, 800.f, 450.f);
						break;
					case 2:
						AEVec2Set(&position, -800.f, 450.f);
						break;
					case 3:
						AEVec2Set(&position, 800.f, -450.f);
						break;
					case 4:
						AEVec2Set(&position, -800.f, -450.f);
						break;
					}
				}
			}
			if (bIsWaving)
			{
				const int bulletsPerWave = 40;
				const float angleRangeDeg = 30.0f;
				const float angleRangeRad = angleRangeDeg * (3.1415926f / 180.0f);
				const float frequency = 10.0f;
				const float intervalTime = 0.03f;
				const float waveSpeed = 8.0f;

				static float waveTimer = 0.0f;
				static int bulletIndex = 0;
				static float wavePhase = 0.0f;

				waveTimer += global::DeltaTime;
				wavePhase += global::DeltaTime * waveSpeed;

				if (bulletIndex < bulletsPerWave && waveTimer > intervalTime)
				{
					AEVec2 baseDir;
					AEVec2Sub(&baseDir, &global::PlayerLocation, &position);
					AEVec2Normalize(&baseDir, &baseDir);

					int centeredIndex = bulletIndex - (bulletsPerWave / 2);

					float waveAngle = sinf(wavePhase + centeredIndex / frequency) * angleRangeRad;

					float cosA = cosf(waveAngle);
					float sinA = sinf(waveAngle);

					AEVec2 rotatedDir;
					rotatedDir.x = baseDir.x * cosA - baseDir.y * sinA;
					rotatedDir.y = baseDir.x * sinA + baseDir.y * cosA;

					AEVec2Neg(&rotatedDir, &rotatedDir);

					this->SpawnProjectile(rotatedDir, position);

					bulletIndex++;
					waveTimer = 0.0f;
				}
				if (bulletIndex >= bulletsPerWave)
				{
					bulletIndex = 0;
					waveTimer = 0.0f;
				}
			}
		}
	}
	void Stage1Boss::Draw()
	{
		EnemyBoss::Draw();
	}
	void Stage1Boss::Destroy()
	{
		EnemyBoss::Destroy();
	}
	void Stage2Boss::Init()
	{
		EnemyBoss::Init();

		name = "Valoran";
		epithet = "the Crusader";//or Knight-Commander
		Stats->MovementSpeed = 1200.f;
		size = { 300.f, 300.f };
		position = { 0.f, 0.f };
		CollisionRadius = 150.f;

		Stats->HP = 900;
		Stats->MaxHP = 900;
		Stats->Damage = 1;

		dashTimer = 0.f;
		dashSpeed = 500.f;
		cooldownTime = 0.f;

		AEVec2Set(&Stats->ProjectileSize, 30, 30);
		Stats->ProjectileSpeed = 10.f;
		Stats->ProjectileCollisionSize = 15.f;
		AnimationState = IDLE;
		MaxAnimationCount[IDLE] = 2;
		MaxAnimationCount[MOVE] = 2;
		MaxAnimationCount[JUMP] = 2;
		MaxAnimationCount[ATTACK] = 2;
	}
	void Stage2Boss::Update()
	{
		EnemyBoss::Update();
		if (!bIsJumping)
		{
			if (!bIsCharging)
			{
				cooldownTime += global::DeltaTime;

				if (cooldownTime > 2.f)
				{
					AEVec2Sub(&dashDirection, &global::PlayerLocation, &position);
					AEVec2Normalize(&dashDirection, &dashDirection);
					AnimationState = MOVE;
					bIsCharging = true;
				}
				else if (cooldownTime > 1.f)
				{
					if (!bDoRadialAttack)
					{
						AEVec2 toPlayer;
						AEVec2Sub(&toPlayer, &global::PlayerLocation, &position);
						AEVec2Normalize(&toPlayer, &toPlayer);

						const int numBullets = 5;
						const float angleSpreadDeg = 30.f;
						const float angleStepDeg = angleSpreadDeg / (numBullets - 1);
						const float angleStartDeg = -angleSpreadDeg / 2.0f;

						for (int i = 0; i < numBullets; ++i)
						{
							float angleDeg = angleStartDeg + i * angleStepDeg;
							float angleRad = angleDeg * (3.1415926f / 180.0f);

							float cosA = cosf(angleRad);
							float sinA = sinf(angleRad);

							AEVec2 rotatedDir;
							rotatedDir.x = toPlayer.x * cosA - toPlayer.y * sinA;
							rotatedDir.y = toPlayer.x * sinA + toPlayer.y * cosA;
							AEVec2Neg(&rotatedDir, &rotatedDir);
							SpawnProjectile(rotatedDir, position);
						}
						Manager::SFXManager.Play("BossAttackKnife");
						bDoRadialAttack = true;
					}
				}
			}
			else
			{
				AEVec2 delta;
				AEVec2Scale(&delta, &dashDirection, Stats->MovementSpeed * global::DeltaTime);

				afterImageTimer += global::DeltaTime;
				if (afterImageTimer >= afterImageCooldown)
				{
					AfterImage img;
					img.position = position;
					img.size = size;
					img.alpha = DrawAlpha;
					img.timer = 0.3f;

					afterImages.push_back(img);
					afterImageTimer = 0.f;
				}

				if (dashDirection.x < 0.0f && size.x > 0.0f)
				{
					size.x *= -1.0f;
				}
				else if (dashDirection.x > 0.0f && size.x < 0.0f)
				{
					size.x *= -1.0f;
				}

				AEVec2 newPos = { position.x + delta.x, position.y + delta.y };
				float EllipseA = (global::worldMax.x - global::worldMin.x) / 2 - (abs(size.x) / 2);
				float EllipseB = (global::worldMax.y - global::worldMin.y) / 2 - (size.y / 2);
				float value = (newPos.x * newPos.x) / (EllipseA * EllipseA) + (newPos.y * newPos.y) / (EllipseB * EllipseB);
				if (value <= 1.0f)
				{
					position = newPos;
				}
				else
				{
					bDoRadialAttack = false;
					bIsCharging = false;
					cooldownTime = 0.f;
					dashCount++;
					AnimationState = IDLE;
					if (dashCount >= 3)
					{
						AnimationState = JUMP;
						bIsJumping = true;
						jumpTimer = 0.f;
						jumpStartPos = position;
						jumpTargetPos = global::PlayerLocation;
						float EllipseA = (global::worldMax.x - global::worldMin.x) / 2 - (abs(size.x) / 2);
						float EllipseB = (global::worldMax.y - global::worldMin.y) / 2 - (size.y / 2);
						float value = (jumpTargetPos.x * jumpTargetPos.x) / (EllipseA * EllipseA) +
							(jumpTargetPos.y * jumpTargetPos.y) / (EllipseB * EllipseB);

						if (value > 1.0f)
						{
							AEVec2 dir;
							AEVec2 Orient = { 0.f, 0.f };
							AEVec2Sub(&dir, &jumpTargetPos, &Orient); 
							AEVec2Normalize(&dir, &dir);

							jumpTargetPos.x = dir.x * EllipseA;
							jumpTargetPos.y = dir.y * EllipseB;
						}
						dashCount = 0;
						if (Manager::gm.currStateREF)
						{
							Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
							if (GS)
							{
								GS->VFXManager.AddWarningVFX(VFXType::WarningCircle, size, jumpMoveDuration,jumpTargetPos, false, this);
								bIsWarned = true;
							}
						}
					}
				}
			}
		}
		else
		{
			jumpTimer += global::DeltaTime;

			float fadeOutRatio = std::fmin(jumpTimer / jumpFadeOutDuration, 1.0f);
			float moveRatio = std::fmin(jumpTimer / jumpMoveDuration, 1.0f);
			float fadeInRatio = std::fmax((jumpTimer - (jumpMoveDuration - jumpFadeInDuration)) / jumpFadeInDuration, 0.0f);

			if (jumpTimer < jumpMoveDuration - jumpFadeInDuration)
			{
				DrawAlpha = 1.0f - 0.7f * fadeOutRatio;
			}
			else
			{
				AnimationState = ATTACK;
				DrawAlpha = 0.3f + 0.7f * fadeInRatio; 
			}

			position.x = jumpStartPos.x + (jumpTargetPos.x - jumpStartPos.x) * moveRatio;
			position.y = jumpStartPos.y + (jumpTargetPos.y - jumpStartPos.y) * moveRatio;

			if (jumpTimer >= jumpMoveDuration)
			{
				AnimationState = IDLE;
				DrawAlpha = 1.0f;
				bIsJumping = false;

				AEVec2 toPlayer;
				AEVec2Sub(&toPlayer, &global::PlayerLocation, &position);
				float distance = AEVec2Length(&toPlayer);
			}
		}

		for (auto it = afterImages.begin(); it != afterImages.end(); )
		{
			it->timer -= global::DeltaTime;
			it->alpha -= global::DeltaTime / 0.3f;

			if (it->timer <= 0 || it->alpha <= 0)
				it = afterImages.erase(it);
			else
				++it;
		}
	}
	void Stage2Boss::Draw()
	{
		for (const auto& img : afterImages)
		{
			Utils::DrawObject(img.position, offset, img.size, Texture, Mesh, img.alpha);
		}

		Utils::DrawObject(*this, true, DrawAlpha);
	}
	void Stage2Boss::Destroy()
	{
		EnemyBoss::Destroy();
	}
	void Stage2Boss::OnPlayerHit()
	{
		EnemyBoss::OnPlayerHit();
	}

	void Stage3Boss::Init()
	{
		EnemyBoss::Init();
		name = "Seraphiel";
		epithet = "the Luminary";//or Radiant
		size = { 300.f, 300.f };
		position = { 0.f, 0.f };
		CollisionRadius = 150.f;

		Stats->HP = 1600;
		Stats->MaxHP = 1600;
		Stats->Damage = 1;
		Stats->MovementSpeed = 100.f;
		AnimationState = IDLE;
		MaxAnimationCount[IDLE] = 2;

		AEVec2Set(&Stats->ProjectileSize, 30, 30);
		ProjectileSpawnTimer = 0;
		ProjectileChamberTimer = 1.5f;
		bIsWaving = false;
		bLaserWarned = false;
		Stats->ProjectileSpeed = 3.f;
		Stats->Damage = 1;
		Stats->ProjectileCollisionSize = 15.f;

		MovePositions = {
		{ 0.f, 0.f },
		{ 800.f, 450.f },
		{ -800.f, 450.f },
		{ 800.f, -450.f },
		{ -800.f, -450.f }
		};
	}
	void Stage3Boss::Update()
	{
		EnemyBoss::Update();

		MoveTimer += global::DeltaTime;

		if (!bIsMoving && MoveTimer >= MoveCooldown)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(0, static_cast<int>(MovePositions.size()) - 1);

			int index;
			do 
			{
				index = dist(gen);
			} while (MovePositions[index].x == position.x && MovePositions[index].y == position.y);

			MoveStartPos = position;
			MoveTargetPos = MovePositions[index];
			bIsMoving = true;
			MoveTimer = 0.f;
		}

		if (bIsMoving)
		{
			AEVec2 toTarget;
			AEVec2Sub(&toTarget, &MoveTargetPos, &position);
			float distance = AEVec2Length(&toTarget);

			if (distance < 5.f)
			{
				position = MoveTargetPos;
				bIsMoving = false;
			}
			else
			{
				AEVec2 dir;
				AEVec2Normalize(&dir, &toTarget);

				AEVec2 delta;
				AEVec2Scale(&delta, &dir, Stats->MovementSpeed * global::DeltaTime);
				AEVec2Add(&position, &position, &delta);
			}
		}

		BombTimer += global::DeltaTime;
		if (BombTimer >= BombSpawnInterval)
		{
			BombTimer = 0.f;
			std::random_device rd;
			std::mt19937 gen(rd());
			
			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					for (int i = 0;i < 3;i++)
					{
						std::uniform_real_distribution<float> distX(global::worldMin.x, global::worldMax.x);
						std::uniform_real_distribution<float> distY(global::worldMin.y, global::worldMax.y);

						AEVec2 bombPos = { distX(gen), distY(gen) };

						AEVec2 WarningDraw = { BombRadius*2.f, BombRadius*2.f };
						GS->VFXManager.AddWarningVFX(VFXType::WarningCircle, WarningDraw, BombDelay, bombPos, false, this);
						SpawnBomb(bombPos);
					}
				}
			}
		}

		BlackholeTimer += global::DeltaTime;
		if (BlackholeTimer >= BlackholeInterval)
		{
			BlackholeTimer = 0.f;

			SpawnBlackHole(BlackholePosition);
			bBlackHoleWarned = false;
		}
		else if (BlackholeTimer > BlackholeInterval - 1.f)
		{
			if (!bBlackHoleWarned)
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<float> distX(global::worldMin.x + 200.f, global::worldMax.x - 200.f);
				std::uniform_real_distribution<float> distY(global::worldMin.y + 200.f, global::worldMax.y - 200.f);

				BlackholePosition = { distX(gen), distY(gen) };
				bBlackHoleWarned = true;
				if (Manager::gm.currStateREF)
				{
					Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
					if (GS)
					{
						AEVec2 WarningDraw = { BlackholeRadius, BlackholeRadius };
						GS->VFXManager.AddWarningVFX(VFXType::WarningCircle, WarningDraw, 1.f, BlackholePosition, false, this);
						bBlackHoleWarned = true;
					}
				}
			}
		}

		LaserTimer += global::DeltaTime;
		if (LaserTimer >= LaserSpawnMaxTime)
		{
			bLaserWarned = false;
			std::vector<AEVec2> directions;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::bernoulli_distribution dist2(0.5);
			std::bernoulli_distribution dist3(0.5);

			
			bLaserRotRight = dist2(gen);
			bUseRotate = dist3(gen);
			if (bUseEightDirections)
			{
				directions = {
					{ 1.f,  0.f },
					{ -1.f, 0.f },
					{ 0.f,  1.f },
					{ 0.f, -1.f },
					{ 0.707f,  0.707f },
					{ -0.707f, 0.707f },
					{ 0.707f, -0.707f },
					{ -0.707f, -0.707f },
				};
			}
			else
			{
				directions = {
					{ 1.f,  0.f },
					{ -1.f, 0.f },
					{ 0.f,  1.f },
					{ 0.f, -1.f }
				};
			}

			for (const AEVec2& dir : directions)
			{
				AEVec2 normDir = dir;
				AEVec2Normalize(&normDir, &normDir);

				LaserAttack* laser = new LaserAttack;
				laser->Init(position, normDir, this, 3200.f, (bUseRotate ? 10.f : 5.f), (bUseRotate ? 0.2f : 0.f), bLaserRotRight);

				if (Manager::gm.currStateREF)
				{
					Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
					if (GS)
					{
						GS->EAAs.push_back(laser);
					}
				}
			}
			LaserTimer = 0.f;
			Manager::SFXManager.Play("LaserFire");
		}
		else if (LaserTimer > LaserSpawnMaxTime - 1.f)
		{
			if (!bLaserWarned)
			{
				bLaserWarned = true;
				std::vector<AEVec2> directions;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::bernoulli_distribution dist1(0.5);
				bUseEightDirections = dist1(gen);
				if (bUseEightDirections)
				{
					directions = {
						{ 1.f,  0.f },
						{ -1.f, 0.f },
						{ 0.f,  1.f },
						{ 0.f, -1.f },
						{ 0.707f,  0.707f },
						{ -0.707f, 0.707f },
						{ 0.707f, -0.707f },
						{ -0.707f, -0.707f },
					};
				}
				else
				{
					directions = {
						{ 1.f,  0.f },
						{ -1.f, 0.f },
						{ 0.f,  1.f },
						{ 0.f, -1.f }
					};
				}
				if (Manager::gm.currStateREF)
				{
					Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
					if (GS)
					{
						AEVec2 WarningDraw = { 3200.f, 30.f };
						for (AEVec2 dir : directions)
						{
							GS->VFXManager.AddWarningVFX(VFXType::WarningSquare, position, WarningDraw, dir,false, this, 1.f);
						}
						bIsWarned = true;
					}
				}
				Manager::SFXManager.Play("LaserWarning");
			}
		}
		ProjectileSpawnTimer += global::DeltaTime;
		WaveAttackSpawnTimer += global::DeltaTime;

		if (ProjectileSpawnTimer > ProjectileChamberTimer)
		{
			ProjectileSpawnTimer = 0;

			const int numProjectiles = 24;
			const float angleStep = 15.0f * (3.1415926f / 180.0f);
			const float radius = 1.0f;

			float angleOffset = angleOffsetToggle ? (7.5f * (3.1415926f / 180.0f)) : 0.0f;

			for (int i = 0; i < numProjectiles; ++i)
			{
				float angle = i * angleStep + angleOffset;
				AEVec2 dir;
				dir.x = cosf(angle) * radius;
				dir.y = sinf(angle) * radius;

				this->SpawnProjectile(dir, position);
			}

			angleOffsetToggle = !angleOffsetToggle;
			Manager::SFXManager.Play("BossAttackKnife");
		}

		if (WaveAttackSpawnTimer > WaveAttackChamberTimer)
		{
			bIsWaving = true;
			if (WaveAttackSpawnTimer > WaveAttackChamberTimer * 2)
			{
				bIsWaving = false;
				WaveAttackSpawnTimer = 0;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dist(0, 4);
				int randIndex = dist(gen);
				switch (randIndex)
				{
				case 0:
					AEVec2Set(&position, 0.f, 0.f);
					break;
				case 1:
					AEVec2Set(&position, 800.f, 450.f);
					break;
				case 2:
					AEVec2Set(&position, -800.f, 450.f);
					break;
				case 3:
					AEVec2Set(&position, 800.f, -450.f);
					break;
				case 4:
					AEVec2Set(&position, -800.f, -450.f);
					break;
				}
			}
		}
		if (bIsWaving)
		{
			const int bulletsPerWave = 40;
			const float angleRangeDeg = 30.0f;
			const float angleRangeRad = angleRangeDeg * (3.1415926f / 180.0f);
			const float frequency = 10.0f;
			const float intervalTime = 0.03f;
			const float waveSpeed = 8.0f;

			static float waveTimer = 0.0f;
			static int bulletIndex = 0;
			static float wavePhase = 0.0f;

			waveTimer += global::DeltaTime;
			wavePhase += global::DeltaTime * waveSpeed;

			if (bulletIndex < bulletsPerWave && waveTimer > intervalTime)
			{
				AEVec2 baseDir;
				AEVec2Sub(&baseDir, &global::PlayerLocation, &position);
				AEVec2Normalize(&baseDir, &baseDir);

				int centeredIndex = bulletIndex - (bulletsPerWave / 2);

				float waveAngle = sinf(wavePhase + centeredIndex / frequency) * angleRangeRad;

				float cosA = cosf(waveAngle);
				float sinA = sinf(waveAngle);

				AEVec2 rotatedDir;
				rotatedDir.x = baseDir.x * cosA - baseDir.y * sinA;
				rotatedDir.y = baseDir.x * sinA + baseDir.y * cosA;

				AEVec2Neg(&rotatedDir, &rotatedDir);

				this->SpawnProjectile(rotatedDir, position);

				bulletIndex++;
				waveTimer = 0.0f;
			}
			if (bulletIndex >= bulletsPerWave)
			{
				bulletIndex = 0;
				waveTimer = 0.0f;
			}
		}
	}
	void Stage3Boss::Draw()
	{
		Utils::DrawObject(*this);
	}
	void Stage3Boss::Destroy()
	{
		EnemyBoss::Destroy();
	}
	void Stage3Boss::OnPlayerHit()
	{
		EnemyBoss::OnPlayerHit();
	}

	void Stage3Boss::SpawnBomb(AEVec2& pos)
	{
		BombAttack* NewBomb = new BombAttack;
		NewBomb->Init(pos, BombRadius, BombDelay);
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				GS->EAAs.push_back(NewBomb);
			}
		}
	}
	void Stage3Boss::SpawnBlackHole(AEVec2& pos)
	{
		BlackholeAttack* NewBlackhole = new BlackholeAttack;
		AEVec2 DrawSize;
		AEVec2Set(&DrawSize, BlackholeRadius, BlackholeRadius);
		NewBlackhole->Init(pos, DrawSize, BlackholeRadius, BlackholeDuration,this,true);
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				GS->EAAs.push_back(NewBlackhole);
			}
		}
	}
}