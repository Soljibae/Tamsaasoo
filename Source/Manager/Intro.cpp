#include "Intro.h"
#include "../Global/GlobalVariables.h"

namespace Manager
{
	void Intro::Init()
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		Intro::Splash.Mesh = Utils::CreateMesh();
		Intro::Splash.Texture = AEGfxTextureLoad("Assets/DigiPen_WHITE.png");
		Intro::Splash.position = { 0.f, 0.f };
		Intro::Splash.size = { 1026, 249 };
	}
	void Intro::Update()
	{
		//static f32 alpha = 0.f;
		//alpha += global::DeltaTime;
		//if (alpha >= 1.f)
		//	alpha = 1.f;
	}
	void Intro::Draw()
	{
		Utils::DrawObject(Intro::Splash);
	}
	void Intro::Destroy()
	{

	}
}