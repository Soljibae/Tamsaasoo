#pragma once
#include "AEEngine.h"
#include "EnemyData.h"
#include <string>
#include <fstream>
namespace InGame
{
	struct SpawnList
	{
	public:
		std::string MinionNum;
		std::string ArcherNum;
		std::string DasherNum;
		std::string TankerNum;
		std::string BomberNum;
		std::string ZigZagNum;
		std::string OrbiterNum;
		std::string SniperNum;
		std::string BurnerNum;
		std::string HolerNum;
	};

	class WaveManager
	{
	public:
		SpawnList CurrList;
		std::string str_buf;
		std::fstream fs;
		void Init();
		void GetNextList();
		void Destroy();
	};
}