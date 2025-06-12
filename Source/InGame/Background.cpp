#include "Background.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"

namespace InGame
{
	void Background::Init()
	{
		AEVec2Set(&size, static_cast<f32>(global::ScreenWidth) * 2.f, static_cast<f32>(global::ScreenHeight) * 2.f); //to do
		AEVec2Set(&position, 0.f, 0.f);
		Mesh = Utils::CreateMesh();
		//Texture = AEGfxTextureLoad(""); to do add map texture
	}

	void Background::Update()
	{
		//to do Update texture when switching stages.
	}

	void Background::Draw()
	{
		Utils::DrawObject(*this);
	}

	void Background::Destroy()
	{
		Utils::DestroyMesh(Mesh);
		AEGfxTextureUnload(Texture);
	}
}