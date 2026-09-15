#include "Abytek/Cursor.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/Windows/Cursor.hpp"
#include "Abytek/Android/Cursor.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_Cursor)       
        auto WindowSubsystem = F_WindowSubsystem::GetInstance();
        switch (WindowSubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
            case E_WindowAPI::WINDOWS:
                return TU<F_WindowsCursor>()();
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
            case E_WindowAPI::ANDROID_:
                return TU<F_AndroidCursor>()();
#endif  
        }   
    ABYTEK_RA_END_OBJECT()
    void A_Cursor::Build()
    {
        A_RAObject::BuildMinimal();
    }
    void A_Cursor::Release()
    {
        A_RAObject::Release();
    }
    
    void A_Cursor::Move(const F_Vector2_I32& NewPosition)
    {
        _NextState.Position = NewPosition;
    }
    void A_Cursor::SetVisibility(B8 Value)
    {
        _NextState.IsVisible = Value;
    }
    void A_Cursor::Capture(const TW<A_Window> Window)
    {
        _NextState.LockedToWindow = Window;
    }

    void A_Cursor::FinalizeNextState()
    {
    }
    void A_Cursor::UpdateCurrentState()
    { 
        auto CurrentState = _CurrentState;
        _CurrentState = _NextState;
        if (_NextState.Position != CurrentState.Position)
        {
            Events.OnMove.Broadcast(CurrentState, _NextState);
        }
        if (_NextState.IsVisible != CurrentState.IsVisible)
        {
            if (_NextState.IsVisible)
            {
                Events.OnVisible.Broadcast(CurrentState, _NextState);
            }
            else 
            {
                Events.OnHidden.Broadcast(CurrentState, _NextState);
            }
        }
        if (_NextState.LockedToWindow && (_NextState.LockedToWindow != CurrentState.LockedToWindow))
        {
            Events.OnLockToWindow.Broadcast(CurrentState, _NextState);
        }
        if (!_NextState.LockedToWindow && (_NextState.LockedToWindow != CurrentState.LockedToWindow))
        {
            Events.OnUnlockFromWindow.Broadcast(CurrentState, _NextState);
        }
    }
}
