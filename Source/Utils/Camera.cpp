#include "Camera.h"
#include "../Global/GlobalVariables.h"

namespace Utils
{

	void Camera::Init(InGame::Character& player)
	{
		position = player.position;

		translate_matrix ={{
						{ 1.f, 0.f, 0.f },
						{ 0.f, 1.f, 0.f },
						{ -position.x, -position.y, 1.f }
			}};

		AEVec2Set(&deadzoneSize, static_cast<f32>(global::ScreenWidth) * 2.f - 200.f, static_cast<f32>(global::ScreenHeight) * 2.f - 200.f); //to do 임시값
	}

	void Camera::Update(InGame::Character& player)
	{
		position = player.position;

		if (-deadzoneSize.x / 2 < position.x && position.x < deadzoneSize.x / 2 &&
			-deadzoneSize.y / 2 < position.y && position.y < deadzoneSize.y / 2)
		{
			translate_matrix = { {
					{ 1.f, 0.f, 0.f },
					{ 0.f, 1.f, 0.f },
					{ -position.x, -position.y, 1.f  }
			} }; //카메라 매트릭스 역행렬을 마우스에 취해야할듯함.
		}
	
	}
}