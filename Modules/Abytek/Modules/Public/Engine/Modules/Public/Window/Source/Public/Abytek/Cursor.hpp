#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"
#include "Abytek/CursorCommon.hpp"


namespace Abytek
{
    class A_Window;
    
    class ABYTEK_ENGINE_WINDOW_API A_Cursor : public A_RAObject
    {
    public:
        friend class A_CursorManager;
        
    private:
        F_CursorState _CurrentState;
        F_CursorState _NextState;

    public:
        F_CursorEvents Events;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCurrentState() const noexcept
        {
            return _CurrentState;
        }
        ABYTEK_FORCE_INLINE auto& InjectCurrentState() noexcept
        {
            return _CurrentState;
        }
        ABYTEK_FORCE_INLINE const auto& GetNextState() const noexcept
        {
            return _NextState;
        }
        ABYTEK_FORCE_INLINE auto& InjectNextState() noexcept
        {
            return _NextState;
        }

        ABYTEK_FORCE_INLINE auto WillBeDestroyed() const noexcept
        {
            return _CurrentState.WillBeDestroyed;
        }
        ABYTEK_FORCE_INLINE const auto& GetPosition() const noexcept
        {
            return _CurrentState.Position;
        }
        ABYTEK_FORCE_INLINE auto IsVisible() const noexcept
        {
            return _CurrentState.IsVisible;
        }
        ABYTEK_FORCE_INLINE const auto& GetLockedToWindow() const noexcept
        {
            return _CurrentState.LockedToWindow;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_Cursor);
        virtual void Build();
        virtual void Release() override;
        
    public:
        virtual void Move(const F_Vector2_I32& NewPosition);
        virtual void SetVisibility(B8 Value);
        virtual void Capture(const TW<A_Window> Window);
        
    public:
        virtual void FinalizeNextState();
        virtual void UpdateCurrentState();
    };
}
