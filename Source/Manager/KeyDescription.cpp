#pragma once
#include "KeyDescription.h"
#include "../Utils/Utils.h"
#include "GameManager.h"
#include "../Global/GlobalVariables.h"
namespace Manager
{
	void KeyDescription::Init()
	{
        DescriptionImage = new InGame::Actor;
        DescriptionImage->Mesh = Utils::CreateMesh(1, 1);
        DescriptionImage->Texture = AEGfxTextureLoad("Assets/KeyDescription.png");
        PressAnyKeyImage = new InGame::Actor;
        PressAnyKeyImage->Mesh = Utils::CreateMesh(1, 1);
        PressAnyKeyImage->Texture = AEGfxTextureLoad("Assets/AnyKeyToStart.png");
        PressAnyKeyImage->size.x = 425.f;
        PressAnyKeyImage->size.y = 46.f;

	}
	void KeyDescription::Update()
	{
		bool bIsPressedAnyKey = false;
        if (AEInputCheckTriggered(AEVK_LBUTTON))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_RBUTTON))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_MBUTTON))
            bIsPressedAnyKey = true;

        // 기능 키
        else if (AEInputCheckTriggered(AEVK_BACK))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_TAB))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_RETURN))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_LSHIFT))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_RSHIFT))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_LCTRL))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_RCTRL))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_LALT))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_RALT))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_PRINTSCREEN))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_SCROLLLOCK))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_PAUSE))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_CAPSLOCK))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_ESCAPE))
            bIsPressedAnyKey = true;

        // 방향키, 스페이스, 탐색키
        else if (AEInputCheckTriggered(AEVK_SPACE))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_PAGEUP))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_PAGEDOWN))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_END))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_HOME))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_LEFT))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_UP))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_RIGHT))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_DOWN))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_INSERT))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_DELETE))
            bIsPressedAnyKey = true;

        // 숫자키
        else if (AEInputCheckTriggered(AEVK_0))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_1))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_2))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_3))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_4))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_5))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_6))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_7))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_8))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_9))
            bIsPressedAnyKey = true;

        // 알파벳키
        else if (AEInputCheckTriggered(AEVK_A))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_B))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_C))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_D))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_E))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_G))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_H))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_I))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_J))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_K))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_L))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_M))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_N))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_O))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_P))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_Q))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_R))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_S))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_T))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_U))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_V))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_W))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_X))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_Y))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_Z))
            bIsPressedAnyKey = true;

        // 넘패드
        else if (AEInputCheckTriggered(AEVK_NUMPAD0))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMPAD1))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMPAD2))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMPAD3))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMPAD4))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMPAD5))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMPAD6))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMPAD7))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMPAD8))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMPAD9))
            bIsPressedAnyKey = true;

        // 넘패드 연산 키
        else if (AEInputCheckTriggered(AEVK_NUM_MULTIPLY))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUM_PLUS))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUM_MINUS))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUM_PERIOD))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUM_DIVIDE))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_NUMLOCK))
            bIsPressedAnyKey = true;

        // F1 ~ F12
        else if (AEInputCheckTriggered(AEVK_F1))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F2))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F3))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F4))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F5))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F6))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F7))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F8))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F9))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F10))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F11))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_F12))
            bIsPressedAnyKey = true;

        // 특수문자
        else if (AEInputCheckTriggered(AEVK_SEMICOLON))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_SLASH))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_BACKQUOTE))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_LBRACKET))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_BACKSLASH))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_RBRACKET))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_QUOTE))
            bIsPressedAnyKey = true;

        else if (AEInputCheckTriggered(AEVK_EQUAL))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_MINUS))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_PERIOD))
            bIsPressedAnyKey = true;
        else if (AEInputCheckTriggered(AEVK_COMMA))
            bIsPressedAnyKey = true;

        if (bIsPressedAnyKey)
        {
            Manager::gm.SetNextGameState(EGameState::PLAYING);
        }
        else
        {
            DescriptionImage->size.x = global::ScreenWidth;
            DescriptionImage->size.y = global::ScreenHeight;
            PressAnyKeyImage->position.y = -(global::ScreenHeight - PressAnyKeyImage->size.y) / 2.f + 100.f;
            Timer += global::DeltaTime;
            PressAnyKeyImage->Alpha = (std::sin(Timer) + 1.0f) * 0.5f;
        }
	}
	void KeyDescription::Draw()
	{
        Utils::DrawObject(*DescriptionImage, false,1.f);
        Utils::DrawObject(*PressAnyKeyImage, false, PressAnyKeyImage->Alpha);
	}
	void KeyDescription::Destroy()
	{
        if (DescriptionImage->Mesh)
        {
            Utils::DestroyMesh(DescriptionImage->Mesh);
        }
        if (DescriptionImage->Texture)
        {
            AEGfxTextureUnload(DescriptionImage->Texture);
        }
        if (PressAnyKeyImage->Mesh)
        {
            Utils::DestroyMesh(PressAnyKeyImage->Mesh);
        }
        if (PressAnyKeyImage->Texture)
        {
            AEGfxTextureUnload(PressAnyKeyImage->Texture);
        }
	}
    
}