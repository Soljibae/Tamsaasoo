#include "MainMenu.h"
#include "../Utils/Utils.h"
#include "GameManager.h"
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

		button.Mesh = Utils::CreateMesh();
		button.Texture = AEGfxTextureLoad("Assets/StartButton.png");
		button.position = { 0.f, 100.f };
		button.size = { 300.f, 300.f };
	}
	void MainMenu::Update()
	{
		fadeInTime += global::DeltaTime;
		if (fadeInTime < fadeInDuration)
		{
			alpha = 1.0f - (fadeInTime / fadeInDuration);
		}
		else
		{
			alpha = 0;
			if (Utils::IsMouseInSquare(button))
			{
				if (global::KeyInput(AEVK_LBUTTON))
				{
					gm.SetNextGameState(EGameState::PLAYING);
				}

			}
		}
	}
	void MainMenu::Draw()
	{
		Utils::DrawObject(Illust, false);
		Utils::DrawObject(button, false);
		if (alpha != 0)
		{
			Utils::DrawObject(Black, false, alpha);
		}
	}
	void MainMenu::Destroy()
	{
		AEGfxMeshFree(Illust.Mesh);
		AEGfxTextureUnload(Illust.Texture);

		AEGfxMeshFree(Black.Mesh);
		AEGfxTextureUnload(Black.Texture);

		AEGfxMeshFree(button.Mesh);
		AEGfxTextureUnload(button.Texture);
	}
}