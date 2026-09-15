#include "Abytek/IHIMouse.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/Windows/IHIMouse.hpp"
#include "Abytek/Android/IHIMouse.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_IHIMouse)
        auto& IHISubsystem = F_IHISubsystem::GetInstance();
        switch (IHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
        case E_IHIAPI::WINDOWS:
            return TU<F_WindowsIHIMouse>()();
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
        case E_IHIAPI::ANDROID_:
            return TU<F_AndroidIHIMouse>()();
#endif
        }
    ABYTEK_RA_END_OBJECT()
    void A_IHIMouse::Build()
    {
        A_RAObject::BuildMinimal();
    }
    void A_IHIMouse::Release()
    {
        A_RAObject::Release();
    }

    void A_IHIMouse::FinalizeNextState()
    {
    }
    void A_IHIMouse::UpdateCurrentState()
    {
        auto CurrentState = _CurrentState;
        _CurrentState = _NextState;
        if (_NextState.PressedButtons != CurrentState.PressedButtons)
        {
            for (const auto& PressedButton : _NextState.PressedButtons)
            {
                if (CurrentState.PressedButtons.find(PressedButton) == CurrentState.PressedButtons.end())
                {
                    Events.OnButtonDown.Broadcast(PressedButton, CurrentState, _NextState);
                }
            }
            for (const auto& PressedButton : CurrentState.PressedButtons)
            {
                if (_NextState.PressedButtons.find(PressedButton) == _NextState.PressedButtons.end())
                {
                    Events.OnButtonUp.Broadcast(PressedButton, CurrentState, _NextState);
                }
            }
        }
        if (_NextState.DeltaWheel != 0)
        {
            Events.OnWheel.Broadcast(_NextState.DeltaWheel, CurrentState, _NextState);
        }
        if (_NextState.DeltaPosition != F_Vector2_I32::Zero())
        {
            Events.OnMove.Broadcast(_NextState.DeltaPosition, CurrentState, _NextState);
        }
            
        _CurrentState.DeltaPosition = F_Vector2_I32::Zero();
        _NextState.DeltaPosition = F_Vector2_I32::Zero();
        _CurrentState.DeltaWheel = 0;
        _NextState.DeltaWheel = 0;
    }
}
