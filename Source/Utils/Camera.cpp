#include "Camera.h"
#include "../Global/GlobalVariables.h"
#include <algorithm>

namespace Utils
{

	void Camera::Init(InGame::Character& player)
	{
		curr_position = player.position;
		target_position = player.position;

		translate_matrix ={{
						{ 1.f, 0.f, -curr_position.x, },
						{ 0.f, 1.f, -curr_position.y },
						{ 0.f, 0.f, 1.f }
			}};

		AEVec2Set(&deadzoneSize, static_cast<f32>(global::ScreenWidth) * 2.f - 200.f, static_cast<f32>(global::ScreenHeight) * 2.f - 200.f); //to do ÀÓ½Ã°ª
	}

	void Camera::Update(InGame::Character& player)
	{
		target_position = player.position;
		curr_position.x += (target_position.x - curr_position.x) * global::DeltaTime * 5.f;
		curr_position.y += (target_position.y - curr_position.y) * global::DeltaTime * 5.f;

		curr_position.x = std::clamp(curr_position.x, -static_cast<f32>(deadzoneSize.x / 2), static_cast<f32>(deadzoneSize.x / 2));
		curr_position.y = std::clamp(curr_position.y, -static_cast<f32>(deadzoneSize.y / 2), static_cast<f32>(deadzoneSize.y / 2));
		
		translate_matrix = { {
					{ 1.f, 0.f, -curr_position.x, },
					{ 0.f, 1.f, -curr_position.y },
					{ 0.f, 0.f, 1.f }
			} };
	}
}