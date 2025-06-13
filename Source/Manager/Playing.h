#pragma once
#include "AEEngine.h"
#include "GameState.h"
#include "../InGame/PlayerCharacter.h"
#include "../InGame/Projectile.h"
#include "../InGame/EnemyCharacter.h"
#include "../Utils/Camera.h"
#include <vector>
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
		std::vector<InGame::Projectile*> PPs;
		std::vector<InGame::EnemyCharacter*> ECs;
		std::vector<InGame::Projectile*> EPs;

		f32 WaveTimer;
		u8 SpawnCount;
		void SpawnWave();
	};
	extern Utils::Camera* CAM;
}