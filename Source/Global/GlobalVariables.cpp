#include "GlobalVariables.h"
namespace global
{
	s8 font;
	f32 DeltaTime = 0;
	AEVec2 PlayerLocation;
	AEVec2 PlayerMouseDirection;
	AEVec2 worldMin, worldMax;
	u8(*KeyInput)(u8) = AEInputCheckTriggered;
	AEGfxTexture* testTexture = nullptr;
	AEGfxVertexList* testMesh = nullptr;
}