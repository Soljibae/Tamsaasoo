#pragma once
#include "AEEngine.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "EnemyCharacter.h"
#include <vector>
namespace InGame
{
	class Game
	{
	public:
		void Init();
		void Update();
		void Draw();
		void Destroy();

		PlayerCharacter* PC;
		std::vector<Projectile*> PPs;
		std::vector<EnemyCharacter*> ECs;
		std::vector<Projectile*> EPs;
	};
}
