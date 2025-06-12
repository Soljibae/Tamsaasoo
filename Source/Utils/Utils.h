#pragma once
#include "AEEngine.h"
#include "../InGame/Actor.h"

namespace Utils
{
	//메시 생성 함수 
	AEGfxVertexList* CreateMesh(s32 row = 1, s32 column = 1);
	
	//메시 해제 함수
	void DestroyMesh(AEGfxVertexList* Mesh);

	//사각형 그리기 함수
	void DrawObject(InGame::Actor& object, f32 alpha = 1.f);

	//애니메이션 함수
	void InitOffset(InGame::Actor& object);

	void UpdateOffset(InGame::Actor& object);

	//충돌 판정(circle to circle)
	bool CheckCollision(InGame::Actor& object1, InGame::Actor& object2);

	//네모 박스 안에 마우스가 들어왔는지?
	bool IsMouseInSquare(f32 x, f32 y, f32 width, f32 height);

	//원 안에 마우스가 들어왔는지?
	bool IsMouseInCircle(f32 x, f32 y, f32 r);

}