#pragma once
#include "AEEngine.h"
#include "../InGame/Actor.h"
#include "../InGame/Item.h"

namespace Utils
{

	AEGfxVertexList* CreateMesh(s32 row = 1, s32 column = 1);
	
	void DestroyMesh(AEGfxVertexList* Mesh);

	void DrawObject(InGame::Actor& object, bool is_camera_enabled = true, f32 alpha = 1.f);

	void DrawObject(InGame::Actor& object, AEGfxTexture* Texture, AEGfxVertexList* Mesh, f32 alpha = 1.f);

	void DrawObject(AEVec2 position, AEVec2 offset, AEVec2 size, AEGfxTexture* Texture, AEGfxVertexList* Mesh, f32 alpha = 1.f);

	void DrawObject(InGame::SkillEffectItem& object, AEGfxTexture* Texture, AEGfxVertexList* Mesh);

	void DrawObjectWithDirection(InGame::Actor& object);

	void DrawItem(InGame::Item& item);

	void DrawTest(f32 x, f32 y, f32 width, f32 height);

	void InitOffset(InGame::Actor& object);

	void UpdateOffset(InGame::Actor& object);

	void UpdateOffset(InGame::SkillEffectItem& object);

	bool CheckCollision(InGame::Actor& object1, InGame::Actor& object2);

	bool CheckCollision(InGame::Actor& object1, AEVec2 pos, f32 r);
  
	bool IsMouseInSquare(InGame::Actor& object);

	bool IsMouseInSquare(f32 x, f32 y, f32 width, f32 height);

	bool IsMouseInCircle(InGame::Actor& object);

	bool IsMouseInCircle(f32 x, f32 y, f32 r);

	s32 GetItemCount(s32 ID);

	void TestInit();

	void TestDestroy();
}