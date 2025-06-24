#include "EnemyCharacter.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "../Manager/GameManager.h"
#include "../Manager/Playing.h"
#include <algorithm>
namespace InGame
{
	void InGame::EnemyCharacter::Init()
	{
		row = 4;
		column = 2;
		FrameTime = 0.2f;
		AnimationState = IDLE;
		AnimationCount = 0;
		MaxAnimationCount[IDLE] = 2;
		MaxAnimationCount[MOVE] = 2;
		Mesh = Utils::CreateMesh(row,column);
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
		Stats.BulletSpeed = InData->BulletSpeed;
		size = InData->DrawSize;
		CollisionRadius = InData->CollisionRadius;
		position = Pos;
		Stats.MaxHP = InData->Health;
		Stats.HP = InData->Health;

		AEVec2Set(&ProjectileSize, InData->ProjectileSize.x, InData->ProjectileSize.y);

		/*--------DASHER--------*/
		bHasDashed = false;
		bIsDashing = false;
		recoverTimer = 0.f;
		dashDuration = 0.f;
		dashDirection = direction;
		dashStartPos = position;
		/*--------DASHER--------*/

		/*--------BOMBER--------*/
		isDetonating = false;
		detonationTimer = 0.f;
		detonationDelay = 2.0f;
		explosionRadius = 150.f;
		explosionDamage = 4;
		/*--------BOMBER--------*/
	}

	void InGame::EnemyCharacter::Update()
	{
		UpdateEffectTime();
		Utils::UpdateOffset(*this);
		if (Stats.StatusEffectTimer[SLOW] > 0)
		{
			FrameTime = 0.6f;
		}
		else
		{
			FrameTime = 0.2f;
		}
		if (Stats.StatusEffectTimer[BURN] > 0)
		{
			BurnTimer += global::DeltaTime;
			if (BurnTimer >= global::effectiveBurnRate)
			{
				BurnTimer = 0.f;
				adjustHealth(-Stats.MaxHP * global::effectiveBurnDamage);
			}
		}

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
			
			if (Stats.StatusEffectTimer[FEAR] > 0)
				AEVec2Scale(&direction, &direction, -1.f);

			switch (Type)
			{
				case EnemyType::MINION:
				{
					f32 effectiveMovementSpeed = Stats.MovementSpeed;

					if (Stats.StatusEffectTimer[SLOW] > 0)
					{
						effectiveMovementSpeed *= 0.5f;
					}

					position.x -= direction.x * effectiveMovementSpeed * global::DeltaTime;
					position.y -= direction.y * effectiveMovementSpeed * global::DeltaTime;
					break;
				}
				case EnemyType::ARCHER:
				{
					ProjectileSpawnTimer += global::DeltaTime;
					if (len > 500)
					{
						f32 effectiveMovementSpeed = Stats.MovementSpeed;

						if (Stats.StatusEffectTimer[SLOW] > 0)
						{
							effectiveMovementSpeed *= 0.5f;
						}

						position.x -= direction.x * effectiveMovementSpeed * global::DeltaTime;
						position.y -= direction.y * effectiveMovementSpeed * global::DeltaTime;
					}
					else
					{
						f32 effectiveChamberTimer = ProjectileChamberTimer;

						if (Stats.StatusEffectTimer[SLOW] > 0)
						{
							effectiveChamberTimer *= 1.5f;
						}

						if (ProjectileSpawnTimer > effectiveChamberTimer)
						{
							ProjectileSpawnTimer = 0;
							SpawnProjectile(direction, position);
						}
					}
					break;
				}
				case EnemyType::DASHER:
				{
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
								f32 effectiveDashSpeed = dashSpeed;

								if (Stats.StatusEffectTimer[SLOW] > 0)
								{
									effectiveDashSpeed *= 0.5f;
								}
							
								position.x -= dashDirection.x * effectiveDashSpeed * global::DeltaTime;
								position.y -= dashDirection.y * effectiveDashSpeed * global::DeltaTime;
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
							f32 effectiveMovementSpeed = walkSpeed;

							if (Stats.StatusEffectTimer[SLOW] > 0)
							{
								effectiveMovementSpeed *= 0.5f;
							}

							position.x -= direction.x * effectiveMovementSpeed * global::DeltaTime;
							position.y -= direction.y * effectiveMovementSpeed * global::DeltaTime;
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
				case EnemyType::TANKER:
				{
					f32 effectiveMovementSpeed = Stats.MovementSpeed;

					if (Stats.StatusEffectTimer[SLOW] > 0)
					{
						effectiveMovementSpeed *= 0.5f;
					}

					position.x -= direction.x * effectiveMovementSpeed * global::DeltaTime;
					position.y -= direction.y * effectiveMovementSpeed * global::DeltaTime;
					break;
				}
				case EnemyType::BOMBER:
				{
					if (!isDetonating)
					{
						if (len <= 200.f)
						{
							isDetonating = true;
							detonationTimer = 0.f;
						}
						else
						{
							f32 effectiveMovementSpeed = Stats.MovementSpeed;

							if (Stats.StatusEffectTimer[SLOW] > 0)
							{
								effectiveMovementSpeed *= 0.5f;
							}

							position.x -= direction.x * effectiveMovementSpeed * global::DeltaTime;
							position.y -= direction.y * effectiveMovementSpeed * global::DeltaTime;
						}
					}
					else
					{
						f32 effectiveDetonationDelay = detonationDelay;

						if (Stats.StatusEffectTimer[SLOW] > 0)
						{
							effectiveDetonationDelay *= 1.5f;
						}

						detonationTimer += global::DeltaTime;
						if (detonationTimer >= effectiveDetonationDelay)
						{
							this->adjustHealth(-9999);
						}
					}
					break;
				}
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
	void EnemyCharacter::adjustHealth(f32 Amount)
	{
		Stats.HP = std::clamp(Stats.HP + Amount, 0.f, Stats.MaxHP);
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
