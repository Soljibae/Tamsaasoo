#pragma once
#include "SpawnList.h"
#include <iostream>
#include <sstream>
namespace InGame
{
	void WaveManager::Init()
	{
		fs.open("Assets/SpawnList.csv", std::ios::in);
		GetNextList();
	}
	void WaveManager::GetNextList()
	{
		if (!getline(fs, str_buf)) {
			std::cerr << "Unable to read" << std::endl;
			return;
		}

		std::stringstream ss(str_buf);
		std::getline(ss, CurrList.MinionNum, ',');
		std::getline(ss, CurrList.ArcherNum, ',');
		std::getline(ss, CurrList.DasherNum, ',');
		std::getline(ss, CurrList.TankerNum, ',');
		std::getline(ss, CurrList.BomberNum, ',');
		std::getline(ss, CurrList.ZigZagNum, ',');
		std::getline(ss, CurrList.OrbiterNum, ',');
		std::getline(ss, CurrList.SniperNum, ',');
	}

	void WaveManager::Destroy()
	{
		fs.close();
	}
}