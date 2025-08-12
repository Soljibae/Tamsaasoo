#include "BossHPBar.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
namespace Manager
{
	static f32 w = global::ScreenWidth;
	static f32 h = global::ScreenHeight;
	const static f32 padding = 5.f;
	const static f32 fontSize = 100.f;
	const static f32 nameDrawSize = 0.8f;
	const static f32 epitDrawSize = 0.3f;
	void BossHPBar::Init(InGame::EnemyBoss* InBoss)
	{
		Boss = InBoss;
		maxHP = Boss->Stats->MaxHP;
		currHP = Boss->Stats->HP;
		prevHP = currHP;
		BG.position = { 0.f, h/2.f - 150 };
		BG.size = { w / 3.f, 40.f };
		BG.Texture = AEGfxTextureLoad("Assets/ItemSlots/HPFrame.png");
		BGMesh = Utils::CreateNinePatchMesh();
		HP.position = BG.position;
		HP.size = BG.size;
		HP.Texture = AEGfxTextureLoad("Assets/HP/BossHP.png");
		HP.Mesh = Utils::CreateMesh();
		pFont = AEGfxCreateFont("Assets/Fonts/Herofire.ttf", fontSize);
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
		f32 nw, nh, ew, eh;
		AEGfxGetPrintSize(pFont, Boss->name.c_str(), nameDrawSize, &nw, &nh);
		AEGfxGetPrintSize(pFont, Boss->epithet.c_str(), epitDrawSize, &ew, &eh);
		f32 halfW{ w / 2.f }, halfH{ h / 2.f };
		f32 nameX{ BG.position.x - (BG.size.x / 2.f) },
			nameY{ BG.position.y + (BG.size.y / 2.f) + padding * 3.f },
			epitX{ BG.position.x + (BG.size.x/2.f) - (ew * halfW) },
			epitY{ BG.position.y + (BG.size.y / 2.f) + eh + padding * 3.f };
		AEGfxPrint(pFont, Boss->name.c_str(), nameX / halfW, nameY / halfH, nameDrawSize, 1.f, 0.2f, 0.2f, 1.f);
		AEGfxPrint(pFont, Boss->epithet.c_str(), epitX / halfW, epitY / halfH, epitDrawSize, 1.f, 1.f, 1.f, 0.8f);
	}

	void BossHPBar::Destroy()
	{
		if (pFont)
		{
			AEGfxDestroyFont(pFont);
		}
		if (HP.Mesh)
		{
			AEGfxMeshFree(HP.Mesh);
			HP.Mesh = nullptr;
		}
		if (HP.Texture)
		{
			AEGfxTextureUnload(HP.Texture);
			HP.Texture = nullptr;
		}
		for (auto &mesh : BGMesh)
		{
			AEGfxMeshFree(mesh);
			mesh = nullptr;
		}
		if (BG.Texture)
		{
			AEGfxTextureUnload(BG.Texture);
			BG.Texture = nullptr;
		}
	}
}