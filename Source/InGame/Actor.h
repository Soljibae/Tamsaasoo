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
		AEGfxVertexList* Mesh = nullptr;
		AEGfxTexture* Texture = nullptr;
		AEVec2 size;
		AEVec2 position;
		f32 CollisionRadius = 0.f;
		AEVec2 direction;
		f32 MovementSpeed = 0.f;
		s32 row, column;
		f32 FrameTime;
		f32 TimeAcc = 0.f;
		AEVec2 offset;
		bool bIsPandingKill = false;
		bool bIsValid = false;
	};
}
