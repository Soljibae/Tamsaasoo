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

	void Item::Draw()
	{
		Utils::DrawItem(*this);
	}

	void Item::StaticInit()
	{
		itemTexture = AEGfxTextureLoad("Assets/Character.png");

		itemMesh = Utils::CreateMesh();

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

	void Item_1::Init()
	{
		id = 1;
		name = "item_1";
		description = "this is item_1";
		AEVec2 offset;
		AEVec2Set(&position, 0.f, 0.f);

		offset.x = 1.f / static_cast<f32>(column) * static_cast<f32>((id - 1) % column);
		offset.y = 1.f - 1.f / static_cast<f32>(row) * static_cast<f32>((id - 1) / column + 1);
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
}