#include "Item.h"
#include "PlayerCharacter.h"
#include "../Utils/Utils.h"
#include <iostream>

namespace InGame
{
	AEGfxTexture* Item::itemTexture = nullptr;
	AEGfxVertexList* Item::itemMesh = nullptr;
	AEVec2 Item::size;
	s32 Item::row = 1, Item::column = 3;

	Item::Item(const Item& other)
		: id(other.id), name(other.name), description(other.description),
		position(other.position), offset(other.offset), bHasBeenUsed(other.bHasBeenUsed)
	{
	}

	void Item::Draw()
	{
		Utils::DrawItem(*this);
	}

	void Item::StaticInit()
	{
		itemTexture = AEGfxTextureLoad("Assets/Item.png");

		itemMesh = Utils::CreateMesh(1, 3);

		AEVec2Set(&size, 64.f, 64.f);
	}
	void Item::StaticDestroy()
	{
		if (itemTexture)
		{
			AEGfxTextureUnload(itemTexture);
			itemTexture = nullptr;
		}
			
		Utils::DestroyMesh(itemMesh);
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
	std::unique_ptr<Item> Item_1::Clone() const
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
		description = "this is item_1";
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
	std::unique_ptr<Item> Item_2::Clone() const
	{
		return std::make_unique<Item_2>(*this);
	}
}