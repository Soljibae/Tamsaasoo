#pragma once
#include <string>
namespace InGame
{
	enum StageType
	{
		LAND,
		TOWER,
		HEAVEN,
	};

	class Stage
	{
	public:
		StageType Type;
		std::string MapTextureAddress;
		std::string BossTextureAddress;
	};

	class Stage1 : public Stage
	{
	public:
		Stage1();
	};

	class Stage2 : public Stage
	{
	public:
		Stage2();
	};

	class Stage3 : public Stage
	{
	public:
		Stage3();
	};
}