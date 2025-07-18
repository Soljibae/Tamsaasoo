#include "ExpUI.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "GameOver.h"
#include <sstream>
namespace Manager
{
	ExpUI ExpPanel;

	void ExpUI::Init(InGame::PlayerCharacter* InPC)
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		f32 space = 3.f;

		pFont = AEGfxCreateFont("Assets/Fonts/buggy-font.ttf", fontSize);

		xpBarBackground.Mesh = Utils::CreateMesh();
		xpBarBackground.Texture = AEGfxTextureLoad("Assets/black.png");
		xpBarBackground.size = { 240.f , 40.f};
		xpBarBackground.position = { -(w / 2) + 180.f + 120.f , (h / 2) - 90.f - xpBarBackground.size.y};

		xpBarEmpty.Mesh = Utils::CreateMesh();
		xpBarEmpty.Texture = AEGfxTextureLoad("Assets/white.png");
		xpBarEmpty.size = { xpBarBackground.size.x - space * 2, xpBarBackground.size.y - space * 2 };
		xpBarEmpty.position = { xpBarBackground.position.x , xpBarBackground.position.y};

		xpBarFill.Mesh = Utils::CreateMesh();
		xpBarFill.Texture = AEGfxTextureLoad("Assets/green.png");
		xpBarFill.size = { 0, xpBarBackground.size.y - space * 2 };
		xpBarFill.position = { 0.f , xpBarBackground.position.y };

		PC = InPC;
	}
	void ExpUI::Update()
	{
		if (gameOverScreen.isGameOver)
			return;
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 space = 2.f;

		xpBarFill.size.x = xpBarEmpty.size.x * PC->PS->ExpCount / PC->PS->TargetExp;
		xpBarFill.position.x = xpBarBackground.position.x - xpBarEmpty.size.x / 2.f + xpBarFill.size.x / 2.f;
	}
	void ExpUI::Draw()
	{
		if (gameOverScreen.isGameOver)
			return;
		Utils::DrawObject(xpBarBackground, false);
		Utils::DrawObject(xpBarEmpty, false, 0.5f);
		Utils::DrawObject(xpBarFill, false);
		
		f32 padding = 5.f;

		std::ostringstream os;

		os << "Lv." << PC->PS->Level;
		f32 textW, textH;
		AEGfxGetPrintSize(pFont, os.str().c_str(), textDrawSize, &textW, &textH);

		f32 wh = global::ScreenWidth / 2.f;
		f32 hh = global::ScreenHeight / 2.f;
		
		AEGfxPrint(pFont, os.str().c_str(), (xpBarBackground.position.x - xpBarBackground.size.x / 2.f + padding) / wh,
			(xpBarBackground.position.y - xpBarBackground.size.y / 2.f) / hh + textH, textDrawSize, 1.f, 1.f, 1.f, 1.f);
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