#pragma once
#include "AEEngine.h"
namespace global
{
	static s32 ScreenWidth = 1600;
	static s32 ScreenHeight = 900;
	extern s8 font;
	extern f32 DeltaTime;
	extern AEVec2 PlayerLocation;
	extern AEVec2 PlayerMouseDirection;
	extern bool IsEnemyRecentlyDied;
	extern AEVec2 RecentlyDeadEnemyPosition;
	extern AEVec2 worldMin, worldMax;
	extern u8 (*KeyInput)(u8);
	extern AEGfxTexture* testTexture;
	extern AEGfxVertexList* testMesh;
}