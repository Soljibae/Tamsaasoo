#include "Item.h"
#include "PlayerCharacter.h"
#include "../Utils/Utils.h"
#include <iostream>
#include "../Global/GlobalVariables.h"
#include "../Manager/Playing.h"
#include "../Manager/GameManager.h"

namespace InGame
{
	AEGfxTexture* Item::itemIconTexture = nullptr;
	AEGfxVertexList* Item::itemIconMesh = nullptr;
	AEVec2 Item::size;
	s32 Item::row = 7, Item::column = 3;

	Item::Item(const Item& other)
		: id(other.id), name(other.name), description(other.description),
		position(other.position), offset(other.offset), bHasBeenUsed(other.bHasBeenUsed)
	{
	}

	void Item::DrawIcon()
	{
		Utils::DrawItem(*this);
	}

	void Item::StaticInit()
	{
		itemIconTexture = AEGfxTextureLoad("Assets/Item.png");

		itemIconMesh = Utils::CreateMesh(row, column);

		AEVec2Set(&size, 64.f, 64.f);
	}
	void Item::StaticDestroy()
	{
		if (itemIconTexture)
		{
			AEGfxTextureUnload(itemIconTexture);
			itemIconTexture = nullptr;
		}

		Utils::DestroyMesh(itemIconMesh);
	}
	//============================================= ID_1
	Item_1::Item_1(const Item_1& other)
		: Item(other)
	{
	}
	void Item_1::Init()
	{
		id = 1;
		name = "item_1";
		description = "this is item_1";
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_1::Use(Actor* owner)
	{
		if (!bHasBeenUsed)
		{
			PlayerCharacter* player = dynamic_cast<PlayerCharacter*>(owner);

			if (player)
			{
				player->Stats.FireRate *= 2.f;
				bHasBeenUsed = true;
			}
		}
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
		: Item(other)
	{
	}
	void Item_2::Init()
	{
		id = 2;
		name = "item_2";
		description = "this is item_2";
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_2::Use(Actor* owner)
	{
		if (!bHasBeenUsed)
		{
			PlayerCharacter* player = dynamic_cast<PlayerCharacter*>(owner);

			if (player)
			{
				player->Stats.BulletSpeed *= 2.f;
				bHasBeenUsed = true;
			}
		}
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
		: Item(other), dir{ other.dir }, pos1{ other.pos1 }, pos2{ other.pos2 }, distance{ other.distance },
		Damage{ other.Damage }, HitCount{ other.HitCount }, BulletSpeed{ other.BulletSpeed }, objectSize{ other.objectSize },
		FireRate{ other.FireRate }, FireTimer{ other.FireTimer }
	{
	}
	void Item_3::Init()
	{
		id = 3;
		name = "item_3";
		description = "this is item_3";
		AEVec2Set(&position, 0.f, 0.f);
		distance = 80.f;
		Damage = 1;
		FireRate = 5.f;
		FireTimer = 0.f;
		HitCount = 1;
		BulletSpeed = 15.f;
		AEVec2Set(&objectSize, 40.f, 40.f);
		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_3::Use(Actor* owner)
	{
		s16 count = Utils::GetItemCount(3);

		dir = global::PlayerMouseDirection;
		AEVec2 DirectionVector;
		AEVec2Scale(&DirectionVector, &dir, distance);

		AEMtx33 rotate;
		AEMtx33Rot(&rotate, AEDegToRad(90.f));
		AEMtx33MultVec(&pos1, &rotate, &DirectionVector);
		AEVec2Add(&pos1, &pos1, &global::PlayerLocation);

		AEMtx33Rot(&rotate, AEDegToRad(-90.f));
		AEMtx33MultVec(&pos2, &rotate, &DirectionVector);
		AEVec2Add(&pos2, &pos2, &global::PlayerLocation);

		FireTimer += global::DeltaTime;
		if (FireTimer >= 1.f / FireRate)
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
						PP1->Spawn(dir, pos1, BulletSpeed, Damage);
						GS->PPs.push_back(PP1);
						Projectile* PP2 = GS->PPPool.back();
						GS->PPPool.pop_back();
						PP2->Spawn(dir, pos2, BulletSpeed, Damage);
						GS->PPs.push_back(PP2);
					}
				}
			}
		}
	}
	void Item_3::Draw()
	{
		Utils::DrawTest(pos1.x, pos1.y, objectSize.x, objectSize.y);
		Utils::DrawTest(pos2.x, pos2.y, objectSize.x, objectSize.y);
	}
	std::shared_ptr<Item> Item_3::Clone() const
	{
		return std::make_shared<Item_3>(*this);
	}
	//============================================= ID_4
	Item_4::Item_4(const Item_4& other)
		: Item(other)
	{
	}
	void Item_4::Init()
	{
		id = 4;
		name = "item_4";
		description = "this is item_4";
		AEVec2Set(&position, 0.f, 0.f);
		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_4::Use(Actor* owner)
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
		description = "this is item_5";
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_5::Use(Actor* owner)
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
		description = "this is item_6";
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_6::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_7::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_8::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_9::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_10::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_11::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_12::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_13::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_14::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_15::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_16::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_17::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_18::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_19::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_20::Use(Actor* owner)
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
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = (1.f / static_cast<f32>(column)) * static_cast<f32>((id - 1) % column);
		offset.y = (1.f / static_cast<f32>(row)) * static_cast<f32>((id - 1) / column);
	}
	void Item_21::Use(Actor* owner)
	{
	}
	void Item_21::Draw()
	{
	}
	std::shared_ptr<Item> Item_21::Clone() const
	{
		return std::make_shared<Item_21>(*this);
	}
}