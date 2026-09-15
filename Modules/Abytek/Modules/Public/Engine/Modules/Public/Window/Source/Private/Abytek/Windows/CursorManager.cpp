#include "Abytek/Windows/CursorManager.hpp"
#include "Abytek/Windows/Cursor.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/LLEvents/Windows/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsCursorManager::Build()
    {
        A_CursorManager::Build();

        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();

        auto& WinAPIProcessMsgEvent = LLEvents->GetOnWinAPIProcessMsgEvent();
        _OnProcessMsgEvent = WinAPIProcessMsgEvent.AddListener(
            [this](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, TF_Optional<HRESULT>& ResultOpt)
            {
                _ProcessMessage(hwnd, uMsg, wParam, lParam, ResultOpt);
            }
        );
    }
    void F_WindowsCursorManager::Release()
    {
        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();

        auto& WinAPIProcessMsgEvent = LLEvents->GetOnWinAPIProcessMsgEvent();
        WinAPIProcessMsgEvent.RemoveListener(_OnProcessMsgEvent);
        
        A_CursorManager::Release();
    }

    void F_WindowsCursorManager::OnUpdate()
    {
        A_CursorManager::OnUpdate();
    }
    void F_WindowsCursorManager::OnPostUpdate()
    {
        // Send commands to make win msg
        for (const auto& Cursor : GetCursors())
        {
            auto CastedCursor = Cursor.FastCast<F_WindowsCursor>();
            
            const auto& CurrentState = Cursor->GetCurrentState();
            const auto& NextState = Cursor->GetNextState();
            
            if (NextState.WillBeDestroyed)
            {
                continue;
            }
            if (CurrentState.Position != NextState.Position)
            {
                CastedCursor->WindowsMove(NextState.Position);
            }
            if (CurrentState.IsVisible != NextState.IsVisible)
            {
                CastedCursor->WindowsSetVisibility(NextState.IsVisible);
            }
            if (NextState.LockedToWindow)
            {
                CastedCursor->WindowsLockToWindow(NextState.LockedToWindow);
            }
            if (!NextState.LockedToWindow && (CurrentState.LockedToWindow != NextState.LockedToWindow))
            {
                CastedCursor->WindowsUnlockFromWindow();
            }
        }
        
        A_CursorManager::OnPostUpdate();
    }

    void F_WindowsCursorManager::_ProcessMessage(HWND WinAPIHandle, UINT UMsg, WPARAM WParam, LPARAM LParam, TF_Optional<HRESULT>& ResultOpt)
    {
        auto MainCursor = GetMainCursor();
        auto& NextState = MainCursor->InjectNextState();
        
        switch (UMsg)
        {
        case WM_INPUT:
            {
                RAWINPUT RawInput;
                U32 RawInputSize = sizeof(RAWINPUT);
                if(
                    GetRawInputData(
                        reinterpret_cast<HRAWINPUT>(LParam),
                        RID_INPUT,
                        &RawInput,
                        &RawInputSize,
                        sizeof(RAWINPUTHEADER)
                    ) == U32(-1)
                )
                {
                    ABYTEK_ENGINE_IHI_ASSERT(false) << "can't get raw input data";
                }
                if (RawInput.header.dwType == RIM_TYPEMOUSE)
                {
                    if(!(RawInput.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE))
                    {
                        POINT p;
                        GetCursorPos(&p);
                        auto& CursorPosition = NextState.Position;
                        CursorPosition.X = p.x;
                        CursorPosition.Y = p.y;
                    }
                }
                break;
            }
        }
    }
}
#endif