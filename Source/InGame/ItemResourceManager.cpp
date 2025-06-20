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
	}
}