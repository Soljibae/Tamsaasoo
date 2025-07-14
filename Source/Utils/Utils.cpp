#include "Utils.h"
#include "../Global/GlobalVariables.h"
#include "../Manager/Playing.h"
#include "../Manager/GameManager.h"
#include "../InGame/PlayerCharacter.h"
#include <random>

AEGfxVertexList* Utils::CreateMesh(s32 row, s32 column)
{
	AEGfxVertexList* Mesh;

	AEGfxMeshStart();

	f32 sprite_uv_width = 1.f / static_cast<f32>(column);
	f32 sprite_uv_height = 1.f / static_cast<f32>(row);


	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, sprite_uv_height,
		0.5f, -0.5f, 0xFFFFFFFF, sprite_uv_width, sprite_uv_height,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, sprite_uv_width, sprite_uv_height,
		0.5f, 0.5f, 0xFFFFFFFF, sprite_uv_width, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	Mesh = AEGfxMeshEnd();

	return Mesh;
}

std::array<AEGfxVertexList*, 9> Utils::CreateNinePatchMesh()
{
	struct P { f32 x, y, u0, v0, u1, v1; };
	std::vector<P> Patches = {

		{ 0.5f, 0.5f, 0.00f, 0.75f, 0.25f, 1.00f },  // TopLeft
		{ 0.5f, 0.5f, 0.25f, 0.75f, 0.75f, 1.00f },  // TopCenter
		{ 0.5f, 0.5f, 0.75f, 0.75f, 1.00f, 1.00f },  // TopRight

		{ 0.5f, 0.5f, 0.00f, 0.25f, 0.25f, 0.75f },  // CenterLeft
		{ 0.5f, 0.5f, 0.25f, 0.25f, 0.75f, 0.75f },  // Center
		{ 0.5f, 0.5f, 0.75f, 0.25f, 1.00f, 0.75f },  // CenterRight		

		{ 0.5f, 0.5f, 0.00f, 0.00f, 0.25f, 0.25f },  // BottomLeft
		{ 0.5f, 0.5f, 0.25f, 0.00f, 0.75f, 0.25f },  // BottomCenter
		{ 0.5f, 0.5f, 0.75f, 0.00f, 1.00f, 0.25f },   // BottomRight

	};
	std::array<AEGfxVertexList*, 9> PatchArr{ nullptr };
	int i = 0;
	for (auto p : Patches)
	{
		AEGfxTriAdd(
			-p.x, -p.y, 0xFFFFFFFF, p.u0, p.v0,
			p.x, -p.y, 0xFFFFFFFF, p.u1, p.v0,
			-p.x, p.y, 0xFFFFFFFF, p.u0, p.v1
		);
		AEGfxMeshStart();
		AEGfxTriAdd(
			p.x, -p.y, 0xFFFFFFFF, p.u1, p.v0,
			p.x, p.y, 0xFFFFFFFF, p.u1, p.v1,
			-p.x, p.y, 0xFFFFFFFF, p.u0, p.v1
		);
		PatchArr[i] = AEGfxMeshEnd();

		i++;
	}
	return PatchArr;
}
void Utils::DrawNinePatchMesh(InGame::Actor object, AEGfxTexture* texture, std::array<AEGfxVertexList*, 9> patches, f32 padding)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	AEGfxTextureSet(texture, 0.f, 0.f);
	AEMtx33 scale;
	AEMtx33 tran;
	AEMtx33 transform;
	int i = 1;
	for (auto mesh : patches)
	{
		float x_trans_offset = 0;
		float y_trans_offset = 0;
		float x_scale = 0;
		float y_scale = 0;
		switch (i)
		{
		case 2:
			x_scale = object.size.x;
			y_scale = padding;
			x_trans_offset = 0;
			y_trans_offset = (object.size.y + padding) / 2.f;
			break;
		case 8:
			x_scale = object.size.x;
			y_scale = padding;
			x_trans_offset = 0;
			y_trans_offset = -(object.size.y + padding) / 2.f;
			break;
		case 4:
			x_scale = padding;
			y_scale = object.size.y;
			x_trans_offset = -(object.size.x + padding) / 2.f;
			y_trans_offset = 0;
			break;
		case 6:
			x_scale = padding;
			y_scale = object.size.y;
			x_trans_offset = (object.size.x + padding) / 2.f;
			y_trans_offset = 0;
			break;
		case 5:
			x_scale = object.size.x;
			y_scale = object.size.y;
			x_trans_offset = 0;
			y_trans_offset = 0;
			break;
		case 1:
			x_scale = padding;
			y_scale = padding;
			x_trans_offset = -(object.size.x + padding) / 2.f;
			y_trans_offset = (object.size.y + padding) / 2.f;
			break;
		case 3:
			x_scale = padding;
			y_scale = padding;
			x_trans_offset = (object.size.x + padding) / 2.f;
			y_trans_offset = (object.size.y + padding) / 2.f;
			break;
		case 7:
			x_scale = padding;
			y_scale = padding;
			x_trans_offset = -(object.size.x + padding) / 2.f;
			y_trans_offset = -(object.size.y + padding) / 2.f;
			break;
		case 9:
			x_scale = padding;
			y_scale = padding;
			x_trans_offset = (object.size.x + padding) / 2.f;
			y_trans_offset = -(object.size.y + padding) / 2.f;
			break;
		}

		AEMtx33Scale(&scale,
			x_scale,
			y_scale);
		AEMtx33Trans(&tran,
			object.position.x + x_trans_offset,
			object.position.y + y_trans_offset);
		AEMtx33Concat(&transform, &tran, &scale);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
		i++;
	}
}
void Utils::DestroyMesh(AEGfxVertexList* Mesh)
{
	if (Mesh != nullptr)
	{
		AEGfxMeshFree(Mesh);
	}
}

void Utils::DrawObject(InGame::Actor& object, bool is_camera_enabled, f32 alpha)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(object.Texture, object.offset.x, object.offset.y);

	AEMtx33 scale;
	AEMtx33Scale(&scale, object.size.x, object.size.y);
	AEMtx33 tran;
	AEMtx33Trans(&tran, object.position.x, object.position.y);
	AEMtx33 transform;

	if (is_camera_enabled)
	{
		AEVec2 translated_pos;
		AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &object.position);
		AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);
	}

	AEMtx33Concat(&transform, &tran, &scale);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, alpha);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(object.Mesh, AE_GFX_MDM_TRIANGLES);
  }

void Utils::DrawObject(InGame::Character& object, bool is_camera_enabled, f32 alpha)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(object.Texture, object.offset.x, object.offset.y);

	AEMtx33 scale;
	AEMtx33Scale(&scale, object.size.x, object.size.y);
	AEMtx33 tran;
	AEMtx33Trans(&tran, object.position.x, object.position.y);
	AEMtx33 transform;

	if (is_camera_enabled)
	{
		AEVec2 translated_pos;
		AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &object.position);
		AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);
	}

	AEMtx33Concat(&transform, &tran, &scale);
	if (object.Stats->StatusEffectTimer[InGame::EStatusEffect::BURN] > 0.f)
	{
		AEGfxSetColorToMultiply(1.f, 0.f, 0.f, 0.3f);
	}
	else if (object.Stats->StatusEffectTimer[InGame::EStatusEffect::STUN] > 0.f)
	{
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.3f);
	}
	else if (object.Stats->StatusEffectTimer[InGame::EStatusEffect::SLOW] > 0.f)
	{
		AEGfxSetColorToMultiply(0.f, 1.f, 0.f, 0.3f);
	}
	else if (object.Stats->StatusEffectTimer[InGame::EStatusEffect::FEAR] > 0.f)
	{
		AEGfxSetColorToMultiply(1.f, 0.f, 1.f, 0.3f);
	}
	else if (object.Stats->StatusEffectTimer[InGame::EStatusEffect::VULNERABLE] > 0.f)
	{
		AEGfxSetColorToMultiply(1.f, 1.f, 0.f, 0.3f);
	}
	else
	{
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);
	}

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, alpha);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(object.Mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);
}
void Utils::DrawObject(InGame::Actor& object, AEGfxTexture* Texture, AEGfxVertexList* Mesh, f32 alpha)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(Texture, object.offset.x, object.offset.y);

	AEMtx33 scale;
	AEMtx33Scale(&scale, object.size.x, object.size.y);
	AEMtx33 tran;
	AEMtx33Trans(&tran, object.position.x, object.position.y);
	AEMtx33 transform;

	AEMtx33Concat(&transform, &tran, &scale);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, alpha);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::DrawObject(AEVec2 position, AEVec2 offset, AEVec2 size, AEGfxTexture* Texture, AEGfxVertexList* Mesh, f32 alpha)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	if (Manager::gm.currStateREF)
	{
		Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
		
		AEGfxTextureSet(Texture, offset.x, offset.y);

		AEMtx33 scale;
		AEMtx33Scale(&scale, size.x, size.y);
		AEMtx33 tran;
		AEVec2 translated_pos;
		AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &position);
		AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);
		AEMtx33 transform;

		AEMtx33Concat(&transform, &tran, &scale);

		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

		AEGfxSetColorToAdd(0.f, 0.f, 0.f, alpha);

		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
			
	}
}

void Utils::DrawObject(InGame::SkillEffectItem& object, AEGfxTexture* Texture, AEGfxVertexList* Mesh)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(Texture, object.AnimationOffset.x, object.AnimationOffset.y);

	AEMtx33 scale;
	AEMtx33Scale(&scale, object.effectSize.x, object.effectSize.y);

	AEMtx33 tran;
	AEVec2 translated_pos;
	AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &object.effectPosition);
	AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);
	
	AEMtx33 transform;
	AEMtx33Concat(&transform, &tran, &scale);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::DrawObjectWithDirection(InGame::Actor& object, f32 alpha)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(object.Texture, object.offset.x, object.offset.y);

	// 1. scale
	AEMtx33 scale;
	AEMtx33Scale(&scale, object.size.x, object.size.y);

	// 2. flip
	float flipX = (object.direction.x >= 0.f) ? 1.0f : -1.0f;
	AEMtx33 flip;
	AEMtx33Scale(&flip, 1, flipX);

	// 3. rotate
	float angle = atan2f(object.direction.y, object.direction.x);
	AEMtx33 rotate;
	AEMtx33Rot(&rotate, angle);

	// 4. translate
	AEVec2 translated_pos;
	AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &object.position);
	AEMtx33 tran;
	AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);

	
	AEMtx33 transform;
	AEMtx33Concat(&transform, &flip, &scale);
	AEMtx33Concat(&transform, &rotate, &transform);
	AEMtx33Concat(&transform, &tran, &transform);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, alpha);
	
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(object.Mesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::DrawObjectWithDirection(InGame::SkillEffectItem& object, AEGfxTexture* Texture, AEGfxVertexList* Mesh, AEVec2 Direction)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(Texture, object.AnimationOffset.x, object.AnimationOffset.y);

	// 1. scale
	AEMtx33 scale;
	AEMtx33Scale(&scale, object.effectSize.x, object.effectSize.y);

	// 2. flip
	float flipX = (Direction.x >= 0.f) ? 1.0f : -1.0f;
	AEMtx33 flip;
	AEMtx33Scale(&flip, 1, flipX);

	// 3. rotate
	float angle = atan2f(Direction.y, Direction.x);
	AEMtx33 rotate;
	AEMtx33Rot(&rotate, angle);

	// 4. translate
	AEVec2 translated_pos;
	AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &object.effectPosition);
	AEMtx33 tran;
	AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);


	AEMtx33 transform;
	AEMtx33Concat(&transform, &flip, &scale);
	AEMtx33Concat(&transform, &rotate, &transform);
	AEMtx33Concat(&transform, &tran, &transform);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::DrawObjectWithDirection(InGame::SkillEffectItem& object, AEVec2 Position, AEGfxTexture* Texture, AEGfxVertexList* Mesh, AEVec2 Direction)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(Texture, object.AnimationOffset.x, object.AnimationOffset.y);

	// 1. scale
	AEMtx33 scale;
	AEMtx33Scale(&scale, object.effectSize.x, object.effectSize.y);

	// 2. flip
	float flipX = (Direction.x >= 0.f) ? 1.0f : -1.0f;
	AEMtx33 flip;
	AEMtx33Scale(&flip, 1, flipX);

	// 3. rotate
	float angle = atan2f(Direction.y, Direction.x);
	AEMtx33 rotate;
	AEMtx33Rot(&rotate, angle);

	// 4. translate
	AEVec2 translated_pos;
	AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &Position);
	AEMtx33 tran;
	AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);


	AEMtx33 transform;
	AEMtx33Concat(&transform, &flip, &scale);
	AEMtx33Concat(&transform, &rotate, &transform);
	AEMtx33Concat(&transform, &tran, &transform);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::DrawItem(InGame::Item& item)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	if (Manager::gm.currStateREF)
	{
		Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
		if (GS)
		{
			if (GS->ITRM)
			{
				AEGfxTextureSet(GS->ITRM->itemIconTexture, item.iconOffset.x, item.iconOffset.y);

				AEMtx33 scale;
				AEMtx33Scale(&scale, InGame::Item::size.x, InGame::Item::size.y);
				AEMtx33 tran;
				AEMtx33Trans(&tran, item.iconPosition.x, item.iconPosition.y);
				AEMtx33 transform;

				AEMtx33Concat(&transform, &tran, &scale);

				AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

				AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

				AEGfxSetTransform(transform.m);

				AEGfxMeshDraw(GS->ITRM->itemIconMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}
}

void Utils::DrawTest(f32 x, f32 y, f32 width, f32 height)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(global::testTexture, 0.f, 0.f);

	AEVec2 pos;
	AEVec2Set(&pos, x, y);
	AEVec2 translated_pos;
	AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &pos);

	AEMtx33 scale;
	AEMtx33Scale(&scale, width, height);
	AEMtx33 tran;
	AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);
	AEMtx33 transform;

	AEMtx33Concat(&transform, &tran, &scale);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(global::testMesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::InitOffset(InGame::Actor& object)
{
	AEVec2Set(&object.offset, 0.f, 0.f);
}

void Utils::UpdateOffset(InGame::Actor& object)
{
	object.TimeAcc += global::DeltaTime;

	if (object.TimeAcc > object.FrameTime)
	{
		object.TimeAcc = 0;
		object.AnimationCount = (object.AnimationCount + 1) % object.MaxAnimationCount[object.AnimationState];

	}

	object.offset.x = 1.f / object.column * static_cast<f32>(object.AnimationCount);
	object.offset.y = 1.f / object.row * static_cast<f32>(object.AnimationState - 1);
}

void Utils::UpdateOffset(InGame::SkillEffectItem& object)
{
	object.AnimationTimer += global::DeltaTime;
	if (object.AnimationTimer >= object.FrameTime)
	{
		object.AnimationTimer = 0;
		object.AnimationCount = (object.AnimationCount + 1) % object.MaxAnimationCount;
	}
	object.AnimationOffset.x = 1.f / object.effectColumn * static_cast<f32>(object.AnimationCount);
	object.AnimationOffset.y = 1.f / object.effectRow * static_cast<f32>(object.AnimationCount / object.effectColumn);
}

bool Utils::CheckCollision(InGame::Actor& object1, InGame::Actor& object2)
{
	AEVec2 delta;

	AEVec2Sub(&delta, &object1.position, &object2.position);

	f32 distSq = delta.x * delta.x + delta.y * delta.y;

	f32 radiusSum = object1.CollisionRadius + object2.CollisionRadius;
	f32 radiusSumSq = radiusSum * radiusSum;

	return distSq <= radiusSumSq;
}

bool Utils::CheckCollision(InGame::Actor& object1, AEVec2 pos, f32 r)
{
	AEVec2 delta;

	AEVec2Sub(&delta, &object1.position, &pos);

	f32 distSq = delta.x * delta.x + delta.y * delta.y;

	f32 radiusSum = object1.CollisionRadius + r;
	f32 radiusSumSq = radiusSum * radiusSum;

	return distSq <= radiusSumSq;
}

void Utils::CheckCollision(InGame::Projectile& Projectile, std::vector<InGame::EnemyCharacter*> Characters, class InGame::PlayerCharacter& PC, bool isThisBoss)
{
	
	std::vector<std::pair<f32, InGame::EnemyCharacter*>> collided;

	AEVec2 start = Projectile.LastLoc;
	AEVec2 end = Projectile.position;

	AEVec2 dir;
	AEVec2Sub(&dir, &end, &start);

	f32 a = dir.x * dir.x + dir.y * dir.y;

	for (InGame::EnemyCharacter* Character : Characters)
	{
		AEVec2 center = Character->position;
		f32 radius = Character->CollisionRadius;

		AEVec2 f;
		AEVec2Sub(&f, &start, &center);

		f32 b = 2 * (f.x * dir.x + f.y * dir.y);
		f32 c = (f.x * f.x + f.y * f.y) - radius * radius;

		f32 discriminant = b * b - 4 * a * c;

		if (discriminant < 0)
			continue;

		discriminant = sqrtf(discriminant);
		f32 t1 = (-b - discriminant) / (2 * a);
		f32 t2 = (-b + discriminant) / (2 * a);

		f32 t = -1.0f;

		if (t1 >= 0.0f && t1 <= 1.0f)
			t = t1;
		else if (t2 >= 0.0f && t2 <= 1.0f)
			t = t2;

		if (t >= 0.0f)
		{
			collided.push_back({ t, Character });
		}
	}

	std::sort(collided.begin(), collided.end(),
		[](const auto& a, const auto& b) {
			return a.first < b.first;
		});

	std::vector<InGame::EnemyCharacter*> results;
	for (auto& pair : collided)
	{
		results.push_back(pair.second);
	}
	for (InGame::EnemyCharacter* result : results)
	{
		if (Projectile.HitCount > 0)
		{
			if (result->bIsPandingKill)
			{
				continue;
			}
			else
			{
				if (Projectile.hasHit)
					continue;
				if (Projectile.isExplosive)
				{
					Projectile.OnHit(result);
					Projectile.hasHit = true;
				}
				if(isThisBoss)
					result->adjustHealth(-Projectile.Damage * global::additionalDamageToBossRatio);
				else
					result->adjustHealth(-Projectile.Damage);
				PC.OnProjectileHit(result, false);
				if (!Projectile.isExplosive)
					Projectile.OnHit();
			}
		}
		else
		{
			
			if (!Projectile.isExplosive)
				Projectile.bIsPandingKill = true;

			break;
		}
	}
}

bool Utils::IsMouseInSquare(InGame::Actor& object)
{
	return Utils::IsMouseInSquare(object.position.x, object.position.y, object.size.x, object.size.y);
}

bool Utils::IsMouseInSquare(f32 x, f32 y, f32 width, f32 height)
{
	s32 mx = 0;
	s32 my = 0;

	AEInputGetCursorPosition(&mx, &my);

	f32 mouse_x = static_cast<f32>(mx) - AEGfxGetWindowWidth() / 2.0f;
	f32 mouse_y = AEGfxGetWindowHeight() / 2.0f - static_cast<f32>(my);

	return (x - width / 2.f <= mouse_x && mouse_x <= x + width / 2.f && y - height / 2.f <= mouse_y && mouse_y <= y + height / 2.f);
}

bool Utils::IsMouseInCircle(InGame::Actor& object)
{
	return Utils::IsMouseInCircle(object.position.x, object.position.y, object.CollisionRadius);
}

bool Utils::IsMouseInCircle(f32 x, f32 y, f32 r)
{
	s32 mx = 0;
	s32 my = 0;

	AEInputGetCursorPosition(&mx, &my);

	f32 mouse_x = static_cast<f32>(mx) - AEGfxGetWindowWidth() / 2.0f;
	f32 mouse_y = AEGfxGetWindowHeight() / 2.0f - static_cast<f32>(my);

	return ((mouse_x - x) * (mouse_x - x) + (mouse_y - y) * (mouse_y - y) <= r * r);
}

s32 Utils::GetItemCount(s32 ID)
{
	Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);

	for (auto curr = GS->PC->inventory.begin(); curr != GS->PC->inventory.end(); ++curr)
	{
		if (curr->first->id == ID)
		{
			return curr->second;
		}
	}

	return 0;
}

f32 Utils::GetRandomFloat(f32 min, f32 max)
{
	static std::random_device rd; 
	static std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dis(min, max);

	return dis(gen);
}

AEVec2 Utils::GetRandomPointInEllipse(f32 collisionSize)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	float angle = dist(gen) * TWO_PI; 

	float distance = std::sqrt(dist(gen));

	float ellipseA = (global::worldMax.x - global::worldMin.x) / 2.0f - collisionSize;
	float ellipseB = (global::worldMax.y - global::worldMin.y) / 2.0f - collisionSize;

	float relativeX = ellipseA * distance * AECos(angle);
	float relativeY = ellipseB * distance * AESin(angle);

	AEVec2 Point;
	AEVec2Set(&Point, relativeX, relativeY);

	return Point;
}

void Utils::ClampActorPosition(InGame::Actor* InActor, AEVec2 NewPos)
{
	if (InActor)
	{
		if (Manager::gm.currStateREF)
		{
			Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
			if (GS)
			{
				if (GS->CurrentStageType == InGame::StageType::TOWER)
				{
					float EllipseA = (global::worldMax.x - global::worldMin.x) / 2;
					float EllipseB = (global::worldMax.y - global::worldMin.y) / 2;
					float value = (NewPos.x * NewPos.x) / (EllipseA * EllipseA) + (NewPos.y * NewPos.y) / (EllipseB * EllipseB);
					if (value <= 1.0f)
					{
						InActor->position = NewPos;
					}
				}
				else
				{
					if (NewPos.x > global::worldMax.x)
					{
						InActor->position.x = global::worldMax.x;
					}
					else
					{
						InActor->position.x = NewPos.x;
					}
					if (InActor->position.x < global::worldMin.x)
					{
						InActor->position.x = global::worldMin.x;
					}
					else
					{
						InActor->position.x = NewPos.x;
					}
					if (InActor->position.y > global::worldMax.y)
					{
						InActor->position.y = global::worldMax.y;
					}
					else
					{
						InActor->position.y = NewPos.y;
					}
					if (InActor->position.y < global::worldMin.y)
					{
						InActor->position.y = global::worldMin.y;
					}
					else
					{
						InActor->position.y = NewPos.y;
					}
				}
			}
		}
	}
}

void Utils::TestInit()
{
	global::testTexture = AEGfxTextureLoad("Assets/test.png");
	global::testMesh = Utils::CreateMesh();
}

void Utils::TestDestroy()
{
	AEGfxTextureUnload(global::testTexture);
	Utils::DestroyMesh(global::testMesh);
}
//