#include "AEEngine.h"
#include "../InGame/Actor.h"

namespace Utils
{
	//�޽� ���� �Լ� 
	AEGfxVertexList* CreateMesh(int row = 1, int column = 1);
	
	//�޽� ���� �Լ�
	void DestroyMesh(AEGfxVertexList* Mesh);

	//�簢�� �׸��� �Լ�
	//void DrawObject(Actor& object);

	//�ִϸ��̼� �Լ�

	//�浹 ����(circle to circle)
	bool CheckCollision();

	//ī�޶� �̵� �Լ�
}