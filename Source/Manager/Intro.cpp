#include "Intro.h"
#include "GameManager.h"
#include "../Global/GlobalVariables.h"

namespace Manager
{
	f32 Intro::alpha = 0;

	void Intro::Init()
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);

		Splash.Mesh = Utils::CreateMesh();
		Splash.Texture = AEGfxTextureLoad("Assets/DigiPen_WHITE.png");
		Splash.position = { 0.f, 0.f };
		Splash.size = { 741, 180 };
	}
	void Intro::Update()
	{
		const int xSizeScale{82}, ySizeScale{20};
		sceneTime += global::DeltaTime;
		Splash.size.x += global::DeltaTime * xSizeScale;
		Splash.size.y += global::DeltaTime * ySizeScale;
		if (global::KeyInput(AEVK_SPACE) || global::KeyInput(AEVK_LBUTTON) || global::KeyInput(AEVK_ESCAPE))
		{
			sceneTime = 50;
		}
		if (sceneTime <= halfTime)
		{
			alpha += global::DeltaTime;
		}
		else
		{
			alpha -= global::DeltaTime;
		}
		if (sceneTime >= sceneDuration)
		{
			gm.SetNextGameState(EGameState::PLAYING);
		}
	}
	void Intro::Draw()
	{
		Utils::DrawObject(Splash, false, alpha);
	}
	void Intro::Destroy()
	{
		AEGfxMeshFree(Splash.Mesh);
		AEGfxTextureUnload(Splash.Texture);
	}
}