#include "Stage.h"
namespace InGame
{
	Stage1::Stage1()
	{
		Type = StageType::LAND;
		MapTextureAddress = "Assets/Lv1Map.png";
		BossTextureAddress = "Assets/Wizard.png";
	}
	Stage2::Stage2()
	{
		Type = StageType::TOWER;
		MapTextureAddress = "Assets/Lv2Map.png";
		BossTextureAddress = "Assets/Knight.png";
	}
	Stage3::Stage3()
	{
		Type = StageType::HEAVEN;
		MapTextureAddress = "Assets/Lv3Map.png";
		BossTextureAddress = "Assets/Angel.png";
	}
	
}