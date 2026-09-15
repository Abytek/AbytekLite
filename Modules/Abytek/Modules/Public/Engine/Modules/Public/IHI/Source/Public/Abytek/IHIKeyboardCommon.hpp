#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"
#include "Abytek/IHIButton.hpp"


namespace Abytek
{
    class A_IHIKeyboard;
    
    struct F_IHIKeyboardState
    {
        B8 WillBeDestroyed = false;
        TF_Set<F_IHIButton, TF_HashBasedLess<F_IHIButton>> PressedButtons;
        TF_Vector<F_TextChar> TextChars;
    };
    
    namespace IHIKeyboardEvents
    {
        using F_OnButtonDown = TF_Event<void(const F_IHIButton& Button, const F_IHIKeyboardState& CurrentState, const F_IHIKeyboardState& NewState)>;
        using F_OnButtonUp = TF_Event<void(const F_IHIButton& Button, const F_IHIKeyboardState& CurrentState, const F_IHIKeyboardState& NewState)>;
        using F_OnTextChar = TF_Event<void(F_TextChar TextChar, const F_IHIKeyboardState& CurrentState, const F_IHIKeyboardState& NewState)>;
    }
    struct F_IHIKeyboardEvents
    {
        IHIKeyboardEvents::F_OnButtonDown OnButtonDown;
        IHIKeyboardEvents::F_OnButtonUp OnButtonUp;
        IHIKeyboardEvents::F_OnTextChar OnTextChar;
    };
}