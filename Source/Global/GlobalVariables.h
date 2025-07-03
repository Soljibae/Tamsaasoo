#pragma once
#include "AEEngine.h"
#include <array>
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
	extern bool isBossBattleStarted;
	extern s32 RecentlyDeadEnemyCount;
	extern s32 CurrentStageNumber;

	extern f32 item12TriggerRatio;
	extern f32 item12AdditionalDamage;
	extern f32 item23RerollCostRatio;
	extern f32 item24GoldGained;
	extern f32 item32ExplosionSize;

	// additionalStats =================
	extern f32 additionalDamage;
	extern f32 additionalDamageRatio;

	extern f32 additionalFireRate;
	extern f32 additionalFireRateRatio;

	extern f32 additionalExpGainedRatio;
	extern f32 additionalGoldGainedRatio;
	extern s32 additionalHitCount;
	extern f32 additionalMovementSpeed;

	extern f32 effectiveBurnDamage;
	extern f32 additionalBurnDamage;
	extern f32 effectiveBurnRate;
	extern f32 additionalBurnRate;

	extern f32 additionalMinionDamage;
	extern f32 additionalMinionDamageRatio;

	extern f32 additionalMinionFireRate;
	extern f32 additionalMinionFireRateRatio;
	extern s32 additionalMinionHitCount;

	extern f32 additionalProcChanceRatio;

	extern f32 additionalDamageToBossRatio;
	extern f32 additionalDamageFromBossRatio;
	extern s32 mouseX, mouseY;

	extern std::array<f32, 3> StageExpGainedRatio;
	extern std::array<f32, 3> StageGoldGainedRatio;
	extern std::array<f32, 3> StageRerollCostRatio;
	//==================================
}