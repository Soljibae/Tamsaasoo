#include "Utils.h"
#include "../Global/GlobalVariables.h"
#include "../Manager/Playing.h"

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

void Utils::DrawObject(InGame::Actor& object, AEGfxTexture* Texture, AEGfxVertexList* Mesh)
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

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::DrawObjectWithDirection(InGame::Actor& object)
{
	/*
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(object.Texture, object.offset.x, object.offset.y);

	AEVec2 direction_vector;
	AEVec2Set(&direction_vector, object.direction.x, object.direction.y);
	AEVec2Normalize(&direction_vector, &direction_vector);

	AEVec2 translated_pos;
	AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &object.position);

	AEMtx33 tran_inv;
	AEMtx33Trans(&tran_inv, -translated_pos.x, -translated_pos.y);
	AEMtx33 scale;
	AEMtx33Scale(&scale, object.size.x, object.size.y);
	AEMtx33 rotate;
	AEMtx33Rot(&rotate, atan2f(direction_vector.y, direction_vector.x));
	AEMtx33 tran;
	AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);
	AEMtx33 transform;

	AEMtx33Concat(&transform, &scale, &tran_inv);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &tran, &transform);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(object.Mesh, AE_GFX_MDM_TRIANGLES);
	*/
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(object.Texture, object.offset.x, object.offset.y);

	AEVec2 translated_pos;
	AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &object.position);

	AEMtx33 tran_inv;
	AEMtx33Trans(&tran_inv, -translated_pos.x, -translated_pos.y);

	// 좌우 반전 플래그
	float flipX = (object.direction.x >= 0) ? 1.0f : -1.0f;

	// 회전각 계산 (항상 오른쪽 기준으로)
	float angle = atan2f(object.direction.y, fabsf(object.direction.x));
	AEMtx33 rotate;
	AEMtx33Rot(&rotate, angle);

	// 스케일 적용
	AEMtx33 scale;
	AEMtx33Scale(&scale, object.size.x * flipX, object.size.y);

	// 최종 변환
	AEMtx33 tran;
	AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);

	AEMtx33 transform;
	AEMtx33Concat(&transform, &scale, &rotate);
	AEMtx33Concat(&transform, &tran, &transform);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(object.Mesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::DrawItem(InGame::Item& item)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(InGame::Item::itemTexture, item.offset.x, item.offset.y);

	AEMtx33 scale;
	AEMtx33Scale(&scale, InGame::Item::size.x, InGame::Item::size.y);
	AEMtx33 tran;
	AEMtx33Trans(&tran, item.position.x, item.position.y);
	AEMtx33 transform;

	AEMtx33Concat(&transform, &tran, &scale);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(InGame::Item::itemMesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::InitOffset(InGame::Actor& object)
{
	AEVec2Set(&object.offset, 0.f, 1.f - 1.f / object.column);
}

void Utils::UpdateOffset(InGame::Actor& object)
{
	/*object.TimeAcc += global::DeltaTime;

	if (object.TimeAcc > object.FrameTime)
	{
		object.TimeAcc = 0;
		object.animation_cnt = (object.animation_cnt + 1) % object.max_animation_cnt;
	}

	object.offset.x = (1.f / static_cast<f32>(object.column)) * static_cast<f32>((animation_cnt) % object.column);
	object.offset.y = (1.f / static_cast<f32>(object.row)) * static_cast<f32>((animation_cnt) / object.column);*/
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