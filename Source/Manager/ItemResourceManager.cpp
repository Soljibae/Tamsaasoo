#include "ItemResourceManager.h"
#include "../Utils/Utils.h"

namespace Manager
{
	void ItemResourceManager::Init()
	{
		itemIconTexture = AEGfxTextureLoad("Assets/Item.png");
		itemIconMesh = Utils::CreateMesh(7, 5);
		
		explosionTexture = AEGfxTextureLoad("Assets/explosion.png");
		explosionMesh = Utils::CreateMesh(1, 9);

		item9Texture = AEGfxTextureLoad("Assets/fire.png");
		item9Mesh = Utils::CreateMesh(4, 4);

		item31minionTexture = AEGfxTextureLoad("Assets/item_minion_0.png");
		item31minionMesh = Utils::CreateMesh();

		item32minionTexture = AEGfxTextureLoad("Assets/item_minion_1.png");
		item32minionMesh = Utils::CreateMesh();

		item33minionTexture = AEGfxTextureLoad("Assets/item_minion_2.png");
		item33minionMesh = Utils::CreateMesh();
	}

	void ItemResourceManager::Destroy()
	{
		if (itemIconTexture)
		{
			AEGfxTextureUnload(itemIconTexture);
			itemIconTexture = nullptr;
		}

		Utils::DestroyMesh(itemIconMesh);

		if (explosionTexture)
		{
			AEGfxTextureUnload(explosionTexture);
			explosionTexture = nullptr;
		}

		Utils::DestroyMesh(explosionMesh);

		if (item9Texture)
		{
			AEGfxTextureUnload(item9Texture);
			item9Texture = nullptr;
		}

		Utils::DestroyMesh(item9Mesh);

		if (item31minionTexture)
		{
			AEGfxTextureUnload(item31minionTexture);
			item31minionTexture = nullptr;
		}

		Utils::DestroyMesh(item31minionMesh);

		if (item32minionTexture)
		{
			AEGfxTextureUnload(item32minionTexture);
			item32minionTexture = nullptr;
		}

		Utils::DestroyMesh(item32minionMesh);

		if (item33minionTexture)
		{
			AEGfxTextureUnload(item33minionTexture);
			item32minionTexture = nullptr;
		}

		Utils::DestroyMesh(item33minionMesh);
	}
}