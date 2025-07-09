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
        if(IsHovered() || isSelected)
            activeButton = this;
        bool shouldScale = (IsHovered() || isSelected) && this == activeButton;

        if (shouldScale)
        {
            size.x = originSize.x * 1.05f;
            size.y = originSize.y * 1.05f;
        }
        else if (!isSelected)
        {
            size = originSize;
            activeButton = nullptr;
        }

        if (IsClicked() && activeButton == this)
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
        this->size = originSize;
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