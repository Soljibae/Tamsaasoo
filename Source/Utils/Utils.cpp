#include "Utils.h"

AEGfxVertexList* Utils::CreateMesh(int row, int column)
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

void Utils::DrawObject(InGame::Actor& object)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	AEGfxSetTransparency(1.0f);

	AEGfxTextureSet(object.Texture, 0, 0); //to do 애니메이션을 위한 오프셋이 필요함

	/*
	AEVec2 translated_pos;

	AEMtx33MultVec(&translated_pos, &camera.translate_matrix, &object.position)

	AEMtx33 scale;
	AEMtx33Scale(&scale, object.size.x, object.size.y);
	AEMtx33 tran;
	AEMtx33Trans(&tran, translated_pos.x, translated_pos.y);
	AEMtx33 transform;

	카메라의 매트릭스를 곱해서 변환된 좌표에서 오브젝트를 그림 -> 실제 좌표 이동 없이 카메라 적용
	*/

	AEMtx33 scale;
	AEMtx33Scale(&scale, object.size.x, object.size.y);
	AEMtx33 tran;
	AEMtx33Trans(&tran, object.position.x, object.position.y);
	AEMtx33 transform;

	AEMtx33Concat(&transform, &tran, &scale);

	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);

	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(object.Mesh, AE_GFX_MDM_TRIANGLES);
}

bool Utils::CheckCollision(InGame::Actor& object1, InGame::Actor& object2)
{
	AEVec2 delta;

	AEVec2Sub(&delta, &object1.position, &object2.position);

	float distSq = delta.x * delta.x + delta.y * delta.y;

	float radiusSum = object1.CollisionRadius + object2.CollisionRadius;
	float radiusSumSq = radiusSum * radiusSum;

	return distSq <= radiusSumSq;
}

bool Utils::IsMouseInSquare(float x, float y, float width, float height)
{
	s32 mx = 0;
	s32 my = 0;

	AEInputGetCursorPosition(&mx, &my);

	float mouse_x = static_cast<float>(mx) - AEGfxGetWindowWidth() / 2.0f;
	float mouse_y = AEGfxGetWindowHeight() / 2.0f - static_cast<float>(my);

	return (x - width / 2.f <= mouse_x && mouse_x <= x + width / 2.f && y - height / 2.f <= mouse_y && mouse_y <= y + height / 2.f);
}

bool Utils::IsMouseInCircle(float x, float y, float r)
{
	s32 mx = 0;
	s32 my = 0;

	AEInputGetCursorPosition(&mx, &my);

	float mouse_x = static_cast<float>(mx) - AEGfxGetWindowWidth() / 2.0f;
	float mouse_y = AEGfxGetWindowHeight() / 2.0f - static_cast<float>(my);

	return ((mouse_x - x) * (mouse_x - x) + (mouse_y - y) * (mouse_y - y) <= r * r);
}