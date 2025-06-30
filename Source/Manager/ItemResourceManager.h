#pragma once
#include "AEEngine.h"

namespace Manager
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