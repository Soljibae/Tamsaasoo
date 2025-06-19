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
	s32 Item::row = 1, Item::column = 3;

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
		itemIconTexture = AEGfxTextureLoad("Assets/Character.png");

		itemIconMesh = Utils::CreateMesh(1, 3);

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
	void Item_1::Draw() {}
	std::shared_ptr<Item> Item_1::Clone() const
	{
		return std::make_unique<Item_1>(*this);
	}

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
	void Item_2::Draw() {}
	std::shared_ptr<Item> Item_2::Clone() const
	{
		return std::make_unique<Item_2>(*this);
	}

	Item_3::Item_3(const Item_3& other)
		: Item(other), dir{ other.dir }, pos1{ other.pos1 }, pos2{ other.pos2 }, distance{ other.distance },
		Damage{ other.Damage }, HitCount{ other.HitCount }, BulletSpeed{ other.BulletSpeed }, objectSize{ other.objectSize },
		FireRate{ other.FireRate } ,FireTimer{ other.FireTimer }
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
		return std::make_unique<Item_3>(*this);
	}
}