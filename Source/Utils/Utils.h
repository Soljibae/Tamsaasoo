#pragma once
#include "AEEngine.h"
#include "../InGame/Actor.h"

namespace Utils
{
	//�޽� ���� �Լ� 
	AEGfxVertexList* CreateMesh(int row = 1, int column = 1);
	
	//�޽� ���� �Լ�
	void DestroyMesh(AEGfxVertexList* Mesh);

	//�簢�� �׸��� �Լ�
	void DrawObject(InGame::Actor& object);

	//�ִϸ��̼� �Լ�

	//�浹 ����(circle to circle)
	bool CheckCollision(InGame::Actor& object1, InGame::Actor& object2);

	//�׸� �ڽ� �ȿ� ���콺�� ���Դ���?
	bool IsMouseInSquare(float x, float y, float width, float height);

	//�� �ȿ� ���콺�� ���Դ���?
	bool IsMouseInCircle(float x, float y, float r);
}