#include "EnemyBoss.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include <random>
#include <cmath>
namespace InGame
{
	void EnemyBoss::Init()
	{
		Mesh = Utils::CreateMesh();
		bIsPandingKill = false;
	}
	void EnemyBoss::Update()
	{

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
	}
	void EnemyBoss::OnPlayerHit()
	{
	}
	void Stage1Boss::Init()
	{
		EnemyBoss::Init();
		Stats.MovementSpeed = 100.f;
		size.x = 400;
		size.y = 400;
		position.x = 0;
		position.y = 0;
		CollisionRadius = 200;
		AEVec2Set(&Stats.ProjectileSize, 30, 30);
		ProjectileSpawnTimer = 0;
		ProjectileChamberTimer = 5;
		bIsWaving = false;

		Stats.HP = 10;
		Stats.FireRate = 5.f;
		Stats.ProjectileSpeed = 3.f;
		Stats.Damage = 1;
		Stats.ProjectileCollisionSize = 15.f;
	}
	void Stage1Boss::Update()
	{
		UpdateEffectTime();

		static bool angleOffsetToggle = false;

		if (Stats.StatusEffectTimer[BURN] > 0)
		{
			BurnTimer += global::DeltaTime;
			if (BurnTimer >= global::effectiveBurnRate)
			{
				BurnTimer = 0.f;
				adjustHealth(-Stats.MaxHP * global::effectiveBurnDamage / 5);
			}
		}

		if (!(Stats.StatusEffectTimer[STUN] > 0))
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

		Stats.MovementSpeed = 1200.f;
		size = { 300.f, 300.f };
		position = { 0.f, 0.f };
		CollisionRadius = 150.f;

		Stats.HP = 40;
		Stats.Damage = 1;

		dashTimer = 0.f;
		dashSpeed = 500.f;
		cooldownTime = 0.f;

		AEVec2Set(&Stats.ProjectileSize, 30, 30);
		Stats.ProjectileSpeed = 10.f;
		Stats.ProjectileCollisionSize = 15.f;
	}
	void Stage2Boss::Update()
	{
		if (!bIsJumping)
		{
			if (!bIsCharging)
			{
				cooldownTime += global::DeltaTime;

				if (cooldownTime > 2.f)
				{
					AEVec2Sub(&dashDirection, &global::PlayerLocation, &position);
					AEVec2Normalize(&dashDirection, &dashDirection);
					bIsCharging = true;
				}
			}
			else
			{
				AEVec2 delta;
				AEVec2Scale(&delta, &dashDirection, Stats.MovementSpeed * global::DeltaTime);

				AEVec2 newPos = { position.x + delta.x, position.y + delta.y };
				float EllipseA = (global::worldMax.x - global::worldMin.x) / 2 - (size.x / 2);
				float EllipseB = (global::worldMax.y - global::worldMin.y) / 2 - (size.y / 2);
				float value = (newPos.x * newPos.x) / (EllipseA * EllipseA) + (newPos.y * newPos.y) / (EllipseB * EllipseB);
				if (value <= 1.0f)
				{
					position = newPos;
				}
				else
				{
					bIsCharging = false;
					cooldownTime = 0.f;
					dashCount++;
					if (dashCount >= 3)
					{
						bIsJumping = true;
						jumpTimer = 0.f;
						jumpStartPos = position;
						jumpTargetPos = global::PlayerLocation;
						dashCount = 0;
					}
				}
			}

			radialAttackTimer += global::DeltaTime;
			if (radialAttackTimer >= radialAttackCooldown)
			{
				radialAttackTimer = 0.f;

				AEVec2 toPlayer;
				AEVec2Sub(&toPlayer, &global::PlayerLocation, &position);
				AEVec2Normalize(&toPlayer, &toPlayer);

				const int numBullets = 5;
				const float angleSpreadDeg = 30.f; // ±15도
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
			}
		}
		else
		{
			jumpTimer += global::DeltaTime;

			// 비율 계산 (0~1)
			float fadeOutRatio = std::fmin(jumpTimer / jumpFadeOutDuration, 1.0f);
			float moveRatio = std::fmin(jumpTimer / jumpMoveDuration, 1.0f);
			float fadeInRatio = std::fmax((jumpTimer - (jumpMoveDuration - jumpFadeInDuration)) / jumpFadeInDuration, 0.0f);

			// Alpha 처리
			if (jumpTimer < jumpMoveDuration - jumpFadeInDuration)
			{
				// 점점 희미해짐
				DrawAlpha = 1.0f - 0.7f * fadeOutRatio; // 1.0 → 0.3
			}
			else
			{
				// 다시 뚜렷하게
				DrawAlpha = 0.3f + 0.7f * fadeInRatio; // 0.3 → 1.0
			}

			// 위치 이동: 점프 시작점 → 착지 위치 (보간)
			position.x = jumpStartPos.x + (jumpTargetPos.x - jumpStartPos.x) * moveRatio;
			position.y = jumpStartPos.y + (jumpTargetPos.y - jumpStartPos.y) * moveRatio;

			// 낙하 완료 처리
			if (jumpTimer >= jumpMoveDuration)
			{
				DrawAlpha = 1.0f;
				bIsJumping = false;

				// 낙하 공격 판정
				AEVec2 toPlayer;
				AEVec2Sub(&toPlayer, &global::PlayerLocation, &position);
				float distance = AEVec2Length(&toPlayer);
			}
		}
	}
	void Stage2Boss::Draw()
	{
		Utils::DrawObject(*this,true,DrawAlpha);
	}
	void Stage2Boss::Destroy()
	{
		EnemyBoss::Destroy();
	}
	void Stage2Boss::OnPlayerHit()
	{
		EnemyBoss::OnPlayerHit();
	}
}