#include "GlobalVariables.h"
namespace global
{
	s8 font;
	f32 DeltaTime = 0;
	AEVec2 PlayerLocation;
	AEVec2 PlayerMouseDirection;
	bool IsEnemyRecentlyDied = false;
	AEVec2 RecentlyDeadEnemyPosition;
	AEVec2 worldMin, worldMax;
	u8(*KeyInput)(u8) = AEInputCheckTriggered;
	AEGfxTexture* testTexture = nullptr;
	AEGfxVertexList* testMesh = nullptr;

	// additionalStats =================
	f32 additionalDamage = 0.f;
	f32 additionalDamageRatio = 1.f;

	f32 additionalFireRate = 0.f;
	f32 additionalFireRateRatio = 1.f;

	f32 additionalExpGainedRatio = 1.f;
	s32 additionalHitCount = 0;

	f32 effectiveBurnDamage = 0.f;
	f32 additionalBurnDamage = 0.f;
	f32 effectiveBurnRate = 1.f;
	f32 additionalBurnRate = 0.f;

	f32 additionalMinionDamage = 0.f;
	f32 additionalMinionDamageRatio = 1.f;

	f32 additionalMinionFireRate = 0.f;
	f32 additionalMinionFireRateRatio = 1.f;
	s32 additionalMinionHitCount = 0;

	f32 additionalProcChanceRatio = 1.f;;
	//==================================
}