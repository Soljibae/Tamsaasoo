#pragma once
#include "AEEngine.h"
#include "Actor.h"
#include <string>
#include <map>
#include <memory>

namespace InGame
{
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
		static void StaticDestroy();

		static AEGfxTexture* itemIconTexture;
		static AEGfxVertexList* itemIconMesh;
		static AEVec2 size;
		static s32 row, column;

		int id;
		std::string name;
		std::string description;
		AEVec2 position;
		AEVec2 offset;
		bool bHasBeenUsed = false;
	};

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
}
