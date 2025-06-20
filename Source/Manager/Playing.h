#pragma once
#include "AEEngine.h"
#include "GameState.h"
#include "../InGame/PlayerCharacter.h"
#include "../InGame/Projectile.h"
#include "../InGame/EnemyCharacter.h"
#include "../InGame/Background.h"
#include "../Utils/Camera.h"
#include <vector>
#include "../InGame/EnemyData.h"
#include "../InGame/EnemyBoss.h"
#include "../InGame/Item.h"
#include "../InGame/ItemDatabase.h"
#include "../InGame/Stage.h"
#include "../InGame/ItemResourceManager.h"

namespace Manager
{
	class Playing : public AGameState
	{
	public:
		void Init() override;
		void Update() override;
		void Draw() override;
		void Destroy() override;

		InGame::PlayerCharacter* PC;
		std::vector<InGame::Projectile*> PPPool;
		std::vector<InGame::EnemyCharacter*> ECPool;
		std::vector<InGame::Projectile*> EPPool;
		std::vector<InGame::Projectile*> PPs;
		std::vector<InGame::EnemyCharacter*> ECs;
		std::vector<InGame::Projectile*> EPs;
		InGame::Background* BG;
		InGame::ItemDatabase* ITDB;
		InGame::ItemResourceManager* ITRM;
		
		bool bIsBossFight = false;
		u8 WaveCount = 0;
		f32 WaveTimer;
		u8 SpawnCount;
		InGame::EnemyType SpawningEnemyType;
		InGame::MinionData MinionStruct;
		InGame::ArcherData ArcherStruct;
		void SpawnWave();
		void ClearWave();
		void InitBossFight();
		void FinishBossFight();
		InGame::EnemyBoss* Boss = nullptr;
		InGame::Stage* CurrentStage = nullptr;
		InGame::StageType CurrentStageType;
		void InitStage();
	};
	extern Utils::Camera* CAM;
	
}