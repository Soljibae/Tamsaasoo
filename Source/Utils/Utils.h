#pragma once
#include "AEEngine.h"
#include "../InGame/Actor.h"

namespace Utils
{
	//�޽� ���� �Լ� 
	AEGfxVertexList* CreateMesh(s32 row = 1, s32 column = 1);
	
	//�޽� ���� �Լ�
	void DestroyMesh(AEGfxVertexList* Mesh);

	//�簢�� �׸��� �Լ�
	void DrawObject(InGame::Actor& object, f32 alpha = 1.f);

	//�ִϸ��̼� �Լ�
	void InitOffset(InGame::Actor& object);

	void UpdateOffset(InGame::Actor& object);

	//�浹 ����(circle to circle)
	bool CheckCollision(InGame::Actor& object1, InGame::Actor& object2);

	//�׸� �ڽ� �ȿ� ���콺�� ���Դ���?
	bool IsMouseInSquare(f32 x, f32 y, f32 width, f32 height);

	//�� �ȿ� ���콺�� ���Դ���?
	bool IsMouseInCircle(f32 x, f32 y, f32 r);

}