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
	s32 Item::row = 7, Item::column = 3;

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
		effectRow(other.effectRow), effectColumn(other.effectColumn)
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
		: SkillEffectItem(other), CoolDown{ other.CoolDown }, isReady{ other.isReady }, isStarted{ other.isStarted }, Damage{ other.Damage }
	{
	}
	void Item_4::Init()
	{
		id = 4;
		name = "item_4";
		description = "this is item_4";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		AEVec2Set(&effectSize, 400.f, 400.f);
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
		Damage = 1;
		effectRow = 1;
		effectColumn = 9;
		tag = EMPTY;
	}
	void Item_4::Use(PlayerCharacter* owner)
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
								GS->ECs[i]->adjustHealth(-Damage);
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
	void Item_4::Update(PlayerCharacter* owner)
	{
	}
	void Item_4::Draw()
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

			if (AnimationCount == effectColumn - 1)
			{
				AnimationCount = 0;
				isStarted = false;
			}
		}
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
		description = "this is item_5";
		AEVec2Set(&iconPosition, 0.f, 0.f);
		effectTime = 3.f;
		procChance = 0.2f;
		tag = EMPTY;

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_5::Use(PlayerCharacter* owner)
	{
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
	void Item_5::OnHit(InGame::EnemyCharacter* target)
	{
		if (Utils::GetRandomFloat(0.f, 1.f) <= (procChance + (Utils::GetItemCount(id) - 1) * procChance / 10.f) * global::additionalProcChanceRatio)
			target->Stats.StatusEffectTimer[SLOW] = effectTime;
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
		description = "this is item_6";
		AEVec2Set(&iconPosition, 0.f, 0.f);

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
	//============================================= ID_7
	Item_7::Item_7(const Item_7& other)
		: Item(other)
	{
	}
	void Item_7::Init()
	{
		id = 7;
		name = "item_7";
		description = "this is item_7";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_7::Use(PlayerCharacter* owner)
	{
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
		description = "this is item_8";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_8::Use(PlayerCharacter* owner)
	{
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
		: Item(other)
	{
	}
	void Item_9::Init()
	{
		id = 9;
		name = "item_9";
		description = "this is item_9";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_9::Use(PlayerCharacter* owner)
	{
	}
	void Item_9::Update(PlayerCharacter* owner)
	{
	}
	void Item_9::Draw()
	{
	}
	std::shared_ptr<Item> Item_9::Clone() const
	{
		return std::make_shared<Item_9>(*this);
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
		description = "this is item_10";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_10::Use(PlayerCharacter* owner)
	{
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
		: Item(other)
	{
	}
	void Item_11::Init()
	{
		id = 11;
		name = "item_11";
		description = "this is item_11";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_11::Use(PlayerCharacter* owner)
	{
	}
	void Item_11::Update(PlayerCharacter* owner)
	{
	}
	void Item_11::Draw()
	{
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
		description = "this is item_12";
		AEVec2Set(&iconPosition, 0.f, 0.f);

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
		: Item(other)
	{
	}
	void Item_13::Init()
	{
		id = 13;
		name = "item_13";
		description = "this is item_13";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_13::Use(PlayerCharacter* owner)
	{
	}
	void Item_13::Update(PlayerCharacter* owner)
	{
	}
	void Item_13::Draw()
	{
	}
	std::shared_ptr<Item> Item_13::Clone() const
	{
		return std::make_shared<Item_13>(*this);
	}
	//============================================= ID_14
	Item_14::Item_14(const Item_14& other)
		: Item(other)
	{
	}
	void Item_14::Init()
	{
		id = 14;
		name = "item_14";
		description = "this is item_14";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_14::Use(PlayerCharacter* owner)
	{
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
		description = "this is item_15";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_15::Use(PlayerCharacter* owner)
	{
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
		: Item(other)
	{
	}
	void Item_16::Init()
	{
		id = 16;
		name = "item_16";
		description = "this is item_16";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_16::Use(PlayerCharacter* owner)
	{
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
		: Item(other)
	{
	}
	void Item_17::Init()
	{
		id = 17;
		name = "item_17";
		description = "this is item_17";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_17::Use(PlayerCharacter* owner)
	{
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
		description = "this is item_18";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_18::Use(PlayerCharacter* owner)
	{
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
		: Item(other)
	{
	}
	void Item_19::Init()
	{
		id = 19;
		name = "item_19";
		description = "this is item_19";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_19::Use(PlayerCharacter* owner)
	{
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
	//============================================= ID_20
	Item_20::Item_20(const Item_20& other)
		: Item(other)
	{
	}
	void Item_20::Init()
	{
		id = 20;
		name = "item_20";
		description = "this is item_20";
		AEVec2Set(&iconPosition, 0.f, 0.f);

		iconOffset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		iconOffset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_20::Use(PlayerCharacter* owner)
	{
	}
	void Item_20::Update(PlayerCharacter* owner)
	{
	}
	void Item_20::Draw()
	{
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
		description = "this is item_31";
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
		tag = EMPTY;
	}
	void Item_31::Use(PlayerCharacter* owner)
	{
	}
	void Item_31::Update(PlayerCharacter* owner)
	{
		//s16 count = Utils::GetItemCount(3);

		effectiveDamage = (Damage + global::additionalMinionDamage) * global::additionalMinionDamageRatio;
		effectiveFireRate = (FireRate + global::additionalMinionFireRate) * global::additionalMinionFireRateRatio;
		effectiveHitCount = HitCount + global::additionalMinionHitCount;

		std::cout << effectiveDamage << std::endl;

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
						PP1->Spawn(dir, effectPosition, BulletSpeed, effectiveDamage, effectiveHitCount);
						GS->PPs.push_back(PP1);
						Projectile* PP2 = GS->PPPool.back();
						GS->PPPool.pop_back();
						PP2->Spawn(dir, effectPosition2, BulletSpeed, effectiveDamage, effectiveHitCount);
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
}