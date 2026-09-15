#include "Abytek/IHIKeyboard.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/Windows/IHIKeyboard.hpp"
#include "Abytek/Android/IHIKeyboard.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_IHIKeyboard)
        auto& IHISubsystem = F_IHISubsystem::GetInstance();
        switch (IHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
        case E_IHIAPI::WINDOWS:
            return TU<F_WindowsIHIKeyboard>()();
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
        case E_IHIAPI::ANDROID_:
            return TU<F_AndroidIHIKeyboard>()();
#endif
        }
    ABYTEK_RA_END_OBJECT()
    void A_IHIKeyboard::Build()
    {
        A_RAObject::BuildMinimal();
    }
    void A_IHIKeyboard::Release()
    {
        A_RAObject::Release();
    }

    void A_IHIKeyboard::FinalizeNextState()
    {
    }
    void A_IHIKeyboard::UpdateCurrentState()
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
        for (auto TextChar : _NextState.TextChars)
        {
            Events.OnTextChar.Broadcast(TextChar, CurrentState, _NextState);
        }
            
        _CurrentState.TextChars = {};
        _NextState.TextChars = {};
    }
}
