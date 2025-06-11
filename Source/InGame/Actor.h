#pragma once
#include "AEEngine.h"
namespace InGame
{
	class Actor
	{
	public:
		virtual void Init();
		virtual void Update();
		virtual void Draw();
		virtual void Destroy();

		AEGfxVertexList* GetMesh();
		void SetMesh(AEGfxVertexList* InMesh);
	protected:
		AEGfxVertexList* Mesh = nullptr;
		AEGfxTexture* Texture = nullptr;
		f32 sizeX = 0.f;
		f32 sizeY = 0.f;
		f32 posX = 0.f;
		f32 posY = 0.f;
		f32 CollisionRadius = 0.f;
		f32 DirX = 0.f;
		f32 DirY = 0.f;
		f32 MovementSpeed = 0.f;
	};
}
