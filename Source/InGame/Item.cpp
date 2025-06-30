#include "Item.h"
#include "PlayerCharacter.h"
#include "../Utils/Utils.h"
#include <iostream>
#include "../Global/GlobalVariables.h"
#include "../Manager/Playing.h"
#include "../Manager/GameManager.h"
#include "Stat.h"
#include <algorithm>

namespace InGame
{
	AEVec2 Item::size;
	s32 Item::row = 7, Item::column = 5;

	Item::Item(const Item& other)
		: id(other.id), name(other.name), description(other.description), iconPosition(other.iconPosition),
		iconOffset(other.iconOffset), tag(other.tag), effectTime(other.effectTime), procChance(other.procChance)
	{
	}
	void Item::DrawIcon()
	{
		Utils::DrawItem(*this);
	}
	void Item::StaticInit()
	{
		AEVec2Set(&size, 64.f, 64.f);
	}
	SkillEffectItem::SkillEffectItem(const SkillEffectItem& other)
		: Item(other), FrameTime(other.FrameTime), FireTimer(other.FireTimer), AnimationOffset(other.AnimationOffset),
		AnimationCount(other.AnimationCount), AnimationTimer(other.AnimationTimer), effectPosition(other.effectPosition), effectSize(other.effectSize),
		effectRow(other.effectRow), effectColumn(other.effectColumn), MaxAnimationCount(other.MaxAnimationCount)
	{
	}
	//============================================= ID_1
	Item_1::Item_1(const Item_1& other)
		: Item(other)
	{
	}
	void Item_1::Init()
	{
		id = 1;
		name = "PRIDE_1";
		description = "Your attack power is increased by an amount proportional to your missing health when you are below half health.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = PRIDE;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);

	}
	void Item_1::Use(PlayerCharacter* owner)
	{
		if (owner->Stats.HP / owner->Stats.MaxHP <= 0.5f)
		{
			global::additionalDamageRatio += (0.05f * (owner->Stats.MaxHP - owner->Stats.HP) * (1.f + (Utils::GetItemCount(this->id) - 1) * 0.1f));
		}
	}
	void Item_1::Update(PlayerCharacter* owner)
	{
	}
	void Item_1::Draw()
	{
	}
	std::shared_ptr<Item> Item_1::Clone() const
	{
		return std::make_shared<Item_1>(*this);
	}
	//============================================= ID_2
	Item_2::Item_2(const Item_2& other)
		: Item(other), prev_PlayerPos(other.prev_PlayerPos), stacks(other.stacks), triggerTimer(other.triggerTimer),
		isActivated(other.isActivated), stackTimer(other.stackTimer)
	{
	}
	void Item_2::Init()
	{
		id = 2;
		name = "item_2";
		description = "After standing still for 3 seconds, your fire rate increases every second. This effect stacks up to 10 times.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = PRIDE;
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				prev_PlayerPos = GS->PC->position;
			}
		}

		triggerTimer = 0.f;
		stackTimer = 0.f;
		stacks = 0;
		isActivated = false;
		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_2::Use(PlayerCharacter* owner)
	{
		if (AEVec2Distance(&prev_PlayerPos, &owner->position) <= 0.1f)
		{
			if (!isActivated)
			{
				triggerTimer += global::DeltaTime;
				if (triggerTimer >= 3.f)
				{
					triggerTimer = 0.f;
					isActivated = true;
					stacks = 1;
				}
			}
			else
			{
				stackTimer += global::DeltaTime;
				if (stackTimer >= 1.f)
				{
					stackTimer = 0.f;
					stacks = std::clamp(stacks + 1, 0, 10);
				}
			}	
		}
		else
		{
			triggerTimer = 0.f;
			stackTimer = 0.f;
			stacks = 0;
			isActivated = false;
		}

		global::additionalFireRate += ((static_cast<f32>(stacks) * 0.05f) * (1.f + (Utils::GetItemCount(id) - 1) * 0.1));

		prev_PlayerPos = owner->position;
	}
	void Item_2::Update(PlayerCharacter* owner)
	{
	}
	void Item_2::Draw()
	{
	}
	std::shared_ptr<Item> Item_2::Clone() const
	{
		return std::make_shared<Item_2>(*this);
	}
	//============================================= ID_3
	Item_3::Item_3(const Item_3& other)
		: Item(other)
	{
	}
	void Item_3::Init()
	{
		id = 3;
		name = "item_3";
		description = "this is item_3";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = PRIDE;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_3::Use(PlayerCharacter* owner)
	{
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				global::additionalDamage += (static_cast<f32>(GS->ECs.size()) * 0.05f * ((1.f) + ((Utils::GetItemCount(id) - 1) * 0.1f)));
			}
		}
	}
	void Item_3::Update(PlayerCharacter* owner)
	{
	}
	void Item_3::Draw()
	{
	}
	std::shared_ptr<Item> Item_3::Clone() const
	{
		return std::make_shared<Item_3>(*this);
	}
	//============================================= ID_4
	Item_4::Item_4(const Item_4& other)
		: SkillEffectItem(other), appliedStack(other.appliedStack)
	{
	}
	void Item_4::Init()
	{
		id = 4;
		name = "item_4";
		description = "Reduces Max HP by 2. Grants one revive on death.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = PRIDE;
		appliedStack = 0;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_4::Use(PlayerCharacter* owner)
	{	
		if (Utils::GetItemCount(id) != appliedStack)
		{
			appliedStack++;
			owner->Stats.ReviveCount++;
			owner->Stats.MaxHP -= 2;
		}
		
	}
	void Item_4::Update(PlayerCharacter* owner)
	{
	}
	void Item_4::Draw()
	{
	}
	std::shared_ptr<Item> Item_4::Clone() const
	{
		return std::make_shared<Item_4>(*this);
	}
	//============================================= ID_5
	Item_5::Item_5(const Item_5& other)
		: Item(other)
	{
	}
	void Item_5::Init()
	{
		id = 5;
		name = "item_5";
		description = "Increases damage dealt to bosses by 50%, but you take 100% more damage from them.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = PRIDE;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_5::Use(PlayerCharacter* owner)
	{
		global::additionalDamageToBossRatio += 0.5f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
		global::additionalDamageFromBossRatio += 1.f;
	}
	void Item_5::Update(PlayerCharacter* owner)
	{
	}
	void Item_5::Draw()
	{
	}
	std::shared_ptr<Item> Item_5::Clone() const
	{
		return std::make_shared<Item_5>(*this);
	}
	//============================================= ID_6
	Item_6::Item_6(const Item_6& other)
		: Item(other)
	{
	}
	void Item_6::Init()
	{
		id = 6;
		name = "item_6";
		description = "Your attacks have a 30% chance to set the enemy on fire for 3 seconds.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		effectTime = 3.f;
		procChance = 0.3f;
		tag = WRATH;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_6::Use(PlayerCharacter* owner)
	{
	}
	void Item_6::Update(PlayerCharacter* owner)
	{
	}
	void Item_6::Draw()
	{
	}
	std::shared_ptr<Item> Item_6::Clone() const
	{
		return std::make_shared<Item_6>(*this);
	}
	void Item_6::OnHit(InGame::EnemyCharacter* target, bool isTargetBoss)
	{
		if (Utils::GetRandomFloat(0.f, 1.f) <= (procChance + (Utils::GetItemCount(id) - 1) * procChance / 10.f) * global::additionalProcChanceRatio)
			target->Stats.StatusEffectTimer[BURN] = effectTime;
	}
	//============================================= ID_7
	Item_7::Item_7(const Item_7& other)
		: Item(other)
	{
	}
	void Item_7::Init()
	{
		id = 7;
		name = "item_7";
		description = "Increases the damage your Burn deals based on the enemy's Max Health.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = WRATH;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_7::Use(PlayerCharacter* owner)
	{
		global::additionalBurnDamage += 0.05f * (1.f + (Utils::GetItemCount(this->id) - 1) * 0.1f);
	}
	void Item_7::Update(PlayerCharacter* owner)
	{
	}
	void Item_7::Draw()
	{
	}
	std::shared_ptr<Item> Item_7::Clone() const
	{
		return std::make_shared<Item_7>(*this);
	}
	//============================================= ID_8
	Item_8::Item_8(const Item_8& other)
		: Item(other)
	{
	}
	void Item_8::Init()
	{
		id = 8;
		name = "item_8";
		description = "Increases the frequency of your Burn damage ticks.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = WRATH;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_8::Use(PlayerCharacter* owner)
	{
		global::additionalBurnRate = 0.2f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
	}
	void Item_8::Update(PlayerCharacter* owner)
	{
	}
	void Item_8::Draw()
	{
	}
	std::shared_ptr<Item> Item_8::Clone() const
	{
		return std::make_shared<Item_8>(*this);
	}
	//============================================= ID_9
	Item_9::Item_9(const Item_9& other)
		: SkillEffectItem(other), CoolDown(other.CoolDown), isStarted(other.isStarted), isReady(other.isReady)
	{
	}
	void Item_9::Init()
	{
		id = 9;
		name = "item_9";
		description = "Every 2 seconds, taking damage unleashes a fiery nova, burning nearby enemies for 30% of their max HP and applying Burn.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		effectTime = 5.f;
		tag = WRATH;
		CoolDown = 2.f;
		FireTimer = 0.f;
		isStarted = false;
		isReady = false;
		AEVec2Set(&effectPosition, 0.f, 0.f);
		FrameTime = 0.1f;
		AEVec2Set(&AnimationOffset, 0.f, 0.f);
		AEVec2Set(&effectSize, 400.f, 400.f);
		AnimationCount = 0;
		AnimationTimer = 0.f;
		effectRow = 1;
		effectColumn = 9;
		MaxAnimationCount = 9;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_9::Use(PlayerCharacter* owner)
	{
		effectPosition = owner->position;

		FireTimer += global::DeltaTime;
		if (FireTimer >= CoolDown)
		{
			FireTimer = 0;
			isReady = true;
		}

		if (isStarted)
		{
			Utils::UpdateOffset(*this);
		}
	}
	void Item_9::Update(PlayerCharacter* owner)
	{
		
	}
	void Item_9::Draw()
	{
		if (isStarted)
		{
			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->ITRM)
					{
						Utils::DrawObject(*this, GS->ITRM->explosionTexture, GS->ITRM->explosionMesh);
					}
				}
			}

			if (AnimationCount == MaxAnimationCount - 1)
			{
				AnimationCount = 0;
				isStarted = false;
			}
		}
	}
	std::shared_ptr<Item> Item_9::Clone() const
	{
		return std::make_shared<Item_9>(*this);
	}
	void Item_9::OnDamaged()
	{
		if (isReady)
		{
			isReady = false;
			
			isStarted = true;
			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->ECs.size() > 0)
					{
						for (size_t i = 0; i < GS->ECs.size(); i++)
						{
							if (Utils::CheckCollision(*GS->ECs[i], effectPosition, effectSize.x / 2))
							{
								GS->ECs[i]->Stats.StatusEffectTimer[BURN] = effectTime;
								
								if (global::isBossBattleStarted)
								{
									GS->ECs[i]->adjustHealth(-GS->ECs[i]->Stats.MaxHP * 0.3f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1) * 0.1f);
								}
								else
								{
									GS->ECs[i]->adjustHealth(-GS->ECs[i]->Stats.MaxHP * 0.3f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1));
								}
							}
						}
					}
				}
			}
		}
	}
	//============================================= ID_10
	Item_10::Item_10(const Item_10& other)
		: Item(other)
	{
	}
	void Item_10::Init()
	{
		id = 10;
		name = "item_10";
		description = "Your damage multiplier increases based on the amount of Burn damage you inflict.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = WRATH;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_10::Use(PlayerCharacter* owner)
	{
		global::additionalDamageRatio += global::effectiveBurnDamage * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
	}
	void Item_10::Update(PlayerCharacter* owner)
	{
	}
	void Item_10::Draw()
	{
	}
	std::shared_ptr<Item> Item_10::Clone() const
	{
		return std::make_shared<Item_10>(*this);
	}
	//============================================= ID_11
	Item_11::Item_11(const Item_11& other)
		: SkillEffectItem(other), CoolDown(other.CoolDown), isStarted(other.isStarted), isReady(other.isReady), 
		additionalEffectSizeRatio(other.additionalEffectSizeRatio), baseEffectSize(other.baseEffectSize)
	{
	}
	void Item_11::Init()
	{
		id = 11;
		name = "item_11";
		description = "this is item_11";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		CoolDown = 5.f;
		isStarted = true;
		isReady = true;
		FireTimer = 0.f;
		FrameTime = 0.1f;
		AEVec2Set(&effectSize, 0.f, 0.f);
		additionalEffectSizeRatio = 1.f;
		AEVec2Set(&AnimationOffset, 0.f, 0.f);
		AEVec2Set(&baseEffectSize, 400.f, 400.f);
		AnimationCount = 0;
		AnimationTimer = 0.f;
		effectRow = 1;
		effectColumn = 9;
		tag = SLOTH;
		effectTime = 3.f;
		MaxAnimationCount = 9;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_11::Use(PlayerCharacter* owner)
	{
		
	}
	void Item_11::Update(PlayerCharacter* owner)
	{
		if ((owner->Stats.FireRate * owner->GunData->GuntypeFireRateRatio) / owner->Stats.effectiveFireRate > 1)
		{
			additionalEffectSizeRatio = (owner->Stats.FireRate * owner->GunData->GuntypeFireRateRatio) / owner->Stats.effectiveFireRate;
		}
		else
		{
			additionalEffectSizeRatio = 1.f;
		}

		AEVec2Scale(&effectSize, &baseEffectSize, additionalEffectSizeRatio);

		if (!isStarted)
		{
			FireTimer += global::DeltaTime;
			if (FireTimer >= CoolDown)
			{
				FireTimer = 0;
				isReady = true;
			}
		}

		if (isReady)
		{
			effectPosition = owner->position;
			isReady = false;
			isStarted = true;

			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->ECs.size() > 0)
					{
						for (size_t i = 0; i < GS->ECs.size(); i++)
						{
							if (Utils::CheckCollision(*GS->ECs[i], effectPosition, effectSize.x / 2))
							{
								GS->ECs[i]->Stats.StatusEffectTimer[STUN] = effectTime;
							}
						}
					}
				}
			}
		}

		if (isStarted)
		{
			Utils::UpdateOffset(*this);
		}
	}
	void Item_11::Draw()
	{
		if (isStarted)
		{
			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->ITRM)
					{
						Utils::DrawObject(*this, GS->ITRM->explosionTexture, GS->ITRM->explosionMesh);
					}
				}
			}

			if (AnimationCount == MaxAnimationCount - 1)
			{
				AnimationCount = 0;
				isStarted = false;
			}
		}
	}
	std::shared_ptr<Item> Item_11::Clone() const
	{
		return std::make_shared<Item_11>(*this);
	}
	//============================================= ID_12
	Item_12::Item_12(const Item_12& other)
		: Item(other)
	{
	}
	void Item_12::Init()
	{
		id = 12;
		name = "item_12";
		description = "Deal 50% more damage to enemies above 80% health.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = SLOTH;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_12::Use(PlayerCharacter* owner)
	{
	}
	void Item_12::Update(PlayerCharacter* owner)
	{
	}
	void Item_12::Draw()
	{
	}
	std::shared_ptr<Item> Item_12::Clone() const
	{
		return std::make_shared<Item_12>(*this);
	}
	//============================================= ID_13
	Item_13::Item_13(const Item_13& other)
		: SkillEffectItem(other), CoolDown{ other.CoolDown }, isReady{ other.isReady }, isStarted{ other.isStarted }, 
		additionalEffectSizeRatio(other.additionalEffectSizeRatio), baseEffectSize(other.baseEffectSize)
	{
	}
	void Item_13::Init()
	{
		id = 13;
		name = "item_13";
		description = "Reduces fire rate. A defeated enemy will explode, damaging nearby foes. This effect can only occur once every 4 seconds. The explosion's radius increases as your fire rate decreases";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		AEVec2Set(&baseEffectSize, 400.f, 400.f);
		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
		isReady = true;
		isStarted = false;
		CoolDown = 4.f;
		FireTimer = 0.f;
		FrameTime = 0.1f;
		AEVec2Set(&AnimationOffset, 0.f, 0.f);
		AnimationCount = 0;
		AnimationTimer = 0.f;
		effectRow = 1;
		effectColumn = 9;
		MaxAnimationCount = 9;
		tag = SLOTH;
	}
	void Item_13::Use(PlayerCharacter* owner)
	{
		global::additionalFireRate -= 0.2f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
	}
	void Item_13::Update(PlayerCharacter* owner)
	{
		if ((owner->Stats.FireRate * owner->GunData->GuntypeFireRateRatio) / owner->Stats.effectiveFireRate > 1)
		{
			additionalEffectSizeRatio = (owner->Stats.FireRate * owner->GunData->GuntypeFireRateRatio) / owner->Stats.effectiveFireRate;
		}
		else
		{
			additionalEffectSizeRatio = 1.f;
		}

		AEVec2Scale(&effectSize, &baseEffectSize, additionalEffectSizeRatio);

		if (!isStarted)
		{
			CoolDown = 4.f * (1.f - (Utils::GetItemCount(id) - 1) * 0.05);

			FireTimer += global::DeltaTime;
			if (FireTimer >= CoolDown)
			{
				FireTimer = 0;
				isReady = true;
			}
		}

		if (isReady && global::IsEnemyRecentlyDied)
		{
			effectPosition = global::RecentlyDeadEnemyPosition;
			isReady = false;
			isStarted = true;

			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->ECs.size() > 0)
					{
						for (size_t i = 0; i < GS->ECs.size(); i++)
						{
							if (Utils::CheckCollision(*GS->ECs[i], effectPosition, effectSize.x / 2))
							{
								GS->ECs[i]->adjustHealth(-owner->Stats.effectiveDamage);
							}
						}
					}
				}
			}
		}

		if (isStarted)
		{
			Utils::UpdateOffset(*this);
		}
	}
	void Item_13::Draw()
	{
		if (isStarted)
		{
			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->ITRM)
					{
						Utils::DrawObject(*this, GS->ITRM->explosionTexture, GS->ITRM->explosionMesh);
					}
				}
			}

			if (AnimationCount == MaxAnimationCount - 1)
			{
				AnimationCount = 0;
				isStarted = false;
			}
		}
	}
	std::shared_ptr<Item> Item_13::Clone() const
	{
		return std::make_shared<Item_13>(*this);
	}
	//============================================= ID_14
	Item_14::Item_14(const Item_14& other)
		: Item(other) , appliedStack(other.appliedStack)
	{
	}
	void Item_14::Init()
	{
		id = 14;
		name = "item_14";
		description = "Increases your attack damage but reduces your movement speed.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		appliedStack = 0;
		tag = SLOTH;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_14::Use(PlayerCharacter* owner)
	{
		global::additionalMovementSpeed -= 30.f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
		global::additionalDamage += 1.f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
	}
	void Item_14::Update(PlayerCharacter* owner)
	{
	}
	void Item_14::Draw()
	{
	}
	std::shared_ptr<Item> Item_14::Clone() const
	{
		return std::make_shared<Item_14>(*this);
	}
	//============================================= ID_15
	Item_15::Item_15(const Item_15& other)
		: Item(other)
	{
	}
	void Item_15::Init()
	{
		id = 15;
		name = "item_15";
		description = "Reduces fire. The damage bonus scales inversely with your fire rate.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = SLOTH;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_15::Use(PlayerCharacter* owner)
	{
		global::additionalFireRateRatio -= 0.1f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);

		if ((owner->Stats.FireRate * owner->GunData->GuntypeFireRateRatio) / owner->Stats.effectiveFireRate > 1)
		{
			global::additionalDamageRatio += ((owner->Stats.FireRate * owner->GunData->GuntypeFireRateRatio) / owner->Stats.effectiveFireRate - 1.f) * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
		}
	}
	void Item_15::Update(PlayerCharacter* owner)
	{
	}
	void Item_15::Draw()
	{
	}
	std::shared_ptr<Item> Item_15::Clone() const
	{
		return std::make_shared<Item_15>(*this);
	}
	//============================================= ID_16
	Item_16::Item_16(const Item_16& other)
		: Item(other), appliedStack(other.appliedStack)
	{
	}
	void Item_16::Init()
	{
		id = 16;
		name = "item_16";
		description = "Grants +2 Max Health. Decreases Movement Speed.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		appliedStack = 0;
		tag = GLUTTONY;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_16::Use(PlayerCharacter* owner)
	{
		if (appliedStack != Utils::GetItemCount(id))
		{
			owner->Stats.MaxHP += 2;
			owner->Stats.HP += 2;
			appliedStack++;
		}

		global::additionalMovementSpeed -= 30.f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
	}
	void Item_16::Update(PlayerCharacter* owner)
	{
	}
	void Item_16::Draw()
	{
	}
	std::shared_ptr<Item> Item_16::Clone() const
	{
		return std::make_shared<Item_16>(*this);
	}
	//============================================= ID_17
	Item_17::Item_17(const Item_17& other)
		: Item(other), targetKillCount(other.targetKillCount), currKillCount(other.currKillCount)
	{
	}
	void Item_17::Init()
	{
		id = 17;
		name = "item_17";
		description = "Restores 1 Health per 80 enemies slain.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		targetKillCount = 8;
		currKillCount = 0;
		tag = GLUTTONY;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_17::Use(PlayerCharacter* owner)
	{
		currKillCount += global::RecentlyDeadEnemyCount;

		if (currKillCount >= targetKillCount * (1.f + (Utils::GetItemCount(id) - 1) * 0.1))
		{
			currKillCount -= targetKillCount * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
			owner->Stats.HP += 1;
		}
	}
	void Item_17::Update(PlayerCharacter* owner)
	{
	}
	void Item_17::Draw()
	{
	}
	std::shared_ptr<Item> Item_17::Clone() const
	{
		return std::make_shared<Item_17>(*this);
	}
	//============================================= ID_18
	Item_18::Item_18(const Item_18& other)
		: Item(other)
	{
	}
	void Item_18::Init()
	{
		id = 18;
		name = "item_18";
		description = "Increases Attack Power and decreases Movement Speed based on your Health.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = GLUTTONY;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_18::Use(PlayerCharacter* owner)
	{
		global::additionalDamage += (0.08f * owner->Stats.HP * (1.f + (Utils::GetItemCount(this->id) - 1) * 0.1f));
		global::additionalMovementSpeed -= (8.f * owner->Stats.HP * (1.f + (Utils::GetItemCount(this->id) - 1) * 0.1f));
	}
	void Item_18::Update(PlayerCharacter* owner)
	{
	}
	void Item_18::Draw()
	{
	}
	std::shared_ptr<Item> Item_18::Clone() const
	{
		return std::make_shared<Item_18>(*this);
	}
	//============================================= ID_19
	Item_19::Item_19(const Item_19& other)
		: Item(other), effectTimer(other.effectTimer), isActivated(other.isActivated)
	{
	}
	void Item_19::Init()
	{
		id = 19;
		name = "item_19";
		description = "On taking damage, gain increased Rate of Fire, Attack Power, and Movement Speed for 5 seconds.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		effectTime = 5.f;
		effectTimer = 0.f;
		isActivated = false;
		tag = GLUTTONY;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_19::Use(PlayerCharacter* owner)
	{
		if (isActivated)
		{
			effectTimer += global::DeltaTime;
			if (effectTimer >= effectTime)
			{
				effectTimer = 0.f;
				isActivated = false;
			}

			global::additionalFireRate += 0.5f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
			global::additionalDamage += 0.5f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
			global::additionalMovementSpeed += 50.f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
		}
	}
	void Item_19::Update(PlayerCharacter* owner)
	{
	}
	void Item_19::Draw()
	{
	}
	std::shared_ptr<Item> Item_19::Clone() const
	{
		return std::make_shared<Item_19>(*this);
	}
	void Item_19::OnDamaged()
	{
		isActivated = true;
	}
	//============================================= ID_20
	Item_20::Item_20(const Item_20& other)
		: SkillEffectItem(other), Potions(other.Potions)
	{
	}
	void Item_20::Init()
	{
		id = 20;
		name = "item_20";
		description = "Every 30 seconds, a potion spawns in a random location. Picking it up restores 1 Health.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = GLUTTONY;
		FireTimer = 0.f;
		AEVec2Set(&effectSize, 100.f, 100.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_20::Use(PlayerCharacter* owner)
	{
		FireTimer += global::DeltaTime;
		if (FireTimer >= 30.f * (1.f - (Utils::GetItemCount(id) - 1) * 0.05))
		{
			FireTimer = 0;
			std::shared_ptr<Actor> newPotion = std::make_shared<Actor>();
			newPotion->size = effectSize;
			newPotion->CollisionRadius = effectSize.x / 2;
			newPotion->position = Utils::GetRandomPointInEllipse(newPotion->CollisionRadius);

			Potions.push_back(newPotion);
		}

		for (auto it = Potions.begin(); it != Potions.end(); )
		{
			const auto& potionPtr = *it;

			if (potionPtr && Utils::CheckCollision(*owner, potionPtr->position, potionPtr->CollisionRadius))
			{
				owner->adjustHealth(1);
				it = Potions.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	void Item_20::Update(PlayerCharacter* owner)
	{
	}
	void Item_20::Draw()
	{
		for (const std::shared_ptr<Actor>& Potion : Potions)
		{
			Utils::DrawTest(Potion->position.x, Potion->position.y, Potion->size.x, Potion->size.y);
		}
	}
	std::shared_ptr<Item> Item_20::Clone() const
	{
		return std::make_shared<Item_20>(*this);
	}
	//============================================= ID_21
	Item_21::Item_21(const Item_21& other)
		: Item(other)
	{
	}
	void Item_21::Init()
	{
		id = 21;
		name = "item_21";
		description = "this is item_21";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = GREED;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_21::Use(PlayerCharacter* owner)
	{
	}
	void Item_21::Update(PlayerCharacter* owner)
	{
	}
	void Item_21::Draw()
	{
	}
	std::shared_ptr<Item> Item_21::Clone() const
	{
		return std::make_shared<Item_21>(*this);
	}
	//============================================= ID_22
	Item_22::Item_22(const Item_22& other)
		: Item(other)
	{
	}
	void Item_22::Init()
	{
		id = 22;
		name = "item_22";
		description = "this is item_22";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = GREED;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_22::Use(PlayerCharacter* owner)
	{
	}
	void Item_22::Update(PlayerCharacter* owner)
	{
	}
	void Item_22::Draw()
	{
	}
	std::shared_ptr<Item> Item_22::Clone() const
	{
		return std::make_shared<Item_22>(*this);
	}
	//============================================= ID_23
	Item_23::Item_23(const Item_23& other)
		: Item(other)
	{
	}
	void Item_23::Init()
	{
		id = 23;
		name = "item_23";
		description = "this is item_23";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = GREED;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_23::Use(PlayerCharacter* owner)
	{
	}
	void Item_23::Update(PlayerCharacter* owner)
	{
	}
	void Item_23::Draw()
	{
	}
	std::shared_ptr<Item> Item_23::Clone() const
	{
		return std::make_shared<Item_23>(*this);
	}
	//============================================= ID_24
	Item_24::Item_24(const Item_24& other)
		: Item(other)
	{
	}
	void Item_24::Init()
	{
		id = 24;
		name = "item_24";
		description = "this is item_24";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = GREED;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_24::Use(PlayerCharacter* owner)
	{
	}
	void Item_24::Update(PlayerCharacter* owner)
	{
	}
	void Item_24::Draw()
	{
	}
	std::shared_ptr<Item> Item_24::Clone() const
	{
		return std::make_shared<Item_24>(*this);
	}
	//============================================= ID_25
	Item_25::Item_25(const Item_25& other)
		: Item(other)
	{
	}
	void Item_25::Init()
	{
		id = 25;
		name = "item_25";
		description = "this is item_25";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = GREED;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_25::Use(PlayerCharacter* owner)
	{
	}
	void Item_25::Update(PlayerCharacter* owner)
	{
	}
	void Item_25::Draw()
	{
	}
	std::shared_ptr<Item> Item_25::Clone() const
	{
		return std::make_shared<Item_25>(*this);
	}
	//============================================= ID_26
	Item_26::Item_26(const Item_26& other)
		: Item(other)
	{
	}
	void Item_26::Init()
	{
		id = 26;
		name = "item_26";
		description = "Attacks have a 20% chance to stun enemies for 3 seconds.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		effectTime = 3.f;
		procChance = 0.2f;
		tag = LUST;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_26::Use(PlayerCharacter* owner)
	{
	}
	void Item_26::Update(PlayerCharacter* owner)
	{
	}
	void Item_26::Draw()
	{
	}
	std::shared_ptr<Item> Item_26::Clone() const
	{
		return std::make_shared<Item_26>(*this);
	}
	void Item_26::OnHit(InGame::EnemyCharacter* target, bool isTargetBoss)
	{
		if (isTargetBoss)
		{
			if (Utils::GetRandomFloat(0.f, 1.f) <= ((procChance + (Utils::GetItemCount(id) - 1) * procChance / 10.f) * global::additionalProcChanceRatio) / 10.f)
				target->Stats.StatusEffectTimer[STUN] = effectTime / 10.f;
		}
		else 
		{
			if (Utils::GetRandomFloat(0.f, 1.f) <= (procChance + (Utils::GetItemCount(id) - 1) * procChance / 10.f) * global::additionalProcChanceRatio)
				target->Stats.StatusEffectTimer[STUN] = effectTime;
		}
	}
	//============================================= ID_27
	Item_27::Item_27(const Item_27& other)
		: SkillEffectItem(other), CoolDown{ other.CoolDown }, isReady{ other.isReady }, isStarted{ other.isStarted }
	{
	}
	void Item_27::Init()
	{
		id = 27;
		name = "item_27";
		description = "On kill, there is a 10% chance to fear nearby enemies for 4 seconds.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		AEVec2Set(&effectSize, 500.f, 500.f);
		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
		isReady = true;
		isStarted = false;
		CoolDown = 0.1f;
		FireTimer = 0.f;
		FrameTime = 0.1f;
		AEVec2Set(&AnimationOffset, 0.f, 0.f);
		AnimationCount = 0;
		AnimationTimer = 0.f;
		effectRow = 1;
		effectColumn = 9;
		procChance = 0.1f;
		effectTime = 4.f;
		MaxAnimationCount = 9;
		tag = LUST;
	}
	void Item_27::Use(PlayerCharacter* owner)
	{
	}
	void Item_27::Update(PlayerCharacter* owner)
	{
		if (!isStarted)
		{
			FireTimer += global::DeltaTime;
			if (FireTimer >= CoolDown)
			{
				FireTimer = 0;
				isReady = true;
			}
		}
		
		f32 effectiveProcChance = (procChance + (Utils::GetItemCount(id) - 1) * procChance / 10.f) * global::additionalProcChanceRatio;
		f32 num = Utils::GetRandomFloat(0.f, 1.f);
		
	
		if (isReady && global::IsEnemyRecentlyDied)
		{
			if (num <= effectiveProcChance)
			{
				effectPosition = global::RecentlyDeadEnemyPosition;
				isReady = false;
				isStarted = true;

				if (Manager::gm.currStateREF)
				{
					Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
					if (GS)
					{
						if (GS->ECs.size() > 0)
						{
							for (size_t i = 0; i < GS->ECs.size(); i++)
							{
								if (Utils::CheckCollision(*GS->ECs[i], effectPosition, effectSize.x / 2))
								{
									GS->ECs[i]->Stats.StatusEffectTimer[FEAR] = effectTime;
								}
							}
						}
					}
				}
			}
			else
			{
				isReady = false;
			}
		}

		if (isStarted)
		{
			Utils::UpdateOffset(*this);
		}
	}
	void Item_27::Draw()
	{
		if (isStarted)
		{
			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->ITRM)
					{
						Utils::DrawObject(*this, GS->ITRM->explosionTexture, GS->ITRM->explosionMesh);
					}
				}
			}

			if (AnimationCount == MaxAnimationCount - 1)
			{
				AnimationCount = 0;
				isStarted = false;
			}
		}
	}
	std::shared_ptr<Item> Item_27::Clone() const
	{
		return std::make_shared<Item_27>(*this);
	}
	//============================================= ID_28
	Item_28::Item_28(const Item_28& other)
		: Item(other)
	{
	}
	void Item_28::Init()
	{
		id = 28;
		name = "item_28";
		description = "Attacks have a 20% chance to make an enemy Vulnerable for 3 seconds.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		effectTime = 3.f;
		procChance = 0.2f;
		tag = LUST;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_28::Use(PlayerCharacter* owner)
	{

	}
	void Item_28::Update(PlayerCharacter* owner)
	{
	}
	void Item_28::Draw()
	{
	}
	std::shared_ptr<Item> Item_28::Clone() const
	{
		return std::make_shared<Item_28>(*this);
	}
	void Item_28::OnHit(InGame::EnemyCharacter* target, bool isTargetBoss)
	{
		if (isTargetBoss)
		{
			if (Utils::GetRandomFloat(0.f, 1.f) <= ((procChance + (Utils::GetItemCount(id) - 1) * procChance / 10.f) * global::additionalProcChanceRatio) / 10.f)
				target->Stats.StatusEffectTimer[VULNERABLE] = effectTime;
		}
		else
		{
			if (Utils::GetRandomFloat(0.f, 1.f) <= (procChance + (Utils::GetItemCount(id) - 1) * procChance / 10.f) * global::additionalProcChanceRatio)
				target->Stats.StatusEffectTimer[VULNERABLE] = effectTime;
		}
	}
	//============================================= ID_29
	Item_29::Item_29(const Item_29& other)
		: Item(other)
	{
	}
	void Item_29::Init()
	{
		id = 29;
		name = "item_29";
		description = "Increases the activation chance of all status effects.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = LUST;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_29::Use(PlayerCharacter* owner)
	{
		global::additionalProcChanceRatio += 0.3f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1);
	}
	void Item_29::Update(PlayerCharacter* owner)
	{
	}
	void Item_29::Draw()
	{
	}
	std::shared_ptr<Item> Item_29::Clone() const
	{
		return std::make_shared<Item_29>(*this);
	}
	//============================================= ID_30
	Item_30::Item_30(const Item_30& other)
		: Item(other)
	{
	}
	void Item_30::Init()
	{
		id = 30;
		name = "item_30";
		description = "Attacks execute enemies that are below 10% of their maximum health.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = LUST;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_30::Use(PlayerCharacter* owner)
	{
	}
	void Item_30::Update(PlayerCharacter* owner)
	{
	}
	void Item_30::Draw()
	{
	}
	std::shared_ptr<Item> Item_30::Clone() const
	{
		return std::make_shared<Item_30>(*this);
	}
	void Item_30::OnHit(InGame::EnemyCharacter* target, bool isTargetBoss)
	{
		if (!isTargetBoss)
		{
			if (target->Stats.HP / target->Stats.MaxHP <= 0.1f * (1.f + (Utils::GetItemCount(id) - 1) * 0.1) && target->Stats.HP > 0.f)
			{
				std::cout << "HP:" << target->Stats.HP << ", MAXHP:" << target->Stats.MaxHP << std::endl;
				target->adjustHealth(-99999999.f);
			}
		}
	}
	//============================================= ID_31
	Item_31::Item_31(const Item_31& other)
		: SkillEffectItem(other), dir{ other.dir }, effectPosition2{ other.effectPosition2 }, distance{ other.distance },
		Damage{ other.Damage }, HitCount{ other.HitCount }, BulletSpeed{ other.BulletSpeed }, FireRate{ other.FireRate },
		effectiveFireRate{ other.effectiveFireRate }, effectiveDamage{ other.effectiveDamage }, effectiveHitCount{ other.effectiveHitCount }
	{
	}
	void Item_31::Init()
	{
		id = 31;
		name = "item_31";
		description = "Summons two minions that fire bullets in the direction you are aiming.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		distance = 80.f;
		Damage = 1;
		FireRate = 5.f;
		FireTimer = 0.f;
		HitCount = 1;
		BulletSpeed = 15.f;
		AEVec2Set(&effectSize, 128.f, 45.f);
		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
		AEVec2Set(&AnimationOffset, 0, 0);
		tag = ENVY;

		dir = global::PlayerMouseDirection;
		AEVec2 DirectionVector;
		AEVec2Scale(&DirectionVector, &dir, distance);

		AEMtx33 rotate;
		AEMtx33Rot(&rotate, AEDegToRad(90.f));
		AEMtx33MultVec(&effectPosition, &rotate, &DirectionVector);
		AEVec2Add(&effectPosition, &effectPosition, &global::PlayerLocation);

		AEMtx33Rot(&rotate, AEDegToRad(-90.f));
		AEMtx33MultVec(&effectPosition2, &rotate, &DirectionVector);
		AEVec2Add(&effectPosition2, &effectPosition2, &global::PlayerLocation);
	}
	void Item_31::Use(PlayerCharacter* owner)
	{
	}
	void Item_31::Update(PlayerCharacter* owner)
	{
		//s16 count = Utils::GetItemCount(3);

		effectiveDamage = (Damage + global::additionalMinionDamage)* global::additionalMinionDamageRatio;
		effectiveFireRate = (FireRate + global::additionalMinionFireRate) * global::additionalMinionFireRateRatio;
		effectiveHitCount = HitCount + global::additionalMinionHitCount;

		dir = global::PlayerMouseDirection; 
		AEVec2 DirectionVector;
		AEVec2Scale(&DirectionVector, &dir, distance);

		AEMtx33 rotate;
		AEMtx33Rot(&rotate, AEDegToRad(90.f));
		AEMtx33MultVec(&effectPosition, &rotate, &DirectionVector);
		AEVec2Add(&effectPosition, &effectPosition, &global::PlayerLocation);

		AEMtx33Rot(&rotate, AEDegToRad(-90.f));
		AEMtx33MultVec(&effectPosition2, &rotate, &DirectionVector);
		AEVec2Add(&effectPosition2, &effectPosition2, &global::PlayerLocation);

		FireTimer += global::DeltaTime;
		if (FireTimer >= 1.f / effectiveFireRate)
		{
			FireTimer = 0.f;
			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->PPPool.size() > 0)
					{
						Projectile* PP1 = GS->PPPool.back();
						GS->PPPool.pop_back();
						PP1->Spawn(dir, effectPosition, BulletSpeed, effectiveDamage, effectiveHitCount, false);
						GS->PPs.push_back(PP1);
						Projectile* PP2 = GS->PPPool.back();
						GS->PPPool.pop_back();
						PP2->Spawn(dir, effectPosition2, BulletSpeed, effectiveDamage, effectiveHitCount, false);
						GS->PPs.push_back(PP2);
					}
				}
			}
		}
	}
	void Item_31::Draw()
	{
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (GS->ITRM)
				{
					Utils::DrawObjectWithDirection(*this, GS->ITRM->minionTexture, GS->ITRM->minionMesh, dir);
					Utils::DrawObjectWithDirection(*this, this->effectPosition2, GS->ITRM->minionTexture, GS->ITRM->minionMesh, dir);
				}
			}
		}
	}
	std::shared_ptr<Item> Item_31::Clone() const
	{
		return std::make_shared<Item_31>(*this);
	}
	//============================================= ID_32
	Item_32::Item_32(const Item_32& other)
		: SkillEffectItem(other), attackDir(other.attackDir), dir{ other.dir }, explosionPosition{ other.explosionPosition }, distance{ other.distance },
		Damage{ other.Damage }, HitCount{ other.HitCount }, BulletSpeed{ other.BulletSpeed }, FireRate{ other.FireRate }, distanceMin(other.distanceMin),
		effectiveFireRate{ other.effectiveFireRate }, effectiveDamage{ other.effectiveDamage }, effectiveHitCount{ other.effectiveHitCount }, closestEnemy(other.closestEnemy)
	{
	}
	void Item_32::Init()
	{
		id = 32;
		name = "item_32";
		description = "Summons a minion that lobs grenades at the nearest enemy.";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		AEVec2Set(&iconPosition, 0.f, 0.f);
		distance = 80.f;
		Damage = 3;
		FireRate = 1.f;
		FireTimer = 0.f;
		HitCount = 1;
		BulletSpeed = 15.f;
		AEVec2Set(&effectSize, 64.f, 64.f);
		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
		AEVec2Set(&AnimationOffset, 0, 0);
		tag = ENVY;

		dir = global::PlayerMouseDirection;
		AEVec2 DirectionVector;
		AEVec2Scale(&DirectionVector, &dir, distance);
		AEVec2Sub(&effectPosition, &global::PlayerLocation, &DirectionVector);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_32::Use(PlayerCharacter* owner)
	{
	}
	void Item_32::Update(PlayerCharacter* owner)
	{
		effectiveDamage = (Damage + global::additionalMinionDamage) * global::additionalMinionDamageRatio;
		effectiveFireRate = (FireRate + global::additionalMinionFireRate) * global::additionalMinionFireRateRatio;
		effectiveHitCount = HitCount;

		dir = global::PlayerMouseDirection;
		AEVec2 DirectionVector;
		AEVec2Scale(&DirectionVector, &dir, distance);
		AEVec2Sub(&effectPosition, &global::PlayerLocation, &DirectionVector);

		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (GS->ECs.size() > 0)
				{
					distanceMin = AEVec2Distance(&effectPosition, &GS->ECs[0]->position);
					closestEnemy = GS->ECs[0];
					for (size_t i = 1; i < GS->ECs.size(); i++)
					{
						if (distanceMin > AEVec2Distance(&effectPosition, &GS->ECs[i]->position))
						{
							distanceMin = AEVec2Distance(&effectPosition, &GS->ECs[i]->position);
							closestEnemy = GS->ECs[i];
						}
					}
					AEVec2Set(&attackDir, closestEnemy->position.x - effectPosition.x, closestEnemy->position.y - effectPosition.y);
					AEVec2Normalize(&attackDir, &attackDir);
				}
			}
		}

		FireTimer += global::DeltaTime;
		if (FireTimer >= 1.f / effectiveFireRate)
		{
			FireTimer = 0.f;
			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->PPPool.size() > 0)
					{
						Projectile* PP1 = GS->PPPool.back();
						GS->PPPool.pop_back();
						PP1->Spawn(attackDir, effectPosition, BulletSpeed, effectiveDamage, effectiveHitCount, true);
						GS->PPs.push_back(PP1);
					}
				}
			}
		}
	}
	void Item_32::Draw()
	{
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (GS->ITRM)
				{
					Utils::DrawObjectWithDirection(*this, GS->ITRM->minionTexture, GS->ITRM->minionMesh, attackDir);
				}
			}
		}
	}
	std::shared_ptr<Item> Item_32::Clone() const
	{
		return std::make_shared<Item_32>(*this);
	}
	//============================================= ID_33
	Item_33::Item_33(const Item_33& other)
		: SkillEffectItem(other), angle(other.angle), angleSpeed(other.angleSpeed), effectiveAngleSpeed(other.effectiveAngleSpeed), dir(other.dir),
		Damage(other.Damage), effectiveDamage(other.effectiveDamage), CoolDown(other.CoolDown), isReady(other.isReady), distance(other.distance)
	{
	}
	void Item_33::Init()
	{
		id = 33;
		name = "item_33";
		description = "this is item_33";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		tag = ENVY;
		Damage = 1.f;
		angle = 0.f;
		angleSpeed = 150.f;
		CoolDown = 0.1f;
		isReady = false;
		distance = 200.f;
		FireTimer = 0.f;
		AEVec2Set(&dir, 0.f, 1.f);
		AEVec2Set(&effectSize, 96.f, 96.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_33::Use(PlayerCharacter* owner)
	{
	}
	void Item_33::Update(PlayerCharacter* owner)
	{
		effectiveDamage = (Damage + global::additionalMinionDamage) * global::additionalMinionDamageRatio;
		effectiveAngleSpeed = (angleSpeed + global::additionalMinionFireRate) * global::additionalMinionFireRateRatio;

		angle -= effectiveAngleSpeed * global::DeltaTime;

		while (angle <= 0.f)
		{
			angle += 360.f;
		}

		FireTimer += global::DeltaTime;
		if (FireTimer >= CoolDown)
		{
			FireTimer = 0.f;
			isReady = true;
		}

		AEVec2 DirectionVector;
		AEVec2Scale(&DirectionVector, &dir, distance);

		AEMtx33 rotate;
		AEMtx33Rot(&rotate, AEDegToRad(angle));
		AEMtx33MultVec(&effectPosition, &rotate, &DirectionVector);
		AEVec2Add(&effectPosition, &effectPosition, &global::PlayerLocation);

		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (GS->EPs.size() > 0)
				{
					for (InGame::Projectile*& EP : GS->EPs)
					{
						if (Utils::CheckCollision(*EP, effectPosition, effectSize.x / 2.f))
						{
							EP->bIsPandingKill = true;
						}
					}
				}
				if (isReady)
				{
					if (GS->ECs.size() > 0)
					{
						for (size_t i = 0; i < GS->ECs.size(); i++)
						{
							if (Utils::CheckCollision(*GS->ECs[i], effectPosition, effectSize.x / 2))
							{
								GS->ECs[i]->adjustHealth(-effectiveDamage);
							}
						}
					}
					isReady = false;
				}
			}
		}
	}
	void Item_33::Draw()
	{

		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (GS->ITRM)
				{
					Utils::DrawObject(*this, GS->ITRM->minionTexture, GS->ITRM->minionMesh);
				}
			}
		}
	}
	std::shared_ptr<Item> Item_33::Clone() const
	{
		return std::make_shared<Item_33>(*this);
	}
	//============================================= ID_34
	Item_34::Item_34(const Item_34& other)
		: Item(other)
	{
	}
	void Item_34::Init()
	{
		id = 34;
		name = "item_34";
		description = "summoner attack speed faster";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
		tag = ENVY;
	}
	void Item_34::Use(PlayerCharacter* owner)
	{
		global::additionalMinionDamage += 1.f * (1.f + (Utils::GetItemCount(this->id) - 1) * 0.1f);
	}
	void Item_34::Update(PlayerCharacter* owner)
	{
	}
	void Item_34::Draw()
	{
	}
	std::shared_ptr<Item> Item_34::Clone() const
	{
		return std::make_shared<Item_34>(*this);
	}
	//============================================= ID_35
	Item_35::Item_35(const Item_35& other)
		: Item(other)
	{
	}
	void Item_35::Init()
	{
		id = 35;
		name = "item_35";
		description = "summoner attack speed faster";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
		tag = ENVY;
	}
	void Item_35::Use(PlayerCharacter* owner)
	{
		global::additionalMinionFireRate += 5.f * (1.f + (Utils::GetItemCount(this->id) - 1) * 0.1f);
	}
	void Item_35::Update(PlayerCharacter* owner)
	{
	}
	void Item_35::Draw()
	{
	}
	std::shared_ptr<Item> Item_35::Clone() const
	{
		return std::make_shared<Item_35>(*this);
	}
	
}