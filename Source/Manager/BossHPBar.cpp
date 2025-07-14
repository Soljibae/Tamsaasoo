#include "BossHPBar.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
namespace Manager
{
	static f32 w = global::ScreenWidth;
	static f32 h = global::ScreenHeight;
	static f32 padding = 5.f;
	
	void BossHPBar::Init(InGame::EnemyBoss* InBoss)
	{
		Boss = InBoss;
		maxHP = Boss->Stats->MaxHP;
		currHP = Boss->Stats->HP;
		prevHP = currHP;
		BG.position = { 0.f, 350.f };
		BG.size = { w / 3.f, 40.f };
		BG.Texture = AEGfxTextureLoad("Assets/ItemSlots/HPFrame.png");
		BGMesh = Utils::CreateNinePatchMesh();
		HP.position = BG.position;
		HP.size = BG.size;
		HP.Texture = AEGfxTextureLoad("Assets/HP/BossHP.png");
		HP.Mesh = Utils::CreateMesh();
	}

	void BossHPBar::Update()
	{
		currHP = Boss->Stats->HP;
		if (prevHP != currHP)
		{
			prevHP = currHP;
			f32 HPRatio = currHP / maxHP;
			f32 decrease = 1.f - HPRatio;
			AEGfxMeshFree(HP.Mesh);
			HP.Mesh = nullptr;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-0.5f, -0.5f, 0xFFFFFFFF, 0.f, 1.f,
				0.5f - decrease, -0.5f, 0xFFFFFFFF, HPRatio, 1.f,
				-0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f
				);
			AEGfxTriAdd(
				0.5f - decrease, -0.5f, 0xFFFFFFFF, HPRatio, 1.f,
				0.5f - decrease, 0.5f, 0xFFFFFFFF, HPRatio, 0.f,
				-0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f
				);
			HP.Mesh = AEGfxMeshEnd();
		}
	}

	void BossHPBar::Draw()
	{
		Utils::DrawNinePatchMesh(BG, BG.Texture, BGMesh, padding);
		Utils::DrawObject(HP, false);
	}

	void BossHPBar::Destroy()
	{

		AEGfxMeshFree(HP.Mesh);
		HP.Mesh = nullptr;
		AEGfxTextureUnload(HP.Texture);
		HP.Texture = nullptr;
		for (auto mesh : BGMesh)
		{
			AEGfxMeshFree(mesh);
			mesh = nullptr;
		}
		BG.Texture = nullptr;
	}
}