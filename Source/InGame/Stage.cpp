#include "Stage.h"
namespace InGame
{
	Stage1::Stage1()
	{
		Type = StageType::LAND;
		MapTextureAddress = "Assets/check.jpg";
		BossTextureAddress = "Assets/PlanetTexture.png";
	}
	Stage2::Stage2()
	{
		Type = StageType::TOWER;
		MapTextureAddress = "Assets/Stage2.png";
		BossTextureAddress = "Assets/test.png";
	}
	Stage3::Stage3()
	{
		Type = StageType::HEAVEN;
		MapTextureAddress = "Assets/check.jpg";
		BossTextureAddress = "Assets/TestBlankRed.png";
	}
	
}