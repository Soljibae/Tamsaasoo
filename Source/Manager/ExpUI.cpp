#include "ExpUI.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"

namespace Manager
{
	ExpUI ExpPanel;

	void ExpUI::Init(InGame::PlayerCharacter* InPC)
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		f32 space = 2.f;

		xpBarBackground.Mesh = Utils::CreateMesh();
		xpBarBackground.Texture = AEGfxTextureLoad("Assets/black.png");
		xpBarBackground.size = { w , 10.f};
		xpBarBackground.position = { 0.f , - h / 2.f + xpBarBackground.size.y / 2.f};

		xpBarEmpty.Mesh = Utils::CreateMesh();
		xpBarEmpty.Texture = AEGfxTextureLoad("Assets/white.png");
		xpBarEmpty.size = { w - space * 2, 10.f - space };
		xpBarEmpty.position = { 0.f , -h / 2.f + xpBarBackground.size.y / 2.f - space};

		xpBarFill.Mesh = Utils::CreateMesh();
		xpBarFill.Texture = AEGfxTextureLoad("Assets/green.png");
		xpBarFill.size = { 0, 10.f - space };
		xpBarFill.position = { 0.f , -h / 2.f + xpBarBackground.size.y / 2.f - space };

		PC = InPC;
	}
	void ExpUI::Update()
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 space = 2.f;

		xpBarFill.size.x = xpBarEmpty.size.x * PC->Stats.ExpCount / PC->Stats.TargetExp;
		xpBarFill.position.x = -w / 2 + space + xpBarFill.size.x / 2.f;
	}
	void ExpUI::Draw()
	{
		Utils::DrawObject(xpBarBackground, false);
		Utils::DrawObject(xpBarEmpty, false, 0.5f);
		Utils::DrawObject(xpBarFill, false);
	}
	void ExpUI::Destroy()
	{
		Utils::DestroyMesh(xpBarBackground.Mesh);
		AEGfxTextureUnload(xpBarBackground.Texture);

		Utils::DestroyMesh(xpBarEmpty.Mesh);
		AEGfxTextureUnload(xpBarEmpty.Texture);

		Utils::DestroyMesh(xpBarFill.Mesh);
		AEGfxTextureUnload(xpBarFill.Texture);
	}
}