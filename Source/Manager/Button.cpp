#include "Button.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"

namespace Manager
{
    Button* Button::activeButton{ nullptr };
    // Mouse over, click checking func to mouse callback
    void Button::Init()
    {
        originSize = this->size;
    }
    void Button::Update()
    {
        bool hovered = IsHovered() || isSelected;

        // 1) Hovered && (아직 누가 차지 안 했거나 내가 이미 차지 중)
        if (hovered && (!activeButton || activeButton == this))
            activeButton = this;

        // 2) activeButton 확인 후 크기 조정
        if (activeButton == this)
        {
            // 마우스가 아직 안 떠났으면 스케일, 아니면 리셋
            if (hovered)
            {
                size.x = originSize.x * 1.05f;
                size.y = originSize.y * 1.05f;
            }
            else
            {
                size = originSize;
                activeButton = nullptr;
            }
        }
        else
        {
            // 다른 버튼이 활성화됐으면 항상 원상 복구
            size = originSize;
        }

        if (IsClicked())
        {
            OnClick();
        }
    }

    // Set mouse callback(func required for argument: ex.lambda)
    void Button::SetCallback(std::function<void()> func)
    {
        callback = func;
    }

    // Invoke callback on clicked
    void Button::OnClick()
    {
        this->size.x /= 1.1f;
        this->size.y /= 1.1f;
        this->wasScaled = false;
        isSelected = false;
        activeButton = nullptr;
        if (callback)
            callback();
    }

    // Return true if mouse on button(IsMouseInSquare)
    bool Button::IsHovered()
    {
        return Utils::IsMouseInSquare(*this);
    }

    // Return true if mouse click on button
    bool Button::IsClicked()
    {
        if (AEInputCheckTriggered(AEVK_LBUTTON) && IsHovered())
        {
            isSelected = true;
            activeButton = this;
        }
        if (isSelected && AEInputCheckReleased(AEVK_LBUTTON) && IsHovered())
        {
            return true;
        }
        if (!AEInputCheckCurr(AEVK_LBUTTON))
        {
            isSelected = false;
            activeButton = nullptr;
        }
        return false;
    }
    bool Button::IsSelected()
    {
        if (isSelected && AEInputCheckCurr(AEVK_LBUTTON))
        {
            return true;
        }
        return false;
    }
}