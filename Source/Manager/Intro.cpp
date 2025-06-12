#include "Intro.h"
#include "../Global/GlobalVariables.h"


f32 Manager::Intro::alpha = 0;
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
		alpha += global::DeltaTime;
	}
	void Intro::Draw()
	{
		Utils::DrawObject(Intro::Splash, alpha);
	}
	void Intro::Destroy()
	{

	}
}