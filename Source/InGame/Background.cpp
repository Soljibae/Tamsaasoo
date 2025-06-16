#include "Background.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"

namespace InGame
{
	void Background::Init()
	{
		AEVec2Set(&size, static_cast<f32>(global::worldMax.x - global::worldMin.x), static_cast<f32>(global::worldMax.y - global::worldMin.y)); 
		AEVec2Set(&position, 0.f, 0.f);
		Mesh = Utils::CreateMesh();
		Texture = AEGfxTextureLoad("Assets/check.jpg");
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