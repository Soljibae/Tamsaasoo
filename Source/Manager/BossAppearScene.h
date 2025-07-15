#pragma once
#include "Playing.h"
#include <array>
namespace Manager
{
	class BossAppearScene
	{
	public:
		void Init(InGame::EnemyBoss* InBoss);
		void Update();
		void Draw();
		void Destroy();
		InGame::EnemyBoss* Boss;
		s8 updateFrame;
		std::array<InGame::Actor, 2> Letterboxs;
	};
}