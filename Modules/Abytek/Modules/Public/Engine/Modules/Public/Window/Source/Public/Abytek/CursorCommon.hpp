#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"


namespace Abytek
{
    class A_Window;
    class A_Cursor;
    
    struct F_CursorState
    {
        B8 WillBeDestroyed = false;
        F_Vector2_I32 Position = F_Vector2_I32::Zero();
        B8 IsVisible = false;
        TW<A_Window> LockedToWindow;
    };
    
    namespace CursorEvents
    {
        using F_OnMove = TF_Event<void(const F_CursorState& CurrentState, const F_CursorState& NewState)>;
        using F_OnHidden = TF_Event<void(const F_CursorState& CurrentState, const F_CursorState& NewState)>;
        using F_OnVisible = TF_Event<void(const F_CursorState& CurrentState, const F_CursorState& NewState)>;
        using F_OnLockToWindow = TF_Event<void(const F_CursorState& CurrentState, const F_CursorState& NewState)>;
        using F_OnUnlockFromWindow = TF_Event<void(const F_CursorState& CurrentState, const F_CursorState& NewState)>;
    }
    struct F_CursorEvents
    {
        CursorEvents::F_OnMove OnMove;
        CursorEvents::F_OnHidden OnHidden;
        CursorEvents::F_OnVisible OnVisible;
        CursorEvents::F_OnLockToWindow OnLockToWindow;
        CursorEvents::F_OnUnlockFromWindow OnUnlockFromWindow;
    };
}