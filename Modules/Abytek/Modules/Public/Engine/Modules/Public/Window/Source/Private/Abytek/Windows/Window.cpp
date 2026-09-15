#include "Abytek/Windows/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/LLEvents/Windows/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    int H_WindowsWindow::ParseShowCommand(B8 IsVisible, E_WindowSizeMode SizeMode)
    {            
        int CmdShow;
        if (IsVisible)
        {
            switch (SizeMode)
            {
            case E_WindowSizeMode::NORMAL:
                CmdShow = SW_SHOWNORMAL;
                break;
            case E_WindowSizeMode::MAXIMIZE:
                CmdShow = SW_SHOWMAXIMIZED;
                break;
            case E_WindowSizeMode::MINIMIZE:
                CmdShow = SW_SHOWMINIMIZED;
                break;
            default:
                ABYTEK_ENGINE_WINDOW_ASSERT(false) << "Invalid window size mode";
                break;
            }
        }
        else
        {
            switch (SizeMode)
            {
            case E_WindowSizeMode::NORMAL:
                CmdShow = SW_NORMAL;
                break;
            case E_WindowSizeMode::MAXIMIZE:
                CmdShow = SW_MAXIMIZE;
                break;
            case E_WindowSizeMode::MINIMIZE:
                CmdShow = SW_MINIMIZE;
                break;
            default:
                ABYTEK_ENGINE_WINDOW_ASSERT(false) << "Invalid window size mode";
                break;
            }
        }
        return CmdShow;
    }

    void F_WindowsWindow::Build(const F_WindowBuildParams& Params)
    {
        A_Window::Build(Params);
        
        _CreateWindow();
    }
    void F_WindowsWindow::Release()
    {
        A_Window::Release();
    }

    void F_WindowsWindow::_CreateWindow()
    {
        auto& NextState = InjectNextState();
        
        _WinAPIHandle = CreateWindowEx(
            NULL,
            F_WindowsLLEvents::GetWindowClassNameCStr(),
            GetTitle().c_str(),
            WS_OVERLAPPEDWINDOW,

            // Offset.XY, Size.XY
            NextState.Offset.X, NextState.Offset.Y, NextState.Size.X, NextState.Size.Y,

            NULL,
            NULL,
            NULL,
            this  
        );
        ABYTEK_ENGINE_WINDOW_ASSERT(_WinAPIHandle) << "cannot create window";
        
        {
            F32 ScalarDPI = static_cast<F32>(GetDpiForWindow(_WinAPIHandle)) / 96.0f;
            NextState.DPI = F_Vector2_F32 { ScalarDPI, ScalarDPI };
        }
        
        WindowsShowWindow(NextState.IsVisible, NextState.SizeMode);
        WindowsSetFocus(NextState.Focus);
        WindowsMove(NextState.Offset);
        WindowsResize(NextState.Size);
    }
    void F_WindowsWindow::_DestroyWindow()
    {
        WindowsDestroyWindow();
    }

    void F_WindowsWindow::WindowsDestroyWindow()
    {
        if (_WinAPIHandle)
        {
            DestroyWindow(_WinAPIHandle);
        }
    }
    void F_WindowsWindow::WindowsShowWindow(B8 InIsVisible, E_WindowSizeMode InSizeMode)
    {
        ShowWindow(_WinAPIHandle, H_WindowsWindow::ParseShowCommand(InIsVisible, InSizeMode));
    }
    void F_WindowsWindow::WindowsSetFocus(B8 InValue)
    {
        if(InValue)
        {
            if(!::SetFocus(_WinAPIHandle))
            {
                DWORD error = GetLastError();
                TCHAR errorMsg[MAX_PATH];
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                              NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                              errorMsg, sizeof(errorMsg), NULL);
                ABYTEK_ENGINE_WINDOW_ASSERT(false) << "Cannot set focus for window" << errorMsg;
                return;
            }
        }
        else
        {
            if(::GetFocus() == _WinAPIHandle)
            {
                if(!::SetFocus(NULL))
                {
                    DWORD error = GetLastError();
                    TCHAR errorMsg[MAX_PATH];
                    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                  NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                  errorMsg, sizeof(errorMsg), NULL);
                    ABYTEK_ENGINE_WINDOW_ASSERT(false) << "Cannot set focus for window" << errorMsg;
                    return;
                }
            }
        }
    }
    void F_WindowsWindow::WindowsMove(const F_Vector2_I32& InValue)
    {
        auto Result = SetWindowPos(
            _WinAPIHandle,
            0,
            InValue.X,
            InValue.Y,
            0,
            0,
            SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW
        );
        ABYTEK_ENGINE_WINDOW_ASSERT(Result) << "Cannot move";
    }
    void F_WindowsWindow::WindowsResize(const F_Vector2_U32& InValue)
    {
        auto Result = SetWindowPos(
            _WinAPIHandle,
            0,
            0,
            0,
            InValue.X,
            InValue.Y,
            SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW
        );
        ABYTEK_ENGINE_WINDOW_ASSERT(Result) << "Cannot resize";
    }

    void F_WindowsWindow::FinalizeNextState()
    {
        A_Window::FinalizeNextState();
        
        auto& NextState = InjectNextState();
        
        {
            RECT Rect;
            ::GetClientRect(
                _WinAPIHandle,
                &Rect
            );

            POINT TopLeftCorner {
                Rect.left,
                Rect.top
            };

            ClientToScreen(
                _WinAPIHandle,
                &TopLeftCorner
            );

            NextState.ClientOffset = F_Vector2_I32(
                (I32)(TopLeftCorner.x),
                (I32)(TopLeftCorner.y)
            );
            NextState.ClientSize = F_Vector2_U32(
                Rect.right - Rect.left,
                Rect.bottom - Rect.top
            );
        }
    }
}
#endif