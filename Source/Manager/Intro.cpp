#include "Intro.h"
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
		Splash.size = { 1026, 249 };
	}
	void Intro::Update()
	{
		alpha += global::DeltaTime;
	}
	void Intro::Draw()
	{
		Utils::DrawObject(Splash, alpha);
	}
	void Intro::Destroy()
	{
		AEGfxMeshFree(Splash.Mesh);
		Splash.Mesh = nullptr;
		AEGfxTextureUnload(Splash.Texture);
	}
}