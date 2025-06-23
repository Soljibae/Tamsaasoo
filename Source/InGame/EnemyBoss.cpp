#include "EnemyBoss.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include <random>
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
	void Stage1Boss::Init()
	{
		EnemyBoss::Init();
		MovementSpeed = 100.f;
		size.x = 400;
		size.y = 400;
		position.x = 0;
		position.y = 0;
		CollisionRadius = 200;

		ProjectileSpawnTimer = 0;
		ProjectileChamberTimer = 5;
		bIsWaving = false;

		Stats.HP = 10;
		Stats.FireRate = 5.f;
		Stats.BulletSpeed = 3.f;
		Stats.Damage = 1;
	}
	void Stage1Boss::Update()
	{
		static bool angleOffsetToggle = false;
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
			if (WaveAttackSpawnTimer > WaveAttackChamberTimer*2)
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
	void Stage1Boss::Draw()
	{
		EnemyBoss::Draw();
	}
	void Stage1Boss::Destroy()
	{
		EnemyBoss::Destroy();
	}
}