#pragma once
#include "AEEngine.h"
#include "Actor.h"
#include "EnemyCharacter.h"
#include <string>
#include <map>
#include <memory>
#include <vector>

namespace InGame
{
	enum ItemTag
	{
		EMPTY,
		ENVY,
		GLUTTONY,
		GREED,
		LUST,
		SLOTH,
		WRATH,
		PRIDE,
	};
	class Item
	{
	public:
		Item() = default;
		Item(const Item& other);

		virtual void Init() = 0;
		virtual void Use(class PlayerCharacter* owner) = 0;
		virtual void Update(class PlayerCharacter* owner) = 0;
		void DrawIcon();
		virtual void Draw() = 0;
		virtual std::shared_ptr<Item> Clone() const = 0;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) = 0;
		virtual void OnDamaged() = 0;

		static void StaticInit();

		static AEVec2 size;
		static s32 row, column;

		int id;
		std::string name;
		std::string description;
		AEVec2 iconPosition;
		AEVec2 iconOffset;
		ItemTag tag;
		f32 effectTime;
		f32 procChance;
	};
	class SkillEffectItem : public Item
	{
	public:
		SkillEffectItem() = default;
		SkillEffectItem(const SkillEffectItem& other);

		virtual std::shared_ptr<Item> Clone() const override = 0;

		f32 FrameTime;
		f32 FireTimer;
		AEVec2 AnimationOffset;
		s32 AnimationCount;
		s32 MaxAnimationCount;
		f32 AnimationTimer;
		AEVec2 effectPosition;
		AEVec2 effectSize;
		s32 effectRow, effectColumn;
	};
	//============================================= ID_1
	class Item_1 : public Item // to do change class name
	{
	public:
		Item_1() = default;
		Item_1(const Item_1& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_2
	class Item_2 : public Item // to do change class name
	{
	public:
		Item_2() = default;
		Item_2(const Item_2& other);

		AEVec2 prev_PlayerPos;
		f32 triggerTimer;
		f32 stackTimer;
		s32 stacks;
		bool isActivated;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_3
	class Item_3 : public Item // to do change class name
	{
	public:
		Item_3() = default;
		Item_3(const Item_3& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_4
	class Item_4 : public SkillEffectItem // to do change class name
	{
	public:
		Item_4() = default;
		Item_4(const Item_4& other);

		s32 appliedStack;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_5
	class Item_5 : public Item // to do change class name
	{
	public:
		Item_5() = default;
		Item_5(const Item_5& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_6
	class Item_6 : public Item // to do change class name
	{
	public:
		Item_6() = default;
		Item_6(const Item_6& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss);
		virtual void OnDamaged() {}
	};
	//============================================= ID_7
	class Item_7 : public Item // to do change class name
	{
	public:
		Item_7() = default;
		Item_7(const Item_7& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_8
	class Item_8 : public Item // to do change class name
	{
	public:
		Item_8() = default;
		Item_8(const Item_8& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_9
	class Item_9 : public SkillEffectItem // to do change class name
	{
	public:
		Item_9() = default;
		Item_9(const Item_9& other);

		f32 CoolDown;
		bool isStarted;
		bool isReady;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged();
	};
	//============================================= ID_10
	class Item_10 : public Item // to do change class name
	{
	public:
		Item_10() = default;
		Item_10(const Item_10& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_11
	class Item_11 : public SkillEffectItem // to do change class name
	{
	public:
		Item_11() = default;
		Item_11(const Item_11& other);

		f32 CoolDown;
		bool isStarted;
		bool isReady;
		AEVec2 baseEffectSize;
		f32 additionalEffectSizeRatio;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_12
	class Item_12 : public Item // to do change class name
	{
	public:
		Item_12() = default;
		Item_12(const Item_12& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_13
	class Item_13 : public SkillEffectItem // to do change class name
	{
	public:
		Item_13() = default;
		Item_13(const Item_13& other);

		f32 CoolDown;
		bool isReady;
		bool isStarted;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_14
	class Item_14 : public Item // to do change class name
	{
	public:
		Item_14() = default;
		Item_14(const Item_14& other);

		s32 appliedStack;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_15
	class Item_15 : public Item // to do change class name
	{
	public:
		Item_15() = default;
		Item_15(const Item_15& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_16
	class Item_16 : public Item // to do change class name
	{
	public:
		Item_16() = default;
		Item_16(const Item_16& other);

		s32 appliedStack;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_17
	class Item_17 : public Item // to do change class name
	{
	public:
		Item_17() = default;
		Item_17(const Item_17& other);

		s32 targetKillCount;
		s32 currKillCount;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_18
	class Item_18 : public Item // to do change class name
	{
	public:
		Item_18() = default;
		Item_18(const Item_18& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_19
	class Item_19 : public Item // to do change class name
	{
	public:
		Item_19() = default;
		Item_19(const Item_19& other);

		f32 effectTimer;
		bool isActivated;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged();
	};
	//============================================= ID_20
	class Item_20 : public SkillEffectItem // to do change class name
	{
	public:
		Item_20() = default;
		Item_20(const Item_20& other);

		std::vector<std::shared_ptr<Actor>> Potions;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_21
	class Item_21 : public Item // to do change class name
	{
	public:
		Item_21() = default;
		Item_21(const Item_21& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_22
	class Item_22 : public Item // to do change class name
	{
	public:
		Item_22() = default;
		Item_22(const Item_22& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_23
	class Item_23 : public Item // to do change class name
	{
	public:
		Item_23() = default;
		Item_23(const Item_23& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_24
	class Item_24 : public Item // to do change class name
	{
	public:
		Item_24() = default;
		Item_24(const Item_24& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_25
	class Item_25 : public Item // to do change class name
	{
	public:
		Item_25() = default;
		Item_25(const Item_25& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_26
	class Item_26 : public Item // to do change class name
	{
	public:
		Item_26() = default;
		Item_26(const Item_26& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss);
		virtual void OnDamaged() {}
	};
	//============================================= ID_27
	class Item_27 : public SkillEffectItem // to do change class name
	{
	public:
		Item_27() = default;
		Item_27(const Item_27& other);

		f32 CoolDown;
		bool isReady;
		bool isStarted;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_28
	class Item_28 : public Item // to do change class name
	{
	public:
		Item_28() = default;
		Item_28(const Item_28& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss);
		virtual void OnDamaged() {}
	};
	//============================================= ID_29
	class Item_29 : public Item // to do change class name
	{
	public:
		Item_29() = default;
		Item_29(const Item_29& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_30
	class Item_30 : public Item // to do change class name
	{
	public:
		Item_30() = default;
		Item_30(const Item_30& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss);
		virtual void OnDamaged() {}
	};
	//============================================= ID_31
	class Item_31 : public SkillEffectItem // to do change class name
	{
	public:
		Item_31() = default;
		Item_31(const Item_31& other);

		AEVec2 dir;
		f32 distance;
		AEVec2 effectPosition2;

		f32 FireRate;
		f32 Damage;
		s32 HitCount;
		f32 BulletSpeed;
		f32 effectiveFireRate;
		f32 effectiveDamage;
		s32 effectiveHitCount;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_32
	class Item_32 : public SkillEffectItem // to do change class name
	{
	public:
		Item_32() = default;
		Item_32(const Item_32& other);

		AEVec2 dir;
		AEVec2 attackDir;
		f32 distance;
		f32 distanceMin;
		AEVec2 explosionPosition;
		class EnemyCharacter* closestEnemy;

		f32 FireRate;
		f32 Damage;
		s32 HitCount;
		f32 BulletSpeed;
		f32 effectiveFireRate;
		f32 effectiveDamage;
		s32 effectiveHitCount;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_33
	class Item_33 : public SkillEffectItem // to do change class name
	{
	public:
		Item_33() = default;
		Item_33(const Item_33& other);

		AEVec2 dir;
		f32 angle;
		f32 angleSpeed;
		f32 effectiveAngleSpeed;
		f32 Damage;
		f32 effectiveDamage;
		f32 CoolDown;
		bool isReady;
		f32 distance;

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_34
	class Item_34 : public Item // to do change class name
	{
	public:
		Item_34() = default;
		Item_34(const Item_34& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
	//============================================= ID_35
	class Item_35 : public Item // to do change class name
	{
	public:
		Item_35() = default;
		Item_35(const Item_35& other);

		virtual void Init() override;
		virtual void Use(class PlayerCharacter* owner) override;
		virtual void Update(class PlayerCharacter* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
		virtual void OnHit(InGame::EnemyCharacter* target, bool isTargetBoss) {}
		virtual void OnDamaged() {}
	};
}
