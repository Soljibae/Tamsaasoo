#include "PlayerCharacter.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include "../Manager/Playing.h"
#include "../Manager/LevelUpUI.h"
#include <iostream>
#include <algorithm>

namespace InGame
{
	void PlayerCharacter::Init()
	{
		position.x = 0;
		position.y = 0;
		CollisionRadius = 50;
		size.x = 100;
		size.y = 100;
		row = 4;
		column = 2;
		Mesh = Utils::CreateMesh(row, column);
		FrameTime = 0.2f;
		TimeAcc = 0.f;
		AnimationState = IDLE;
		AnimationCount = 0;
		MaxAnimationCount[IDLE] = 2;
		MaxAnimationCount[MOVE] = 2;
		MaxAnimationCount[JUMP] = 2;

		Texture = AEGfxTextureLoad("Assets/Character.png");
		GunData = new ShotGunStruct;
		HoldingGun = new Gun();
		HoldingGun->Init(this);

		Stats.MaxHP = 10;
		Stats.HP = Stats.MaxHP;
		Stats.MovementSpeed = 300;
		Stats.FireRate = 2.f;
		Stats.ProjectileSpeed = GunData->ProjectileSpeed;
		Stats.ProjectileCollisionSize = GunData->ProjectileCollisionSize;
		AEVec2Set(&Stats.ProjectileSize, 10.f, 10.f);
		Stats.Damage = 1.f;
		Stats.Level = 1;
		Stats.ExpGained = 1.f;
		Stats.HitCount = GunData->ProjectileHitCount;
		Stats.ExpCount = 0.f;
		Stats.TargetExp = 20.f;
		Stats.Money = 0;
		Stats.rerollCostRatio = 1.f;
		Stats.Potion = 0;
		Stats.BurnDamage = 0.1f;
		global::effectiveBurnDamage = Stats.BurnDamage;
		Stats.BurnRate = 1.f;
		Stats.ReviveCount = 0;
		Stats.StageExpGainedRatio[0] = 1.f;
		Stats.StageExpGainedRatio[1] = 2.5f;
		Stats.StageExpGainedRatio[2] = 6.f;

		Stats.Init();

		Utils::InitOffset(*this);
		inventory.clear();
	}
	void PlayerCharacter::Update()
	{
		if (Stats.MaxHP <= 0)
		{
			bIsPandingKill = true;
		}

		UpdateEffectTime();

		InitAdditionalStats();

		for (const auto& item_ptr : inventory)
		{
			item_ptr.first->Use(this);
		}

		UpdateStats();

		Stats.HP = std::clamp(Stats.HP, 0.f, Stats.MaxHP);
		Stats.effectiveMovementSpeed = std::clamp(Stats.effectiveMovementSpeed, 30.f, 600.f); // to do

		for (const auto& item_ptr : inventory)
		{
			item_ptr.first->Update(this);
		}

		if (bIsDashing)
		{
			UpdateDash();
		}
		else
		{
			DashCooldownTimer += global::DeltaTime;

			if (AEInputCheckTriggered(AEVK_SPACE) && DashCooldownTimer >= DashCooldown)
			{
				AEVec2Set(&DashDirection, 0.f, 0.f);
				if (AEInputCheckCurr(AEVK_W))
				{
					DashDirection.y += 1.f;
				}
				if (AEInputCheckCurr(AEVK_S))
				{
					DashDirection.y -= 1.f;
				}
				if (AEInputCheckCurr(AEVK_A))
				{
					DashDirection.x -= 1.f;
				}
				if (AEInputCheckCurr(AEVK_D))
				{
					DashDirection.x += 1.f;
				}

				if (DashDirection.x != 0.f || DashDirection.y != 0.f)
				{
					AEVec2Normalize(&DashDirection, &DashDirection);
					bIsDashing = true;
				}
			}

			if (Stats.HP > 0)
			{
				UpdateMovement();
			}
		}

		GetMouseDir();

		if (HoldingGun)
		{
			HoldingGun->Update(MouseDirection, position);
			if (size.x * MouseDirection.x >= 0)
			{
				HoldingGun->ArmOffset.x = fabsf(HoldingGun->ArmOffset.x);
			}
			else
			{
				HoldingGun->ArmOffset.x = -fabsf(HoldingGun->ArmOffset.x);
			}
		}

		if (bIsInvincible)
		{
			InvincibleTimer += global::DeltaTime;
			if (InvincibleTimer > 0.5f)
			{
				bIsInvincible = false;
				InvincibleTimer = 0.f;
			}
		}
		global::PlayerLocation = position;
		global::PlayerMouseDirection = MouseDirection;
		Utils::UpdateOffset(*this);
		for (const auto& item_ptr : inventory)
		{
			item_ptr.first->Use(this);
		}
		/*----- Heal Potion -----*/
		if (Stats.Potion > 100)
			Stats.Potion = 100;
		if (global::KeyInput(AEVK_Q))
		{
			if (Stats.HP < Stats.MaxHP)
			{
				if (Stats.Potion >= 50)
				{
					adjustHealth(1);
					Stats.Potion -= 50;
				}
			}
		}
		/*----- Heal Potion -----*/
	}
	void PlayerCharacter::Draw()
	{
		
		if (bIsInvincible)
		{
			HoldingGun->Draw(0.5f);
			Utils::DrawObject(*this, true, 0.5f);
		}
		else
		{
			HoldingGun->Draw();
			Utils::DrawObject(*this);
		}
		
	}
	void PlayerCharacter::Destroy()
	{
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
		}
		if (Texture)
		{
			AEGfxTextureUnload(Texture);
		}
		if (GunData)
		{
			delete GunData;
		}
		if (HoldingGun)
		{
			HoldingGun->Destroy();
			delete HoldingGun;
			HoldingGun = nullptr;
		}
	}
	void PlayerCharacter::adjustHealth(f32 Amount)
	{
		Amount = static_cast<s32>(Amount);

		if (Amount >= 0)
		{
			Stats.HP = std::clamp(Stats.HP + Amount, 0.f, Stats.MaxHP);

			return;
		}

		if (!bIsInvincible)
		{
			Stats.HP = std::clamp(Stats.HP + Amount, 0.f, Stats.MaxHP);
			if (Stats.HP <= 0)
			{
				if (IsRevivable())
				{
					bIsInvincible = true;
					Stats.HP = Stats.MaxHP; // to do
				}
				else
				{
					bIsPandingKill = true;
				}
			}
			else
			{
				bIsInvincible = true;
			}
		}
	}
	void PlayerCharacter::GetMouseDir()
	{
		s32 MX;
		s32 MY;
		AEInputGetCursorPosition(&MX, &MY);

		AEVec2 MP;
		MP.x = static_cast<float>(MX) - AEGfxGetWindowWidth() / 2.0f;
		MP.y = AEGfxGetWindowHeight() / 2.0f - static_cast<float>(MY);

		AEMtx33 translate_matrix;
		AEMtx33Inverse(&translate_matrix, &(Manager::CAM->translate_matrix));

		AEVec2 Result;
		AEMtx33MultVec(&Result, &translate_matrix, &MP);
		f32 length = AEVec2Distance(&position, &Result);
		f32 dx = Result.x - position.x;
		f32 dy = Result.y - position.y;
		MouseDirection.x = dx / length;
		MouseDirection.y = dy / length;
		
		if (size.x > 0 && MouseDirection.x < 0) size.x *= -1;
		if (size.x < 0 && MouseDirection.x > 0) size.x *= -1;
	}
	void PlayerCharacter::UpdateKill(u32 Exp)
	{
		Stats.ExpCount += Exp * Stats.effectiveExpGained * Stats.StageExpGainedRatio[global::CurrentStageNumber - 1];
		if (Stats.ExpCount >= Stats.TargetExp)
		{
			Stats.ExpCount -= Stats.TargetExp;
			Stats.TargetExp *= 1.45f;
			Stats.Level++;
			std::cout << "Level Up : " << Stats.Level << " Next : Target Exp : " << Stats.TargetExp << std::endl;
			Manager::pickPanel.Show();
		}
	}
	void PlayerCharacter::AddItemToInventory(std::shared_ptr<Item> item)
	{
		auto it = std::find_if(inventory.begin(), inventory.end(),
			[&](const auto& pair) {return pair.first->id == item->id; });
		if (it == inventory.end())
		{
			inventory.push_back({ std::move(item), 1 });
		}
		else
		{
			it->second++;
		}
	}
	void PlayerCharacter::UpdateMovement()
	{
		AEVec2 MovingVec;
		AEVec2Set(&MovingVec, 0.f, 0.f);

		if (AEInputCheckCurr(AEVK_W))
		{
			MovingVec.y += 1.f;
		}
		if (AEInputCheckCurr(AEVK_S))
		{
			MovingVec.y -= 1.f;
		}
		if (AEInputCheckCurr(AEVK_A))
		{
			MovingVec.x -= 1.f;
			//if (size.x > 0) size.x *= -1;
		}
		if (AEInputCheckCurr(AEVK_D))
		{
			MovingVec.x += 1.f;
			//if (size.x < 0) size.x *= -1;
		}

		if (MovingVec.x == 0 && MovingVec.y == 0)
		{
			if (AnimationState == MOVE)
			{
				AnimationCount = 0;
			}
			AnimationState = IDLE;
		}
		else
		{
			if (AnimationState == IDLE)
			{
				AnimationCount = 0;
			}
			AnimationState = MOVE;
		}

		if (MovingVec.x != 0 && MovingVec.y != 0)
		{
			AEVec2Normalize(&MovingVec, &MovingVec);
		}

		AEVec2 delta;
		AEVec2Scale(&delta, &MovingVec, Stats.effectiveMovementSpeed * global::DeltaTime);

		AEVec2 newPos = { position.x + delta.x, position.y + delta.y };

		if (Manager::gm.currStateREF)
		{
			Manager::Playing* PC = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (PC)
			{
				if (PC->CurrentStageType == InGame::StageType::TOWER)
				{
					float EllipseA = (global::worldMax.x - global::worldMin.x) / 2;
					float EllipseB = (global::worldMax.y - global::worldMin.y) / 2;
					float value = (newPos.x * newPos.x) / (EllipseA * EllipseA) + (newPos.y * newPos.y) / (EllipseB * EllipseB);
					if (value <= 1.0f)
					{
						position = newPos;
					}
				}
				else
				{
					position.x = std::clamp(newPos.x, global::worldMin.x + abs(size.x) / 2, global::worldMax.x - abs(size.x) / 2);
					position.y = std::clamp(newPos.y, global::worldMin.y + size.y / 2, global::worldMax.y - size.y / 2);
				}
			}
		}
	}
	void PlayerCharacter::UpdateDash()
	{
		DashTimer += global::DeltaTime;
		AEVec2 delta;
		AEVec2Scale(&delta, &DashDirection, DashSpeed * global::DeltaTime);

		AEVec2 newPos = { position.x + delta.x, position.y + delta.y };

		if (Manager::gm.currStateREF)
		{
			Manager::Playing* PC = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (PC)
			{
				if (PC->CurrentStageType == InGame::StageType::TOWER)
				{
					float EllipseA = (global::worldMax.x - global::worldMin.x) / 2;
					float EllipseB = (global::worldMax.y - global::worldMin.y) / 2;
					float value = (newPos.x * newPos.x) / (EllipseA * EllipseA) + (newPos.y * newPos.y) / (EllipseB * EllipseB);
					if (value <= 1.0f)
					{
						position = newPos;
					}
				}
				else
				{
					position.x = std::clamp(newPos.x, global::worldMin.x + abs(size.x) / 2, global::worldMax.x - abs(size.x) / 2);
					position.y = std::clamp(newPos.y, global::worldMin.y + size.y / 2, global::worldMax.y - size.y / 2);
				}
			}
		}
		if (DashTimer >= DashTime)
		{
			bIsDashing = false;
			DashTimer = 0.0f;
			DashCooldownTimer = 0.0f;
		}
	}
	void PlayerCharacter::OnProjectileHit(InGame::EnemyCharacter* target, bool isTargetBoss)
	{
		for (const auto& item_pair : inventory)
		{
			item_pair.first->OnHit(target, isTargetBoss);
		}
	}
	void PlayerCharacter::OnDamaged()
	{
		if (!bIsInvincible)
		{
			for (const auto& item_pair : inventory)
			{
				item_pair.first->OnDamaged();
			}
		}
	}
	void PlayerCharacter::UpdateEffectTime()
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
				break;
			case 1:
				if (Stats.StatusEffectTimer[STUN] > 0)
					Stats.StatusEffectTimer[STUN] -= global::DeltaTime;
				if (Stats.StatusEffectTimer[STUN] < 0)
					Stats.StatusEffectTimer[STUN] = 0;
				break;
			case 2:
				if (Stats.StatusEffectTimer[SLOW] > 0)
					Stats.StatusEffectTimer[SLOW] -= global::DeltaTime;
				if (Stats.StatusEffectTimer[SLOW] < 0)
					Stats.StatusEffectTimer[SLOW] = 0;
				break;
			case 3:
				if (Stats.StatusEffectTimer[FEAR] > 0)
					Stats.StatusEffectTimer[FEAR] -= global::DeltaTime;
				if (Stats.StatusEffectTimer[FEAR] < 0)
					Stats.StatusEffectTimer[FEAR] = 0;
				break;
			case 4:
				if (Stats.StatusEffectTimer[VULNERABLE] > 0)
					Stats.StatusEffectTimer[VULNERABLE] -= global::DeltaTime;
				if (Stats.StatusEffectTimer[VULNERABLE] < 0)
					Stats.StatusEffectTimer[VULNERABLE] = 0;
				break;
			default:
				break;
			}
		}
	}
	void PlayerCharacter::InitAdditionalStats()
	{
		global::additionalDamage = 0.f;
		global::additionalDamageRatio = 1.f;
		global::additionalFireRate = 0.f;
		global::additionalFireRateRatio = 1.f;
		global::additionalExpGainedRatio = 1.f;
		global::additionalHitCount = 0;
		global::additionalMovementSpeed = 0;

		global::additionalBurnDamage = 0.f;
		global::additionalBurnRate = 0.f;

		global::additionalMinionDamage = 0.f;
		global::additionalMinionDamageRatio = 1.f;
		global::additionalMinionFireRate = 0.f;
		global::additionalMinionFireRateRatio = 1.f;
		global::additionalMinionHitCount = 0;

		global::additionalProcChanceRatio = 1.f;

		global::additionalDamageToBossRatio = 1.f;
		global::additionalDamageFromBossRatio = 1.f;
	}

	void PlayerCharacter::UpdateStats()
	{
		std::map<ItemTag, s32> itemTagCount;

		for (int i = static_cast<int>(ENVY); i <= static_cast<int>(PRIDE); ++i)
		{
			itemTagCount[static_cast<ItemTag>(i)] = 0;
		}

		for (const auto& item_ptr : inventory)
		{
			itemTagCount[item_ptr.first->tag] += item_ptr.second;
		}

		if (itemTagCount[PRIDE] >= 7)
		{
			global::additionalFireRate += 0.8;
		}
		else if (6 >= itemTagCount[PRIDE] && itemTagCount[PRIDE] >= 5)
		{
			global::additionalFireRate += 0.5;
		}
		else if (4 >= itemTagCount[PRIDE] && itemTagCount[PRIDE] >= 3)
		{
			global::additionalFireRate += 0.2;
		}

		if (itemTagCount[WRATH] >= 7)
		{
			global::additionalBurnDamage += 0.08;
		}
		else if (6 >= itemTagCount[WRATH] && itemTagCount[WRATH] >= 5)
		{
			global::additionalBurnDamage += 0.05;
		}
		else if (4 >= itemTagCount[WRATH] && itemTagCount[WRATH] >= 3)
		{
			global::additionalBurnDamage += 0.02;
		}

		if (itemTagCount[SLOTH] >= 7)
		{
			global::additionalHitCount += 3;
		}
		else if (6 >= itemTagCount[SLOTH] && itemTagCount[SLOTH] >= 5)
		{
			global::additionalHitCount += 2;
		}
		else if (4 >= itemTagCount[SLOTH] && itemTagCount[SLOTH] >= 3)
		{
			global::additionalHitCount += 1;
		}

		static s16 healthUpByTagCount = 0;

		if (itemTagCount[GLUTTONY] >= 3 && healthUpByTagCount == 0)
		{
			healthUpByTagCount++;
			Stats.MaxHP++;
			Stats.HP++;
		}
		if (itemTagCount[GLUTTONY] >= 5 && healthUpByTagCount == 1)
		{
			healthUpByTagCount++;
			Stats.MaxHP++;
			Stats.HP++;
		}
		if (itemTagCount[GLUTTONY] >= 7 && healthUpByTagCount == 2)
		{
			healthUpByTagCount++;
			Stats.MaxHP++;
			Stats.HP++;
		}

		if (itemTagCount[GREED] >= 7)
		{
			
		}
		else if (6 >= itemTagCount[GREED] && itemTagCount[GREED] >= 5)
		{
			
		}
		else if (4 >= itemTagCount[GREED] && itemTagCount[GREED] >= 3)
		{
			
		}

		if (itemTagCount[LUST] >= 7)
		{
			global::additionalProcChanceRatio += 0.20;
		}
		else if (6 >= itemTagCount[LUST] && itemTagCount[LUST] >= 5)
		{
			global::additionalProcChanceRatio += 0.15;
		}
		else if (4 >= itemTagCount[LUST] && itemTagCount[LUST] >= 3)
		{
			global::additionalProcChanceRatio += 0.07;
		}

		if (itemTagCount[ENVY] >= 7)
		{
			global::additionalMovementSpeed += 60;
		}
		else if (6 >= itemTagCount[ENVY] && itemTagCount[ENVY] >= 5)
		{
			global::additionalMovementSpeed += 35;
		}
		else if (4 >= itemTagCount[ENVY] && itemTagCount[ENVY] >= 3)
		{
			global::additionalMovementSpeed += 20;
		}

		Stats.effectiveDamage = (Stats.Damage + global::additionalDamage) * global::additionalDamageRatio * GunData->GuntypeDamageRatio;
		Stats.effectiveFireRate = (Stats.FireRate + global::additionalFireRate) * global::additionalFireRateRatio * GunData->GuntypeFireRateRatio;
		Stats.effectiveExpGained = Stats.ExpGained * global::additionalExpGainedRatio;
		Stats.effectiveHitCount = Stats.HitCount + global::additionalHitCount;
		Stats.effectiveMovementSpeed = Stats.MovementSpeed + global::additionalMovementSpeed;

		global::effectiveBurnDamage = Stats.BurnDamage + global::additionalBurnDamage;
		global::effectiveBurnRate = Stats.BurnRate - global::additionalBurnRate;
	}

	bool PlayerCharacter::IsRevivable()
	{
		if (Stats.ReviveCount > 0)
		{
			Stats.ReviveCount--;
			return true;
		}
		return false;
	}
}