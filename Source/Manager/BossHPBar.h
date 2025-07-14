#pragma once
#include "../InGame/Actor.h"
#include "../InGame/EnemyBoss.h"
#include <array>
namespace Manager
{
	class BossHPBar : public InGame::Actor
	{
	public:
		void Init(InGame::EnemyBoss* InBoss);
		void Update() override;
		void Draw() override;
		void Destroy() override;
		InGame::Actor HP;
		InGame::Actor BG;
		std::array<AEGfxVertexList*, 9> BGMesh;
		InGame::EnemyBoss* Boss;
		f32 prevHP;
		f32 currHP;
		f32 maxHP;
	};
}