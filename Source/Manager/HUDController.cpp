#include "HUDController.h"
#include "../InGame/Gun.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
namespace Manager
{
	HUDController HUD;
	void HUDController::Init(InGame::PlayerCharacter* InPC, InGame::Gun* InGUN)
	{
		PC = InPC;
		GUN = InGUN;
		MaxHP = PC->Stats.MaxHP;
		currentHP = PC->Stats.HP;
		fireTimer = GUN->FireTimer;

		int w = global::ScreenWidth;
		int h = global::ScreenHeight;
		const float actorWidth = 60.f;
		const float actorHeight = 60.f;
		const float spacingX = 15.0f; // 가로 간격
		const float startX = -(w / 2) + 100.f;
		const float Y = (h / 2) - 100.f;
		//HPBG HUD init
		for (int i = 0; i < MaxHP; ++i)
		{
			int row = i;
			int col = i;
			InGame::Actor obj;
			obj.position = {
				startX + col * (actorWidth + spacingX),
				Y
			};
			obj.size = { actorWidth, actorHeight};
			obj.Mesh = Utils::CreateMesh();
			obj.Texture = AEGfxTextureLoad("Assets/HPBG.png");
			HPBG.push_back(obj);
		}
		//HP HUD init
		for (int i = 0; i < currentHP; i++)
		{
			InGame::Actor obj;
			obj.position = HPBG[i].position;
			obj.size = { actorWidth, actorHeight };
			obj.Mesh = Utils::CreateMesh();
			obj.Texture = AEGfxTextureLoad("Assets/HP.png");
			HP.push_back(obj);
		}
	}
	void HUDController::Update()
	{
		
		while (currentHP > PC->Stats.HP)
		{
			if(!HP.empty())
				HP.pop_back();
			currentHP--;
		}
		currentHP = PC->Stats.HP;
		fireTimer = GUN->FireTimer;
	}
	void HUDController::Draw()
	{
		for(int i = 0; i < HPBG.size(); i++)
			Utils::DrawObject(HPBG[i], false);
		for (int i = 0; i < HP.size(); i++)
			Utils::DrawObject(HP[i], false);
	}
	void HUDController::Destroy()
	{
		HPBG.clear();
		HP.clear();
	}
}