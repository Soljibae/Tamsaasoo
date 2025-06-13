#pragma once
#include "AEEngine.h"
#include "../InGame/Actor.h"

namespace Utils
{

	AEGfxVertexList* CreateMesh(s32 row = 1, s32 column = 1);
	
	void DestroyMesh(AEGfxVertexList* Mesh);

	void DrawObject(InGame::Actor& object, bool is_camera_enabled = true, f32 alpha = 1.f);

	void InitOffset(InGame::Actor& object);

	void UpdateOffset(InGame::Actor& object);

	bool CheckCollision(InGame::Actor& object1, InGame::Actor& object2);
  
	bool IsMouseInSquare(InGame::Actor& object);

	bool IsMouseInSquare(f32 x, f32 y, f32 width, f32 height);

	bool IsMouseInCircle(InGame::Actor& object);

	bool IsMouseInCircle(f32 x, f32 y, f32 r);
}