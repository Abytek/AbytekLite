#include "Abytek/Windows/WindowManager.hpp"
#include "Abytek/Windows/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/LLEvents/Windows/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsWindowManager::Build()
    {
        A_WindowManager::Build();

        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();

        auto& WinAPIProcessMsgEvent = LLEvents->GetOnWinAPIProcessMsgEvent();
        _OnProcessMsgEvent = WinAPIProcessMsgEvent.AddListener(
            [this](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, TF_Optional<HRESULT>& ResultOpt)
            {
                _ProcessMessage(hwnd, uMsg, wParam, lParam, ResultOpt);
            }
        );
    }
    void F_WindowsWindowManager::Release()
    {
        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();

        auto& WinAPIProcessMsgEvent = LLEvents->GetOnWinAPIProcessMsgEvent();
        WinAPIProcessMsgEvent.RemoveListener(_OnProcessMsgEvent);
        
        A_WindowManager::Release();
    }

    void F_WindowsWindowManager::OnUpdate()
    {
        A_WindowManager::OnUpdate();
    }
    void F_WindowsWindowManager::OnPostUpdate()
    {
        // Send commands to make win msg
        for (const auto& Window : GetWindows())
        {
            auto CastedWindow = Window.FastCast<F_WindowsWindow>();
            
            const auto& CurrentState = Window->GetCurrentState();
            const auto& NextState = Window->GetNextState();
            
            if (CurrentState.IsAcceptedToDestroy)
            {
                CastedWindow->WindowsDestroyWindow();
                continue;
            }
            if (CurrentState.Offset != NextState.Offset)
            {
                CastedWindow->WindowsMove(NextState.Offset);
            }
            if (CurrentState.Size != NextState.Size)
            {
                CastedWindow->WindowsResize(NextState.Size);
            }
            if (
                (CurrentState.IsVisible != NextState.IsVisible)
                || (CurrentState.SizeMode != NextState.SizeMode)
            )
            {
                CastedWindow->WindowsShowWindow(NextState.IsVisible, NextState.SizeMode);
            }
            if (CurrentState.Focus != NextState.Focus)
            {
                CastedWindow->WindowsSetFocus(NextState.Focus);
            }
        }
        
        A_WindowManager::OnPostUpdate();
    }

    void F_WindowsWindowManager::_ProcessMessage(HWND WinAPIHandle, UINT UMsg, WPARAM WParam, LPARAM LParam, TF_Optional<HRESULT>& ResultOpt)
    {
        if (UMsg == WM_DESTROY)
        {
            return;
        }
        if (UMsg == WM_CREATE)
        {
            CREATESTRUCT* CreateP = reinterpret_cast<CREATESTRUCT*>(LParam);
            SetWindowLongPtr(WinAPIHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(CreateP->lpCreateParams));
            return;
        }
        
        TW<F_WindowsWindow> Window;
        if (auto RawWindow = reinterpret_cast<F_WindowsWindow*>(GetWindowLongPtr(WinAPIHandle, GWLP_USERDATA)))
        {
            Window = TW<F_WindowsWindow>(RawWindow);
        }
        if (!Window)
        {
            return;
        }
        
        auto& NextState = Window->InjectNextState();
        
        auto ProcessSizeMessage = [&]()
        {
            /*RECT Rect;
            if(GetWindowRect(WinAPIHandle, &Rect))
            {
                F_Vector2_I32 Size = {
                    static_cast<I32>(Rect.right - Rect.left),
                    static_cast<I32>(Rect.bottom - Rect.top)
                };
                NextState.Size = Size;
            }*/
            
            NextState.Size = F_Vector2_U32 {
                static_cast<U32>(LOWORD(LParam)),
                static_cast<U32>(HIWORD(LParam))
            };

            switch (WParam)
            {
            case SIZE_MAXIMIZED:
                NextState.SizeMode = E_WindowSizeMode::MAXIMIZE;
                break;
            case SIZE_MINIMIZED:
                NextState.SizeMode = E_WindowSizeMode::MINIMIZE;
                break;
            case SIZE_RESTORED:
                NextState.SizeMode = E_WindowSizeMode::NORMAL;
                break;
            default:
                break;
            }
        };

        auto ProcessMoveMessage = [&]()
        {
            F_Vector2_I32 Offset = {
                static_cast<I32>(LOWORD(LParam)),
                static_cast<I32>(HIWORD(LParam))
            };
            NextState.Offset = Offset;
        };
        
        switch (UMsg)
        {
        case WM_CLOSE:
            Window->RequestToDestroy();
            ResultOpt = 0;
            return;
        case WM_SETFOCUS:
            NextState.Focus = true;
            break;
        case WM_KILLFOCUS:
            NextState.Focus = false;
            break;
        case WM_SHOWWINDOW:
            if(WParam == TRUE)
            {
                NextState.IsVisible = true;
            }
            else
            {
                NextState.IsVisible = false;
            }
            break;
        case WM_SIZE:
            ProcessSizeMessage();
            break;
        case WM_MOVE:
            ProcessMoveMessage();
            break;
        }
    }
}
#endif