#include "Camera.h"

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
	}

	void Camera::Update(InGame::Character& player)
	{
		position = player.position;

		translate_matrix = { {
						{ 1.f, 0.f, 0.f },
						{ 0.f, 1.f, 0.f },
						{ -position.x, -position.y, 1.f  }
			} };
	}
}