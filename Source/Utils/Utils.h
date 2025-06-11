#include "AEEngine.h"
#include "../InGame/Actor.h"

namespace Utils
{
	//메시 생성 함수 
	AEGfxVertexList* CreateMesh(int row = 1, int column = 1);
	
	//메시 해제 함수
	void DestroyMesh(AEGfxVertexList* Mesh);

	//사각형 그리기 함수
	//void DrawObject(Actor& object);

	//애니메이션 함수

	//충돌 판정(circle to circle)
	bool CheckCollision();

	//카메라 이동 함수
}