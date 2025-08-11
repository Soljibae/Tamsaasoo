#pragma once
#include "AEEngine.h"
#include "GameState.h"
#include "../InGame/PlayerCharacter.h"
#include "../InGame/Projectile.h"
#include "../InGame/EnemyCharacter.h"
#include "../InGame/Background.h"
#include "../Utils/Camera.h"
#include <vector>
#include <list>
#include "../InGame/EnemyData.h"
#include "../InGame/EnemyBoss.h"
#include "../InGame/Item.h"
#include "ItemDatabase.h"
#include "../InGame/Stage.h"
#include "ItemResourceManager.h"
#include "../InGame/SpawnList.h"
#include "../InGame/ArealAttack.h"
#include "../InGame/VFX.h"
#include "../InGame/SoulOrb.h"
#include "BossHPBar.h"

namespace Manager
{
	class Playing : public AGameState
	{
	public:
		void Init() override;
		void Update() override;
		void Draw() override;
		void Destroy() override;

		InGame::Actor Fader;
		InGame::PlayerCharacter* PC;
		std::vector<InGame::Projectile*> PPPool;
		std::vector<InGame::EnemyCharacter*> ECPool;
		std::vector<InGame::Projectile*> EPPool;
		std::vector<InGame::Projectile*> PPs;
		std::vector<InGame::EnemyCharacter*> ECs;
		std::vector<InGame::Projectile*> EPs;
		std::list<InGame::ArealAttack*> PAAs;
		std::list<InGame::ArealAttack*> EAAs;
		std::vector<InGame::SoulOrb*> SOs;
		InGame::Background* BG;
		ItemDatabase* ITDB;
		ItemResourceManager* ITRM;
		InGame::WaveManager WM;

		bool bIsBossFight = false;
		u8 WaveCount = 0;
		f32 StageTimer;
		f32 WaveTimer;
		InGame::EnemyType SpawningEnemyType;
		InGame::MinionData MinionStruct;
		InGame::ArcherData ArcherStruct;
		InGame::DasherData DasherStruct;
		InGame::TankerData TankerStruct;
		InGame::BomberData BomberStruct;
		InGame::ZigZagData ZigZagStruct;
		InGame::OrbiterData OrbiterStruct;
		InGame::SniperData SniperStruct;
		InGame::BurnerData BurnerStruct;
		InGame::HolerData HolerStruct;
		void SpawnWave();
		void ClearWave();
		void InitBossFight();
		void FinishBossFight();
		void ChangeStage();
		InGame::EnemyBoss* Boss = nullptr;
		BossHPBar bossHPBar;
		InGame::Stage* CurrentStage = nullptr;
		InGame::StageType CurrentStageType;
		bool isChangingStage = false;
		void InitStage();
		AEVec2 GetSpawnLocation();

		void DrawTime(f32 time);

		bool bIsJumping = false;
		float JumpAnimationTimer = 0.f;
		InGame::VFXManager VFXManager;

		bool startWeaponPicked = false;

		s8 pFont;
	};
	extern Utils::Camera* CAM;
	
}