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
		bool IsSelected();
		void SetCallback(std::function<void()> func);
		void OnClick();
		static Button* activeButton;
	private:
		std::function<void()> callback;
		bool wasScaled = false;
		bool isSelected = false;
	};
}