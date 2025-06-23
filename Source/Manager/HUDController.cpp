#include "HUDController.h"
#include "../InGame/Gun.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
namespace Manager
{
	HUDController HUD;
	AEGfxVertexList* HUDController::HPMesh = nullptr;
	AEGfxTexture* HUDController::HPTex = nullptr;
	AEGfxTexture* HUDController::HPBGTex = nullptr;
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

		HPMesh = Utils::CreateMesh();
		HPTex = AEGfxTextureLoad("Assets/HPBG.png");
		//HPBG HUD init
		InGame::Actor bgobj;
		bgobj.Texture = HPTex;
		bgobj.Mesh = HPMesh;
		for (int i = 0; i < MaxHP; ++i)
		{
			int row = i;
			int col = i;
			bgobj.position = {
				startX + col * (actorWidth + spacingX),
				Y
			};
			bgobj.size = { actorWidth, actorHeight};
			HPBG.push_back(bgobj);
		}
		//HP HUD init
		HPBGTex = AEGfxTextureLoad("Assets/HP.png");
		InGame::Actor hpobj;
		hpobj.Texture = HPBGTex;
		hpobj.Mesh = HPMesh;
		for (int i = 0; i < currentHP; i++)
		{
			hpobj.position = HPBG[i].position;
			hpobj.size = { actorWidth, actorHeight };
			HP.push_back(hpobj);
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
		AEGfxMeshFree(HPMesh);
		AEGfxTextureUnload(HPTex);
		AEGfxTextureUnload(HPBGTex);
	}
}