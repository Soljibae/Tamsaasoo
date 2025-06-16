#include "PauseUI.h"
#include "Playing.h"
#include "GameManager.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
namespace Manager
{
	PauseUI pausePanel;

	void PauseUI::Init()
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);

		resumeButton.Mesh = Utils::CreateMesh();
		resumeButton.Texture = AEGfxTextureLoad("Assets/ResumeButton.png");
		resumeButton.position = {0, 100};
		resumeButton.size = {300, 120};
		resumeButton.SetCallback([]() { gm.Resume(); });

		mainmenuButton.Mesh = Utils::CreateMesh();
		mainmenuButton.Texture = AEGfxTextureLoad("Assets/MainMenuButton.png");
		mainmenuButton.position = {0, -100};
		mainmenuButton.size = {300, 120};
		mainmenuButton.SetCallback([]() { gm.SetNextGameState(EGameState::MAINMENU); });

		pauseDimmer.Mesh = Utils::CreateMesh();
		pauseDimmer.Texture = AEGfxTextureLoad("Assets/black.png");
		pauseDimmer.position = {0, 0};
		pauseDimmer.size = {w, h};

	}
	void PauseUI::Update()
	{
		resumeButton.Update();
	}
	void PauseUI::Draw()
	{
		mainmenuButton.Update();
		Utils::DrawObject(pauseDimmer, false, 0.5f);
		Utils::DrawObject(resumeButton, false);
		Utils::DrawObject(mainmenuButton, false);
	}
	void PauseUI::Destroy()
	{
		AEGfxMeshFree(resumeButton.Mesh);
		AEGfxTextureUnload(resumeButton.Texture);

		AEGfxMeshFree(mainmenuButton.Mesh);
		AEGfxTextureUnload(mainmenuButton.Texture);

		AEGfxMeshFree(pauseDimmer.Mesh);
		AEGfxTextureUnload(pauseDimmer.Texture);
	}
}