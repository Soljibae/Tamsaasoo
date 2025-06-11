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

	//ī�޶� �̵� �Լ�

	//�׸� �ڽ� �ȿ� ���콺�� ���Դ���?

	//�� �ȿ� ���콺�� ���Դ���?
}