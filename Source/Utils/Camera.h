#pragma once
#include "AEEngine.h"
#include "../InGame/Character.h"

namespace Utils
{
	struct Camera
	{
		AEVec2 curr_position;
		AEVec2 target_position;
		AEMtx33 translate_matrix;
		AEVec2 deadzoneSize;

		void Init(InGame::Character& player);
		void Update(InGame::Character& player);
	};
}