#include "Abytek/Windows/Cursor.hpp"
#include "Abytek/Windows/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/LLEvents/Windows/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsCursor::Build()
    {
        A_Cursor::Build();
        
        auto& NextState = InjectNextState();
        
        {
            CURSORINFO ci;
            ci.cbSize = sizeof(CURSORINFO);
            B8 Status = GetCursorInfo(&ci);
            ABYTEK_ENGINE_WINDOW_ASSERT(Status) << "Cannot get cursor info";
            NextState.IsVisible = (ci.flags & CURSOR_SHOWING) != 0;
        }
    }
    void F_WindowsCursor::Release()
    {
        A_Cursor::Release();
    }

    void F_WindowsCursor::WindowsMove(const F_Vector2_I32& InValue)
    {
        auto Result = SetCursorPos(InValue.X, InValue.Y);
        ABYTEK_ENGINE_WINDOW_ASSERT(Result) << "Cannot move";
    }
    void F_WindowsCursor::WindowsSetVisibility(B8 InValue)
    {
        if(InValue)
        {
            while(ShowCursor(InValue) < 0);
        }
        else
        {
            while(ShowCursor(InValue) >= 0);
        }
    }
    void F_WindowsCursor::WindowsLockToWindow(const TW_Valid<A_Window>& InWindow)
    {
        RECT Rect;
        if(GetWindowRect(InWindow.FastCast<F_WindowsWindow>()->GetWinAPIHandle(), &Rect))
        {
            RECT LastClippedCursor;
            if(GetClipCursor(&LastClippedCursor))
            {
                if(
                    (LastClippedCursor.top != Rect.top)
                    || (LastClippedCursor.bottom != Rect.bottom)
                    || (LastClippedCursor.right != Rect.right)
                    || (LastClippedCursor.left != Rect.left)
                )
                {
                    ClipCursor(&Rect);
                }
            }
            else
            {
                ClipCursor(&Rect);
            }
        }
    }
    void F_WindowsCursor::WindowsUnlockFromWindow()
    {
        ClipCursor(NULL);
    }

    void F_WindowsCursor::FinalizeNextState()
    {
        A_Cursor::FinalizeNextState();
        
        auto& NextState = InjectNextState();
        
        {
            CURSORINFO ci;
            ci.cbSize = sizeof(CURSORINFO);
            if (GetCursorInfo(&ci))
            {
                NextState.IsVisible = (ci.flags & CURSOR_SHOWING) != 0;
            }
        }
    }
}
#endif