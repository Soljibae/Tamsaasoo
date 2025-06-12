#include "Camera.h"
#include "../Global/GlobalVariables.h"
#include <algorithm>

namespace Utils
{

	void Camera::Init(InGame::Character& player)
	{
		position = player.position;

		translate_matrix ={{
						{ 1.f, 0.f, -position.x, },
						{ 0.f, 1.f, -position.y },
						{ 0.f, 0.f, 1.f }
			}};

		AEVec2Set(&deadzoneSize, static_cast<f32>(global::ScreenWidth) * 2.f - 200.f, static_cast<f32>(global::ScreenHeight) * 2.f - 200.f); //to do ÀÓ½Ã°ª
	}

	void Camera::Update(InGame::Character& player)
	{
		position = player.position;

		position.x = std::clamp(position.x, -static_cast<f32>(deadzoneSize.x / 2), static_cast<f32>(deadzoneSize.x / 2));
		position.y = std::clamp(position.y, -static_cast<f32>(deadzoneSize.y / 2), static_cast<f32>(deadzoneSize.y / 2));
		
		translate_matrix = { {
					{ 1.f, 0.f, -position.x, },
					{ 0.f, 1.f, -position.y },
					{ 0.f, 0.f, 1.f }
			} };
	}
}