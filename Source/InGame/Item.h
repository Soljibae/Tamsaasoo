#pragma once
#include "Actor.h"
#include <string>
#include <map>
#include <memory>
namespace InGame
{
	class Item : public Actor
	{
	public:
		virtual void Init() override;
		virtual void Use(Actor* owner);
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual std::unique_ptr<Item> Clone() const = 0;

		int id;
		std::string name;
		std::string description;
		bool bHasBeenUsed = false;
	};

	class Item_1 : public Item
	{
	public:
		virtual void Init() override;
		virtual void Use(Actor* owner) override;
		virtual void Draw() override;
		virtual void Destroy() override;
		virtual std::unique_ptr<Item> Clone() const override;
	};
}
