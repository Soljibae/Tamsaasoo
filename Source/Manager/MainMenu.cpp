#include "MainMenu.h"

namespace Manager
{
	f32 MainMenu::alpha = 1.f;

	void MainMenu::Init()
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);

		Illust.Mesh = Utils::CreateMesh();
		Illust.Texture = AEGfxTextureLoad("Assets/MainTemp.png");
		Illust.position = { 0.f, 0.f };
		Illust.size = { w, h };

		Black.Mesh = Utils::CreateMesh();
		Black.Texture = AEGfxTextureLoad("Assets/Black.png");
		Black.position = { 0.f, 0.f };
		Black.size = { w, h };

	}
	void MainMenu::Update()
	{
		if (alpha > 0.f)
		{
			alpha -= global::DeltaTime;
		}
		else
		{
			alpha = 0;
		}
	}
	void MainMenu::Draw()
	{
		Utils::DrawObject(Illust, false);

		if (alpha != 0)
		{
			Utils::DrawObject(Black, false, alpha);
		}
	}
	void MainMenu::Destroy()
	{
		AEGfxMeshFree(Illust.Mesh);
		AEGfxMeshFree(Black.Mesh);
		AEGfxTextureUnload(Illust.Texture);
		AEGfxTextureUnload(Black.Texture);
	}
}