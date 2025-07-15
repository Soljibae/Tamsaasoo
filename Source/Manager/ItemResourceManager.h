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

		AEGfxTexture* item9Texture;
		AEGfxVertexList* item9Mesh;

		AEGfxTexture* item11Texture;
		AEGfxVertexList* item11Mesh;

		AEGfxTexture* item20Texture;
		AEGfxVertexList* item20Mesh;

		AEGfxTexture* item27Texture;
		AEGfxVertexList* item27Mesh;

		AEGfxTexture* item30Texture;
		AEGfxVertexList* item30Mesh;

		AEGfxTexture* item31minionTexture;
		AEGfxVertexList* item31minionMesh;

		AEGfxTexture* item32minionTexture;
		AEGfxVertexList* item32minionMesh;

		AEGfxTexture* item33minionTexture;
		AEGfxVertexList* item33minionMesh;
	};
}