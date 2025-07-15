#include "EnemyCharacter.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "../Manager/GameManager.h"
#include "../Manager/Playing.h"
#include <algorithm>
#include <random>
namespace InGame
{
	void InGame::EnemyCharacter::Init()
	{
		row = 6;
		column = 2;
		FrameTime = 0.2f;
		AnimationState = IDLE;
		AnimationCount = 0;
		MaxAnimationCount[IDLE] = 2;
		MaxAnimationCount[MOVE] = 2;
		MaxAnimationCount[ATTACK] = 2;
		MaxAnimationCount[DASH] = 2;
		Mesh = Utils::CreateMesh(row,column);
		MovementSpeed = 100.f;
		size.x = 40;
		size.y = 40;
		CollisionRadius = 20;
		Stats = new Stat;
		Stats->HP = 1;
		Stats->FireRate = 1.f;
		Stats->ProjectileSpeed = 30.f;
		Stats->Damage = 1;

		Stats->Init();

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
		AnimationState = IDLE;
		Type = InData->Type;
		Texture = InData->Texture;
		Stats->Damage = InData->Damage;
		Exp = InData->Exp;
		Stats->MovementSpeed = InData->MovementSpeed;
		Stats->ProjectileSpeed = InData->BulletSpeed;
		size = InData->DrawSize;
		CollisionRadius = InData->CollisionRadius;
		position = Pos;
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (GS->CurrentStageType == StageType::LAND)
				{
					Stats->MaxHP = InData->Health;
					Stats->HP = InData->Health;
				}
				else if (GS->CurrentStageType == StageType::TOWER)
				{
					Stats->MaxHP = InData->Health * 3.f;
					Stats->HP = InData->Health * 3.f;
				}
				else if (GS->CurrentStageType == StageType::HEAVEN)
				{
					Stats->MaxHP = InData->Health * 6.f;
					Stats->HP = InData->Health * 6.f;
				}
			}
		}
		

		AEVec2Set(&Stats->ProjectileSize, InData->ProjectileSize.x, InData->ProjectileSize.y);
		Stats->StatusEffectTimer[SLOW] = 0.f;
		Stats->StatusEffectTimer[BURN] = 0.f;
		Stats->StatusEffectTimer[STUN] = 0.f;
		Stats->StatusEffectTimer[FEAR] = 0.f;
		bIsPandingKill = false;
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
		/*--------ZIGZAG--------*/
		bIsZigzagMoving = false;
		bZigZagLeft = true;
		/*--------ZIGZAG--------*/
		/*--------ORBITER--------*/
		std::random_device rd;
		std::mt19937 gen(rd()); // 랜덤 엔진
		std::bernoulli_distribution dist(0.5); // true 확률: 50%

		bRotRight = dist(gen);
		OrbitAngle = 0.f;
		OrbitSpeed = 1.0f + static_cast<float>(rand() % 200) / 100.0f; // 1.0~3.0 rad/s
		OrbitRadius = 300.f;
		OrbitShootTimer = 0.f;
		OrbitShootInterval = 2.0f;
		/*--------ORBITER--------*/
		/*--------SNIPER--------*/
		sniperState = SniperState::APPROACHING;
		SniperShootTimer = 0.f;
		bRetreatDirInitialized = false;
		bIsWarned = false;
		bIsTargetDirValid = false;
		/*--------SNIPER--------*/
		/*--------BURNER--------*/
		float FlameZoneTimer = 0.f;
		bFlameWarned = false;
		/*--------BURNER--------*/
		/*--------HOLER--------*/
		float BlackHoleSpawnTimer = 0.f;
		bHoleWarned = false;
		/*--------HOLER--------*/
	}

	void InGame::EnemyCharacter::Update()
	{
		UpdateEffectTime();
		Utils::UpdateOffset(*this);
		if (Stats->StatusEffectTimer[SLOW] > 0)
		{
			FrameTime = 0.6f;
		}
		else
		{
			FrameTime = 0.2f;
		}
		if (Stats->StatusEffectTimer[BURN] > 0)
		{
			BurnTimer += global::DeltaTime;
			if (BurnTimer >= global::effectiveBurnRate)
			{
				BurnTimer = 0.f;
				adjustHealth(-Stats->MaxHP * global::effectiveBurnDamage);
			}
		}

		if (!(Stats->StatusEffectTimer[STUN] > 0))
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
			
			if (Stats->StatusEffectTimer[FEAR] > 0)
				AEVec2Scale(&direction, &direction, -1.f);

			switch (Type)
			{
				case EnemyType::MINION:
				{
					f32 effectiveMovementSpeed = Stats->MovementSpeed;

					if (Stats->StatusEffectTimer[SLOW] > 0)
					{
						effectiveMovementSpeed *= 0.5f;
					}
					AEVec2 NewPos = { position.x - direction.x * effectiveMovementSpeed * global::DeltaTime , position.y - direction.y * effectiveMovementSpeed * global::DeltaTime };
					Utils::ClampActorPosition(this, NewPos);
					
					break;
				}
				case EnemyType::ARCHER:
				{
					ProjectileSpawnTimer += global::DeltaTime;
					if (len > 500)
					{
						f32 effectiveMovementSpeed = Stats->MovementSpeed;

						if (Stats->StatusEffectTimer[SLOW] > 0)
						{
							effectiveMovementSpeed *= 0.5f;
						}
						AEVec2 NewPos = { position.x - direction.x * effectiveMovementSpeed * global::DeltaTime , position.y - direction.y * effectiveMovementSpeed * global::DeltaTime };
						Utils::ClampActorPosition(this, NewPos);
					}
					else
					{
						f32 effectiveChamberTimer = ProjectileChamberTimer;

						if (Stats->StatusEffectTimer[SLOW] > 0)
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
								dashDirection = direction;
								dashStartPos = position;
								bIsDashing = true;
								AnimationState = DASH;
							}

							AEVec2 moved;
							AEVec2Sub(&moved, &position, &dashStartPos);
							float dashMoved = AEVec2Length(&moved);

							if (dashMoved < dashRange)
							{
								f32 effectiveDashSpeed = dashSpeed;

								if (Stats->StatusEffectTimer[SLOW] > 0)
								{
									effectiveDashSpeed *= 0.5f;
								}
								AEVec2 NewPos = { position.x - direction.x * effectiveDashSpeed * global::DeltaTime , position.y - direction.y * effectiveDashSpeed * global::DeltaTime };
								Utils::ClampActorPosition(this, NewPos);
							}
							else
							{
								bIsDashing = false;
								bHasDashed = true;
								recoverTimer = 0.f;
								AnimationState = IDLE;
							}
						}
						else
						{
							AnimationState = MOVE;
							f32 effectiveMovementSpeed = walkSpeed;

							if (Stats->StatusEffectTimer[SLOW] > 0)
							{
								effectiveMovementSpeed *= 0.5f;
							}

							position.x -= direction.x * effectiveMovementSpeed * global::DeltaTime;
							position.y -= direction.y * effectiveMovementSpeed * global::DeltaTime;
						}
					}
					else
					{
						AnimationState = IDLE;
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
					f32 effectiveMovementSpeed = Stats->MovementSpeed;

					if (Stats->StatusEffectTimer[SLOW] > 0)
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
						f32 effectiveMovementSpeed = Stats->MovementSpeed;

						if (Stats->StatusEffectTimer[SLOW] > 0)
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

					if (Stats->StatusEffectTimer[SLOW] > 0)
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
				case EnemyType::ZIGZAG:
				{
					if (!bIsZigzagMoving)
					{
						AEVec2Sub(&MoveTargetDir, &global::PlayerLocation, &position);
						AEVec2Normalize(&MoveTargetDir, &MoveTargetDir);
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_real_distribution<float> dist(15, 60);

						float angleDeg = dist(gen);
						float angleRad = angleDeg * 3.14159265f / 180.0f;

						std::uniform_real_distribution<float> distSpeed(0.5f, 2.5f);
						ZigZagMaxTime = distSpeed(gen);
						if (bZigZagLeft)
						{
							AEVec2 leftRotated;
							leftRotated.x = direction.x * cosf(angleRad) - direction.y * sinf(angleRad);
							leftRotated.y = direction.x * sinf(angleRad) + direction.y * cosf(angleRad);
							MoveTargetDir = leftRotated;
							bZigZagLeft = !bZigZagLeft;
						}
						else
						{
							AEVec2 rightRotated;
							rightRotated.x = direction.x * cosf(-angleRad) - direction.y * sinf(-angleRad);
							rightRotated.y = direction.x * sinf(-angleRad) + direction.y * cosf(-angleRad);
							MoveTargetDir = rightRotated;
							bZigZagLeft = !bZigZagLeft;
						}
						ZigZagTimer = 0.f;
						bIsZigzagMoving = true;
					}
					else
					{
						ZigZagTimer += global::DeltaTime;
						if (ZigZagTimer < ZigZagMaxTime)
						{
							f32 effectiveMovementSpeed = Stats->MovementSpeed;

							if (Stats->StatusEffectTimer[SLOW] > 0)
							{
								effectiveMovementSpeed *= 0.5f;
							}
							AEVec2 NewPos = { position.x - MoveTargetDir.x * global::DeltaTime * effectiveMovementSpeed, position.y - MoveTargetDir.y * global::DeltaTime * effectiveMovementSpeed };
							Utils::ClampActorPosition(this, NewPos);

						}
						else
						{
							bIsZigzagMoving = false;
						}
					}
					break;
				}
				case EnemyType::ORBITER:
				{
					float lenToPlayer = AEVec2Distance(&position, &global::PlayerLocation);

					// 적 → 플레이어 방향 계산
					AEVec2 dirToPlayer;
					AEVec2Sub(&dirToPlayer, &global::PlayerLocation, &position);
					AEVec2Normalize(&dirToPlayer, &dirToPlayer);

					direction = dirToPlayer; // 탄 발사 및 시선용


					// 기본 회전 각도 설정
					float angleDeg = 0.f;

					if (lenToPlayer < 450.f && lenToPlayer > 300.f)
					{
						if (bRotRight)
						{
							angleDeg = 60.f;  // 더 가까이 붙음
						}
						else
						{
							angleDeg = -60.f;  // 더 가까이 붙음
						}
					}
					else if (lenToPlayer < 250.f)
					{
						if (bRotRight)
						{
							angleDeg = 120.f;  // 더 가까이 붙음
						}
						else
						{
							angleDeg = -120.f;  // 더 가까이 붙음
						}
					}
					else if(lenToPlayer > 250.f && lenToPlayer < 350.f)
					{
						if (bRotRight)
						{
							angleDeg = 90.f;  // 더 가까이 붙음
						}
						else
						{
							angleDeg = -90.f;  // 더 가까이 붙음
						}
					}
					float angleRad = angleDeg * 3.14159265f / 180.0f;

					// 방향을 angleRad 만큼 회전 (시계 방향)
					AEVec2 moveDir;
					moveDir.x = dirToPlayer.x * cosf(angleRad) + dirToPlayer.y * sinf(angleRad);
					moveDir.y = -dirToPlayer.x * sinf(angleRad) + dirToPlayer.y * cosf(angleRad);
					AEVec2Normalize(&moveDir, &moveDir);
					
					// 이동
					float speed = Stats->MovementSpeed * OrbitSpeed;
					if (Stats->StatusEffectTimer[SLOW] > 0)
						speed *= 0.5f;
					AEVec2 NewPos = { position.x + moveDir.x * speed * global::DeltaTime, position.y + moveDir.y * speed * global::DeltaTime };
					Utils::ClampActorPosition(this, NewPos);

					// 탄 발사
					OrbitShootTimer += global::DeltaTime;
					if (OrbitShootTimer >= OrbitShootInterval)
					{
						OrbitShootTimer = 0.f;
						AEVec2 FireDir = {-dirToPlayer.x,-dirToPlayer.y};
						SpawnProjectile(FireDir, position);
					}

					break;
				}
				case EnemyType::SNIPER:
				{
					// 플레이어 방향
					AEVec2 dirToPlayer;
					AEVec2Sub(&dirToPlayer, &global::PlayerLocation, &position);
					AEVec2Normalize(&dirToPlayer, &dirToPlayer);

					direction = dirToPlayer;

					float speed = Stats->MovementSpeed;
					if (Stats->StatusEffectTimer[SLOW] > 0)
						speed *= 0.5f;

					switch (sniperState)
					{
					case SniperState::APPROACHING:
						if (len > SniperApproachDistance)
						{
							AEVec2 NewPos = { position.x + dirToPlayer.x * speed * global::DeltaTime, position.y + dirToPlayer.y * speed * global::DeltaTime };
							Utils::ClampActorPosition(this, NewPos);
							//AnimationState = MOVE;
						}
						else
						{
							sniperState = SniperState::RETREATING;
							bRetreatDirInitialized = false;
						}
						break;

					case SniperState::RETREATING:
						if (!bRetreatDirInitialized)
						{
							// 플레이어 반대 방향으로 후퇴 방향 설정
							SniperRetreatDir.x = -dirToPlayer.x;
							SniperRetreatDir.y = -dirToPlayer.y;
							bRetreatDirInitialized = true;
							SniperRetreatStartPos = position;
						}

						if (AEVec2Distance(&position, &SniperRetreatStartPos) < SniperRetreatDistance)
						{
							AEVec2 NewPos = { position.x + SniperRetreatDir.x * speed * global::DeltaTime, position.y + SniperRetreatDir.y * speed * global::DeltaTime };
							Utils::ClampActorPosition(this, NewPos);
							//AnimationState = MOVE;
						}
						else
						{
							sniperState = SniperState::SHOOTING;
						}
						break;

					case SniperState::SHOOTING:
						SniperShootTimer += global::DeltaTime;
						
						if (SniperShootTimer >= SniperShootInterval)
						{
							SniperShootTimer = 0.f;
							LaserAttack* laser = new LaserAttack;
							laser->Init(position, SniperFireDir, this, 3200.f, 1.f, 0.f, false);
							bIsWarned = false;
							bIsTargetDirValid = false;
							if (Manager::gm.currStateREF)
							{
								Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
								if (GS)
								{
									GS->EAAs.push_back(laser);
								}
							}
							Manager::SFXManager.Play("LaserFire");
						}
						else if (SniperShootTimer >= SniperShootInterval - 0.2f)
						{
							if (!bIsTargetDirValid)
							{
								SniperFireDir = dirToPlayer;
								bIsTargetDirValid = true;
							}
						}
						else if (SniperShootTimer >= SniperShootInterval-1.2f)
						{
							if (!bIsWarned)
							{
								if (Manager::gm.currStateREF)
								{
									Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
									if (GS)
									{
										AEVec2 WarningDraw = { 3200.f, 30.f };
										GS->VFXManager.AddWarningVFX(VFXType::WarningSquare, position, WarningDraw, dirToPlayer,true, this);
										bIsWarned = true;
									}
								}
								Manager::SFXManager.Play("LaserWarning");
							}
						}
						//AnimationState = ATTACK;
						break;
					}
					break;
				}
				case EnemyType::BURNER:
				{
					FlameZoneTimer += global::DeltaTime;
					if (len > 500)
					{
						if (bFlameWarned)
						{
							bFlameWarned = false;
							FlameZoneTimer = 0.f;
						}
						f32 effectiveMovementSpeed = Stats->MovementSpeed;

						if (Stats->StatusEffectTimer[SLOW] > 0)
						{
							effectiveMovementSpeed *= 0.5f;
						}
						AEVec2 NewPos = { position.x - direction.x * effectiveMovementSpeed * global::DeltaTime, position.y - direction.y * effectiveMovementSpeed * global::DeltaTime };
						Utils::ClampActorPosition(this, NewPos);
					}
					else
					{
						if (FlameZoneTimer >= FlameZoneInterval)
						{
							FlameZoneTimer = 0.f;
							bFlameWarned = false;

							BurningAreaAttack* BurningArea = new BurningAreaAttack;
							BurningArea->Init(FlameZonePos, FlameZoneRadius, FlameZoneDuration, 1.f,this,false);

							if (Manager::gm.currStateREF)
							{
								Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
								if (GS)
								{
									GS->EAAs.push_back(BurningArea);
								}
							}
						}
						else if (FlameZoneTimer > FlameZoneInterval - 1.f)
						{
							if (!bFlameWarned)
							{
								bFlameWarned = true;
								if (Manager::gm.currStateREF)
								{
									Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
									if (GS)
									{
										FlameZonePos = global::PlayerLocation;
										AEVec2 FlameZoneSize = { FlameZoneRadius * 2.f,FlameZoneRadius * 2.f };
										GS->VFXManager.AddWarningVFX(VFXType::WarningCircle, FlameZoneSize, 1.f, FlameZonePos, false, this);
										bIsWarned = true;
									}
								}
							}
						}
					}
					break;
				}
				case EnemyType::HOLER:
				{
					BlackHoleSpawnTimer += global::DeltaTime;
					if (len > 500)
					{
						if (bHoleWarned)
						{
							bHoleWarned = false;
							BlackHoleSpawnTimer = 0.f;
						}
						f32 effectiveMovementSpeed = Stats->MovementSpeed;

						if (Stats->StatusEffectTimer[SLOW] > 0)
						{
							effectiveMovementSpeed *= 0.5f;
						}
						AEVec2 NewPos = { position.x - direction.x * effectiveMovementSpeed * global::DeltaTime, position.y - direction.y * effectiveMovementSpeed * global::DeltaTime };
						Utils::ClampActorPosition(this, NewPos);
					}
					else
					{
						if (BlackHoleSpawnTimer >= BlackHoleSpawnInterval)
						{
							BlackHoleSpawnTimer = 0.f;

							BlackholeAttack* BlackHole = new BlackholeAttack;
							AEVec2 Size = { BlackHoleRadius * 2.f,BlackHoleRadius * 2.f };
							
							BlackHole->Init(BlackHolePos, Size, BlackHoleRadius, BlackHoleDuration,this,false);

							if (Manager::gm.currStateREF)
							{
								Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
								if (GS)
								{
									GS->EAAs.push_back(BlackHole);
								}
							}
							bHoleWarned = false;
						}
						else if (BlackHoleSpawnTimer >= BlackHoleSpawnInterval - 1.f)
						{
							if (!bHoleWarned)
							{
								bHoleWarned = true;
								if (Manager::gm.currStateREF)
								{
									Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
									if (GS)
									{
										std::random_device rd;
										std::mt19937 gen(rd());
										std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159265f);
										std::uniform_real_distribution<float> radiusDist(0.0f, BlackHoleRadius);

										float angle = angleDist(gen);
										float radius = radiusDist(gen);

										AEVec2 spawnOffset = { cosf(angle) * radius, sinf(angle) * radius };

										BlackHolePos = { global::PlayerLocation.x + spawnOffset.x,global::PlayerLocation.y + spawnOffset.y };

										AEVec2 HoleSize = { BlackHoleRadius * 2.f,BlackHoleRadius * 2.f };
										GS->VFXManager.AddWarningVFX(VFXType::WarningCircle, HoleSize, 1.f, BlackHolePos, false, this);
										bIsWarned = true;
									}
								}
							}
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
		if (Stats)
		{
			delete Stats;
			Stats = nullptr;
		}
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
			Mesh = nullptr;
		}
		Texture = nullptr;
	}
	void EnemyCharacter::adjustHealth(f32 Amount)
	{
		if (Stats->StatusEffectTimer[VULNERABLE] > 0 && Amount < 0)
		{
			Amount *= 1.2f;
		}
		if (Stats->HP / Stats->MaxHP >= global::item12TriggerRatio && Utils::GetItemCount(12) > 0) //ID:12 Item's effect
		{
			Amount *= global::item12AdditionalDamage;
		}

		Stats->HP = std::clamp(Stats->HP + Amount, 0.0f, Stats->MaxHP);

		if (Stats->HP <= 0)
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
		for (size_t i = 0; i < Stats->StatusEffectTimer.size(); i++)
		{
			switch (i)
			{
			case 0:
				if (Stats->StatusEffectTimer[BURN] > 0)
					Stats->StatusEffectTimer[BURN] -= global::DeltaTime;
				if (Stats->StatusEffectTimer[BURN] < 0)
					Stats->StatusEffectTimer[BURN] = 0;
				break;
			case 1:
				if (Stats->StatusEffectTimer[STUN] > 0)
					Stats->StatusEffectTimer[STUN] -= global::DeltaTime;
				if (Stats->StatusEffectTimer[STUN] < 0)
					Stats->StatusEffectTimer[STUN] = 0;
				break;
			case 2:
				if (Stats->StatusEffectTimer[SLOW] > 0)
					Stats->StatusEffectTimer[SLOW] -= global::DeltaTime;
				if (Stats->StatusEffectTimer[SLOW] < 0)
					Stats->StatusEffectTimer[SLOW] = 0;
				break;
			case 3:
				if (Stats->StatusEffectTimer[FEAR] > 0)
					Stats->StatusEffectTimer[FEAR] -= global::DeltaTime;
				if (Stats->StatusEffectTimer[FEAR] < 0)
					Stats->StatusEffectTimer[FEAR] = 0;
				break;
			case 4:
				if (Stats->StatusEffectTimer[VULNERABLE] > 0)
					Stats->StatusEffectTimer[VULNERABLE] -= global::DeltaTime;
				if (Stats->StatusEffectTimer[VULNERABLE] < 0)
					Stats->StatusEffectTimer[VULNERABLE] = 0;
				break;
			default:
				break;
			}
		}
	}
}
