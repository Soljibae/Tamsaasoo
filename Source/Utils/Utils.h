#pragma once
#include "AEEngine.h"
#include "../InGame/Actor.h"
#include "../InGame/Character.h"
#include "../InGame/PlayerCharacter.h"
#include "../InGame/EnemyCharacter.h"
#include "../InGame/Projectile.h"
#include "../InGame/Item.h"

namespace Utils
{

	AEGfxVertexList* CreateMesh(s32 row = 1, s32 column = 1);

	std::array<AEGfxVertexList*, 9> CreateNinePatchMesh();

	void DrawNinePatchMesh(InGame::Actor object, AEGfxTexture* texture, std::array<AEGfxVertexList*, 9> patches, f32 padding, f32 alpha = 1.f);
	
	void DestroyMesh(AEGfxVertexList* Mesh);

	void DrawObject(InGame::Actor& object, bool is_camera_enabled = true, f32 alpha = 1.f);

	void DrawObject(InGame::Character& object, bool is_camera_enabled = true, f32 alpha = 1.f);
	
	void DrawObject(InGame::Actor& object, AEGfxTexture* Texture, AEGfxVertexList* Mesh, f32 alpha = 1.f);

	void DrawObject(AEVec2 position, AEVec2 offset, AEVec2 size, AEGfxTexture* Texture, AEGfxVertexList* Mesh, f32 alpha = 1.f, bool is_camera_enabled = true);

	void DrawObject(InGame::SkillEffectItem& object, AEGfxTexture* Texture, AEGfxVertexList* Mesh);

	void DrawObjectWithDirection(InGame::Actor& object, f32 alpha = 1.f);

	void DrawObjectWithDirection(InGame::SkillEffectItem& object, AEGfxTexture* Texture, AEGfxVertexList* Mesh, AEVec2 Direction);

	void DrawObjectWithDirection(InGame::SkillEffectItem& object, AEVec2 Position, AEGfxTexture* Texture, AEGfxVertexList* Mesh, AEVec2 Direction);

	void DrawItem(InGame::Item& item);

	void DrawCursor(InGame::Actor cursor);

	void DrawTest(f32 x, f32 y, f32 width, f32 height);

	void InitOffset(InGame::Actor& object);

	void UpdateOffset(InGame::Actor& object);

	void UpdateOffset(InGame::SkillEffectItem& object);

	bool CheckCollision(InGame::Actor& object1, InGame::Actor& object2);

	bool CheckCollision(InGame::Actor& object1, AEVec2 pos, f32 r);

	void CheckCollision(InGame::Projectile& Projectile, std::vector<InGame::EnemyCharacter*> Characters, class InGame::PlayerCharacter& PC, bool isThisBoss);
  
	bool IsMouseInSquare(InGame::Actor& object);

	bool IsMouseInSquare(f32 x, f32 y, f32 width, f32 height);

	bool IsMouseInCircle(InGame::Actor& object);

	bool IsMouseInCircle(f32 x, f32 y, f32 r);

	s32 GetItemCount(s32 ID);

	f32 GetRandomFloat(f32 min, f32 max);

	AEVec2 GetRandomPointInEllipse(f32 collisionSize = 0);

	void ClampActorPosition(InGame::Actor* InActor, AEVec2 NewPos);

	void TestInit();

	void TestDestroy();
}

