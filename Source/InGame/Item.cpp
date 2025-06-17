#include "Item.h"
#include "PlayerCharacter.h"
#include <iostream>

namespace InGame
{
	void Item::Init(){}
	void Item::Use(Actor* owner){}
	void Item::Draw() {}
	void Item::Destroy() {}


	void Item_1::Init()
	{
		id = 1;
		name = "item_1";
		description = "this is item_1";
		//Mesh = nullptr;
		//Texture = nullptr;
		AEVec2Set(&size, 16.f, 16.f);
		//row, column;
		//AEVec2 offset;
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
	void Item_1::Destroy() {}
	std::unique_ptr<Item> Item_1::Clone() const
	{
		return std::make_unique<Item_1>(*this);
	}
}