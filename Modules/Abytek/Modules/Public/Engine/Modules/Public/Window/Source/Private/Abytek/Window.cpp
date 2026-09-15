#include "Abytek/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/Windows/Window.hpp"
#include "Abytek/Android/Window.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_Window)     
        auto WindowSubsystem = F_WindowSubsystem::GetInstance();
        switch (WindowSubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
            case E_WindowAPI::WINDOWS:
                return TU<F_WindowsWindow>()();
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
            case E_WindowAPI::ANDROID_:
                return TU<F_AndroidWindow>()();
#endif
        }
    ABYTEK_RA_END_OBJECT()
    void A_Window::Build(const F_WindowBuildParams& Params)
    {
        A_RAObject::BuildMinimal();
        _NextState.Title = Params.Title;
        _NextState.Offset = Params.Offset;
        _NextState.Size = Params.Size;
        _NextState.SizeMode = Params.SizeMode;
        _NextState.Focus = Params.Focus;
        _NextState.IsVisible = Params.IsVisible;
    }
    void A_Window::Release()
    {
        A_RAObject::Release();
    }

    void A_Window::RequestToDestroy()
    {
        _NextState.IsRequestedToDestroy = true;
    }
    void A_Window::AcceptToDestroy()
    {
        ABYTEK_ENGINE_RHI_ASSERT(_CurrentState.IsRequestedToDestroy) << "Cannot accept to destroy without requesting to destroy the window";
        _NextState.IsAcceptedToDestroy = true;
    }
    void A_Window::SetTitle(const F_Text& Value)
    {
        _NextState.Title = Value;
    }
    void A_Window::Move(const F_Vector2_I32& NewOffset)
    {
        _NextState.Offset = NewOffset;
    }
    void A_Window::Resize(const F_Vector2_U32& Value)
    {
        _NextState.Size = Value;
    }
    void A_Window::SetVisibility(B8 Value)
    {
        _NextState.IsVisible = Value;
    }
    void A_Window::SetFocus(B8 Value)
    {
        _NextState.Focus = Value;
    }
    void A_Window::SetSizeMode(E_WindowSizeMode Value)
    {
        _NextState.SizeMode = Value;
    }

    void A_Window::FinalizeNextState()
    {
    }
    void A_Window::UpdateCurrentState()
    { 
        auto CurrentState = _CurrentState;
        _CurrentState = _NextState;
        if (_NextState.IsRequestedToDestroy && !CurrentState.IsRequestedToDestroy && !_NextState.IsAcceptedToDestroy)
        {
            B8 Accept = true;
            Events.OnRequestDestroy.Broadcast(CurrentState, _NextState, Accept);
            _NextState.IsAcceptedToDestroy = Accept;
            _CurrentState.IsAcceptedToDestroy = Accept;
        }
        if (_NextState.IsAcceptedToDestroy && !CurrentState.IsAcceptedToDestroy)
        {
            Events.OnDestroy.Broadcast(CurrentState, _NextState);
        }
        if (_NextState.Title != CurrentState.Title)
        {
            Events.OnTitleChanged.Broadcast(CurrentState, _NextState);
        }
        if (_NextState.Offset != CurrentState.Offset)
        {
            Events.OnMove.Broadcast(CurrentState, _NextState);
        }
        if (_NextState.SizeMode != CurrentState.SizeMode)
        {
            Events.OnSizeModeChanged.Broadcast(CurrentState, _NextState);
        }
        if (_NextState.Size != CurrentState.Size)
        {
            Events.OnResize.Broadcast(CurrentState, _NextState);
        }
        if (_NextState.Focus != CurrentState.Focus)
        {
            if (_NextState.Focus)
            {
                Events.OnFocus.Broadcast(CurrentState, _NextState);
            }
            else {
                Events.OnLostFocus.Broadcast(CurrentState, _NextState);
            }
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
    }
}
