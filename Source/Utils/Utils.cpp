#include "Utils.h"
#include "../Global/GlobalVariables.h"
#include "../Manager/PlayingState.h"

AEGfxVertexList* Utils::CreateMesh(s32 row, s32 column)
{
	AEGfxVertexList* Mesh;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f / static_cast<float>(row), 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f - 1.0f / static_cast<float>(column));

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f / static_cast<float>(row), 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f / static_cast<float>(row), 1.0f - 1.0f / static_cast<float>(column),
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f - 1.0f / static_cast<float>(column));

	Mesh = AEGfxMeshEnd();

	return Mesh;
}

void Utils::DestroyMesh(AEGfxVertexList* Mesh)
{
	if(Mesh)
		AEGfxMeshFree(Mesh);
}

void Utils::DrawObject(InGame::Actor& object, f32 alpha)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(object.Texture, object.offset.x, object.offset.y);

	AEVec2 translated_pos;

	AEMtx33MultVec(&translated_pos, &(Manager::CAM->translate_matrix), &object.position);

	AEMtx33 scale;
	AEMtx33Scale(&scale, object.size.x, object.size.y);
	AEMtx33 tran;
	AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);
	AEMtx33 transform;

	AEMtx33Concat(&transform, &tran, &scale);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 0.f);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, alpha);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(object.Mesh, AE_GFX_MDM_TRIANGLES);
}

void Utils::InitOffset(InGame::Actor& object)
{
	AEVec2Set(&object.offset, 0.f, 1.f - 1.f / object.column);
}

void Utils::UpdateOffset(InGame::Actor& object)
{
	s32 animation_cnt{ 0 }; // actor���� animation_count ���� �������ҵ�
	s32 n = object.row * object.column; // actor���� max_animation_count ���� �������ҵ�

	object.TimeAcc += global::DeltaTime;

	if (object.TimeAcc > object.FrameTime)
	{
		object.TimeAcc = 0;
		animation_cnt = (animation_cnt + 1) % n;
	}

	object.offset.x = 1.f / static_cast<f32>(object.column) * static_cast<f32>(animation_cnt % object.column);
	object.offset.y = 1.f - 1.f / static_cast<f32>(object.row) * static_cast<f32>(animation_cnt / object.column + 1);

	/*object.TimeAcc += global::DeltaTime;

	if (object.TimeAcc > object.FrameTime)
	{
		object.TimeAcc = 0;
		object.animation_cnt = (object.animation_cnt + 1) % object.max_animation_cnt;
	}

	object.offset.x = 1.f / static_cast<f32>(object.column) * static_cast<f32>(object.animation_cnt % object.column);
	object.offset.y = 1.f - 1.f / static_cast<f32>(object.row) * static_cast<f32>(object.animation_cnt / object.column + 1);*/
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

bool Utils::IsMouseInSquare(f32 x, f32 y, f32 width, f32 height)
{
	s32 mx = 0;
	s32 my = 0;

	AEInputGetCursorPosition(&mx, &my);

	f32 mouse_x = static_cast<f32>(mx) - AEGfxGetWindowWidth() / 2.0f;
	f32 mouse_y = AEGfxGetWindowHeight() / 2.0f - static_cast<f32>(my);

	return (x - width / 2.f <= mouse_x && mouse_x <= x + width / 2.f && y - height / 2.f <= mouse_y && mouse_y <= y + height / 2.f);
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