#include "Button.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"

namespace Manager
{
    // Mouse over, click checking func to mouse callback
    void Button::Update()
    {
        if (IsHovered() && !wasHovered)
        {
            this->size.x *= 1.1f;
            this->size.y *= 1.1f;
            wasHovered = true;
        }
        else if (!IsHovered() && wasHovered)
        {
            this->size.x /= 1.1f;
            this->size.y /= 1.1f;
            wasHovered = false;
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
        return IsHovered() && global::KeyInput(AEVK_LBUTTON);
    }
}