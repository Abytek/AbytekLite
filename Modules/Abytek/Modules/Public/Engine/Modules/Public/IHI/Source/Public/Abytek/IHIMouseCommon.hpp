#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"
#include "Abytek/IHIButton.hpp"


namespace Abytek
{
    class A_IHIMouse;
    
    struct F_IHIMouseState
    {
        B8 WillBeDestroyed = false;
        TF_Set<F_IHIButton, TF_HashBasedLess<F_IHIButton>> PressedButtons;
        I64 DeltaWheel = 0;
        F_Vector2_I32 DeltaPosition = F_Vector2_I32::Zero();
    };
    
    namespace IHIMouseEvents
    {
        using F_OnButtonDown = TF_Event<void(const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)>;
        using F_OnButtonUp = TF_Event<void(const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)>;
        using F_OnWheel = TF_Event<void(I64 Value, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)>;
        using F_OnMove = TF_Event<void(const F_Vector2_I32& DeltaPosition, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)>;
    }
    struct F_IHIMouseEvents
    {
        IHIMouseEvents::F_OnButtonDown OnButtonDown;
        IHIMouseEvents::F_OnButtonUp OnButtonUp;
        IHIMouseEvents::F_OnWheel OnWheel;
        IHIMouseEvents::F_OnMove OnMove;
    };
}