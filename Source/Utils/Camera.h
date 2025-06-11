#pragma once
#include "AEEngine.h"
#include "../InGame/Character.h"

namespace Utils
{
	struct Camera
	{
		AEVec2 position;
		AEMtx33 translate_matrix;
		AEVec2 deadzoneSize;
		AEVec2 worldMin, worldMax;

		void Init(InGame::Character& player);
		void Update(InGame::Character& player);
	};
}