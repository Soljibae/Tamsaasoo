#pragma once
#include "AEEngine.h"
#include "Actor.h"
#include <string>
#include <map>
#include <memory>

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
		virtual void Use(Actor* owner) = 0;
		void DrawIcon();
		virtual void Draw() = 0;
		virtual std::shared_ptr<Item> Clone() const = 0;

		static void StaticInit();

		static AEVec2 size;
		static s32 row, column;

		int id;
		std::string name;
		std::string description;
		AEVec2 position;
		AEVec2 offset;
		bool bHasBeenUsed = false;
	};
	//============================================= ID_1
	class Item_1 : public Item // to do change class name
	{
	public:
		Item_1() = default;
		Item_1(const Item_1& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_2
	class Item_2 : public Item // to do change class name
	{
	public:
		Item_2() = default;
		Item_2(const Item_2& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_3
	class Item_3 : public Item // to do change class name
	{
	public:
		Item_3() = default;
		Item_3(const Item_3& other);

		AEVec2 dir;
		AEVec2 pos1;
		AEVec2 pos2;
		f32 distance;
		s32 Damage;
		f32 FireRate;
		f32 FireTimer;
		s32 HitCount;
		s32 BulletSpeed;
		AEVec2 objectSize;

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_4
	class Item_4 : public Item // to do change class name
	{
	public:
		Item_4() = default;
		Item_4(const Item_4& other);
		f32 CoolDown;
		f32 FireTimer;
		AEVec2 pos;
		AEVec2 explodeSize;
		bool isReady;
		bool isStarted;
		f32 FrameTime;
		AEVec2 AnimationOffset;
		s32 AnimationCount;
		f32 AnimationTimer;
		s32 Damage;

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_5
	class Item_5 : public Item // to do change class name
	{
	public:
		Item_5() = default;
		Item_5(const Item_5& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_6
	class Item_6 : public Item // to do change class name
	{
	public:
		Item_6() = default;
		Item_6(const Item_6& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_7
	class Item_7 : public Item // to do change class name
	{
	public:
		Item_7() = default;
		Item_7(const Item_7& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_8
	class Item_8 : public Item // to do change class name
	{
	public:
		Item_8() = default;
		Item_8(const Item_8& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_9
	class Item_9 : public Item // to do change class name
	{
	public:
		Item_9() = default;
		Item_9(const Item_9& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_10
	class Item_10 : public Item // to do change class name
	{
	public:
		Item_10() = default;
		Item_10(const Item_10& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_11
	class Item_11 : public Item // to do change class name
	{
	public:
		Item_11() = default;
		Item_11(const Item_11& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_12
	class Item_12 : public Item // to do change class name
	{
	public:
		Item_12() = default;
		Item_12(const Item_12& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_13
	class Item_13 : public Item // to do change class name
	{
	public:
		Item_13() = default;
		Item_13(const Item_13& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_14
	class Item_14 : public Item // to do change class name
	{
	public:
		Item_14() = default;
		Item_14(const Item_14& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_15
	class Item_15 : public Item // to do change class name
	{
	public:
		Item_15() = default;
		Item_15(const Item_15& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_16
	class Item_16 : public Item // to do change class name
	{
	public:
		Item_16() = default;
		Item_16(const Item_16& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_17
	class Item_17 : public Item // to do change class name
	{
	public:
		Item_17() = default;
		Item_17(const Item_17& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_18
	class Item_18 : public Item // to do change class name
	{
	public:
		Item_18() = default;
		Item_18(const Item_18& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_19
	class Item_19 : public Item // to do change class name
	{
	public:
		Item_19() = default;
		Item_19(const Item_19& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_20
	class Item_20 : public Item // to do change class name
	{
	public:
		Item_20() = default;
		Item_20(const Item_20& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
	//============================================= ID_21
	class Item_21 : public Item // to do change class name
	{
	public:
		Item_21() = default;
		Item_21(const Item_21& other);

		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual std::shared_ptr<Item> Clone() const override;
	};
}
