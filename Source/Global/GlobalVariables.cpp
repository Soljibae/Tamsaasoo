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
	bool isBossBattleStarted = false;
	bool isTestMod = false;
	s32 RecentlyDeadEnemyCount = 0;
	s32 CurrentStageNumber = 0;
	f32 item12TriggerRatio;
	f32 item12AdditionalDamage;
	f32 item23RerollCostRatio = 1.f;
	f32 item24GoldGained;
	f32 item32ExplosionSize = 0.f;
	s32 MaxPotionGauge = 300;

	// additionalStats =================
	f32 additionalDamage = 0.f;
	f32 additionalDamageRatio = 1.f;

	f32 additionalFireRate = 0.f;
	f32 additionalFireRateRatio = 1.f;

	f32 additionalExpGainedRatio = 1.f;
	f32 additionalGoldGainedRatio = 1.f;
	s32 additionalHitCount = 0;
	f32 additionalMovementSpeed = 0;

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

	f32 additionalDamageToBossRatio = 1.f;
	f32 additionalDamageFromBossRatio = 1.f;

	std::array<f32, 3> StageExpGainedRatio = { 2.5f , 4.0f, 6.0f };
	std::array<f32, 3> StageGoldGainedRatio = { 1.7f , 2.8f, 4.0f };
	std::array<f32, 3> StageRerollCostRatio = { 1.4f , 2.5f, 3.6f };
	//==================================
}