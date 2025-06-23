#pragma once
#include "AEEngine.h"

namespace InGame
{
	class ItemResourceManager
	{
	public:
		void Init();
		void Destroy();

		AEGfxTexture* itemIconTexture;
		AEGfxVertexList* itemIconMesh;

		AEGfxTexture* explosionTexture;
		AEGfxVertexList* explosionMesh;

		AEGfxTexture* minionTexture;
		AEGfxVertexList* minionMesh;
	};
}