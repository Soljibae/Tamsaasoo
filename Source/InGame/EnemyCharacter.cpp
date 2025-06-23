#include "EnemyCharacter.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "../Manager/GameManager.h"
#include "../Manager/Playing.h"
namespace InGame
{
	void InGame::EnemyCharacter::Init()
	{
		Mesh = Utils::CreateMesh();
		MovementSpeed = 100.f;
		size.x = 40;
		size.y = 40;
		CollisionRadius = 20;

		Stats.HP = 1;
		Stats.FireRate = 1.f;
		Stats.BulletSpeed = 30.f;
		Stats.Damage = 1;

		Stats.Init();

		bHasDashed = false;
		dashDuration = 0.f;
		recoverTimer = 0.f;
		dashDirection = { 0, 0 };
	}

	void EnemyCharacter::Spawn(AEVec2 Pos, EnemyData* InData)
	{
		if (InData->Texture == nullptr)
		{
			InData->Init();
		}
		Type = InData->Type;
		Texture = InData->Texture;
		Stats.Damage = InData->Damage;
		Exp = InData->Exp;
		Stats.MovementSpeed = InData->MovementSpeed;
		size = InData->DrawSize;
		CollisionRadius = InData->CollisionRadius;
		position = Pos;

		bHasDashed = false;
		dashDuration = 0.f;
		recoverTimer = 0.f;
		dashDirection = direction;
		dashStartPos = position;
	}

	void InGame::EnemyCharacter::Update()
	{
		UpdateEffectTime();
		
		if (!(Stats.StatusEffectTimer[STUN] > 0))
		{
			f32 len = AEVec2Distance(&global::PlayerLocation, &position);
			f32 dx = position.x - global::PlayerLocation.x;
			f32 dy = position.y - global::PlayerLocation.y;
			direction.x = dx / len;
			if ((direction.x > 0 && size.x > 0) || (direction.x < 0 && size.x < 0))
			{
				size.x *= -1;
			}
			direction.y = dy / len;


			switch (Type)
			{
			case EnemyType::MINION:
				position.x -= direction.x * Stats.MovementSpeed * global::DeltaTime;
				position.y -= direction.y * Stats.MovementSpeed * global::DeltaTime;
				break;
			case EnemyType::ARCHER:
				ProjectileSpawnTimer += global::DeltaTime;
				if (len > 500)
				{
					position.x -= direction.x * MovementSpeed * global::DeltaTime;
					position.y -= direction.y * MovementSpeed * global::DeltaTime;
				}
				else
				{
					if (ProjectileSpawnTimer > ProjectileChamberTimer)
					{
						ProjectileSpawnTimer = 0;
						SpawnProjectile(direction, position);
					}
				}
				break;
			case EnemyType::DASHER:
				static const float dashSpeed = 400.f;
				static const float walkSpeed = 80.f;
				static const float dashTriggerDistance = 300.f;
				static const float recoveryTime = 1.0f;
				static const float dashRange = 400.f;

				f32 len = AEVec2Distance(&global::PlayerLocation, &position);

				if (!bHasDashed)
				{
					if (len <= dashTriggerDistance)
					{
						if (!bIsDashing)
						{
							// 돌진 시작
							dashDirection = direction;
							dashStartPos = position;
							bIsDashing = true;
						}

						// 현재 위치에서 얼마나 이동했는지 측정
						AEVec2 moved;
						AEVec2Sub(&moved, &position, &dashStartPos);
						float dashMoved = AEVec2Length(&moved);

						if (dashMoved < dashRange)
						{
							// 계속 돌진
							position.x -= dashDirection.x * dashSpeed * global::DeltaTime;
							position.y -= dashDirection.y * dashSpeed * global::DeltaTime;
						}
						else
						{
							// 돌진 완료
							bIsDashing = false;
							bHasDashed = true;
							recoverTimer = 0.f;
						}
					}
					else
					{
						// 사정거리 밖 → 천천히 추적
						position.x -= direction.x * walkSpeed * global::DeltaTime;
						position.y -= direction.y * walkSpeed * global::DeltaTime;
					}
				}
				else
				{
					recoverTimer += global::DeltaTime;
					if (recoverTimer >= recoveryTime)
					{
						dashDuration = 0.f;
						bHasDashed = false;
					}
				}
				break;
			}
		}
		
	}
	void EnemyCharacter::Draw()
	{
		Utils::DrawObject(*this);
	}
	void EnemyCharacter::Destroy()
	{
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
			Mesh = nullptr;
		}
		Texture = nullptr;
	}
	void EnemyCharacter::adjustHealth(s32 Amount)
	{
		Stats.HP += Amount;
		if (Stats.HP <= 0)
		{
			bIsPandingKill = true;
		}
	}
	void EnemyCharacter::SpawnProjectile(AEVec2 Dir, AEVec2 Pos)
	{
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (GS->EPPool.size() > 0)
				{
					Dir.x *= -1;
					Dir.y *= -1;
					Projectile* EP = GS->EPPool.back();
					GS->EPPool.pop_back();
					EP->Spawn(Dir, Pos, this);
					GS->EPs.push_back(EP);
				}

			}
		}
	}
	void EnemyCharacter::UpdateEffectTime()
	{
		for (size_t i = 0; i < Stats.StatusEffectTimer.size(); i++)
		{
			switch (i)
			{
			case 0:
				if (Stats.StatusEffectTimer[BURN] > 0)
					Stats.StatusEffectTimer[BURN] -= global::DeltaTime;
				if (Stats.StatusEffectTimer[BURN] < 0)
					Stats.StatusEffectTimer[BURN] = 0;
			case 1:
				if (Stats.StatusEffectTimer[STUN] > 0)
					Stats.StatusEffectTimer[STUN] -= global::DeltaTime;
				if (Stats.StatusEffectTimer[STUN] < 0)
					Stats.StatusEffectTimer[STUN] = 0;
			case 2:
				if (Stats.StatusEffectTimer[SLOW] > 0)
					Stats.StatusEffectTimer[SLOW] -= global::DeltaTime;
				if (Stats.StatusEffectTimer[SLOW] < 0)
					Stats.StatusEffectTimer[SLOW] = 0;
			case 3:
				if (Stats.StatusEffectTimer[FEAR] > 0)
					Stats.StatusEffectTimer[FEAR] -= global::DeltaTime;
				if (Stats.StatusEffectTimer[FEAR] < 0)
					Stats.StatusEffectTimer[FEAR] = 0;
			case 4:
				if (Stats.StatusEffectTimer[VULNERABLE] > 0)
					Stats.StatusEffectTimer[VULNERABLE] -= global::DeltaTime;
				if (Stats.StatusEffectTimer[VULNERABLE] < 0)
					Stats.StatusEffectTimer[VULNERABLE] = 0;
			default:
				break;
			}
		}
	}
}
