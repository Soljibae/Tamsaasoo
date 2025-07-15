#include "ItemResourceManager.h"
#include "../Utils/Utils.h"

namespace Manager
{
	void ItemResourceManager::Init()
	{
		itemIconTexture = AEGfxTextureLoad("Assets/Item.png");
		itemIconMesh = Utils::CreateMesh(9, 5);
		
		explosionTexture = AEGfxTextureLoad("Assets/explosion.png");
		explosionMesh = Utils::CreateMesh(1, 9);

		item9Texture = AEGfxTextureLoad("Assets/fire.png");
		item9Mesh = Utils::CreateMesh(4, 4);

		item11Texture = AEGfxTextureLoad("Assets/burst.png");
		item11Mesh = Utils::CreateMesh(4, 5);

		item20Texture = AEGfxTextureLoad("Assets/itemEffectPotion.png");
		item20Mesh = Utils::CreateMesh(1, 1);

		item27Texture = AEGfxTextureLoad("Assets/fear.png");
		item27Mesh = Utils::CreateMesh(5, 6);

		item30Texture = AEGfxTextureLoad("Assets/excute.png");
		item30Mesh = Utils::CreateMesh(1, 9);

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

		if (item11Texture)
		{
			AEGfxTextureUnload(item11Texture);
			item11Texture = nullptr;
		}

		Utils::DestroyMesh(item11Mesh);

		if (item20Texture)
		{
			AEGfxTextureUnload(item20Texture);
			item20Texture = nullptr;
		}

		Utils::DestroyMesh(item20Mesh);

		if (item27Texture)
		{
			AEGfxTextureUnload(item27Texture);
			item27Texture = nullptr;
		}

		Utils::DestroyMesh(item27Mesh);

		if (item30Texture)
		{
			AEGfxTextureUnload(item30Texture);
			item30Texture = nullptr;
		}

		Utils::DestroyMesh(item30Mesh);

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