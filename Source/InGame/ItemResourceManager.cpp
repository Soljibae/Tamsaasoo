#include "ItemResourceManager.h"
#include "../Utils/Utils.h"

namespace InGame
{
	void ItemResourceManager::Init()
	{
		itemIconTexture = AEGfxTextureLoad("Assets/Item.png");
		itemIconMesh = Utils::CreateMesh(7, 3);
		
		explosionTexture = AEGfxTextureLoad("Assets/explosion.png");
		explosionMesh = Utils::CreateMesh(1, 9);

		minionTexture = AEGfxTextureLoad("Assets/item_minion_0.png");
		minionMesh = Utils::CreateMesh();
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

		if (minionTexture)
		{
			AEGfxTextureUnload(minionTexture);
			minionTexture = nullptr;
		}

		Utils::DestroyMesh(minionMesh);
	}
}