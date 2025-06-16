#pragma once
#include "../InGame/Actor.h"
#include <functional>
namespace Manager
{
	class Button : public InGame::Actor
	{
	public:
		void Update();
		bool IsHovered();
		bool IsClicked();
		void SetCallback(std::function<void()> func);
		void OnClick();
	private:
		std::function<void()> callback;
		bool wasHovered = false;
	};
}