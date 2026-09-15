#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"


namespace Abytek
{
    class A_Window;
    
    enum class E_WindowSizeMode
    {
        NONE,
        NORMAL,
        MINIMIZE,
        MAXIMIZE,
        
        DEFAULT = NORMAL
    };
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_WindowSizeMode)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_WindowSizeMode"));
        
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(NORMAL);
        ABYTEK_REFLECT_ENUM_VALUE(MINIMIZE);
        ABYTEK_REFLECT_ENUM_VALUE(MAXIMIZE);
        
        ABYTEK_REFLECT_ENUM_VALUE(DEFAULT);
    }
    
    struct F_WindowState
    {
        B8 IsRequestedToDestroy = false;
        B8 IsAcceptedToDestroy = false;
        F_Text Title;
        F_Vector2_I32 Offset = F_Vector2_I32::Zero();
        F_Vector2_U32 Size = F_Vector2_U32::Zero();
        F_Vector2_I32 ClientOffset = F_Vector2_I32::Zero();
        F_Vector2_U32 ClientSize = F_Vector2_U32::Zero();
        E_WindowSizeMode SizeMode = E_WindowSizeMode::NONE;
        B8 Focus = false;
        B8 IsVisible = false;
        F_Vector2_F32 DPI = F_Vector2_F32::One();
        
        ABYTEK_FORCE_INLINE auto GetScaledOffset() const noexcept
        {
            return F_Vector2_I32(
                F_Vector2_F32(Offset)
                / DPI
            );
        }
        ABYTEK_FORCE_INLINE auto GetScaledSize() const noexcept
        {
            return F_Vector2_U32(
                F_Vector2_F32(Size)
                / DPI
            );
        }
        ABYTEK_FORCE_INLINE auto GetScaledClientOffset() const noexcept
        {
            return F_Vector2_I32(
                F_Vector2_F32(ClientOffset)
                / DPI
            );
        }
        ABYTEK_FORCE_INLINE auto GetScaledClientSize() const noexcept
        {
            return F_Vector2_U32(
                F_Vector2_F32(ClientSize)
                / DPI
            );
        }
    };
    
    namespace WindowEvents
    {
        using F_OnDestroy = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState)>;
        using F_OnRequestDestroy = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState, B8& OutApprove)>;

        using F_OnTitleChanged = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState)>;
        using F_OnMove = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState)>;
        using F_OnResize = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState)>;
        using F_OnSizeModeChanged = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState)>;
        using F_OnLostFocus = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState)>;
        using F_OnFocus = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState)>;
        using F_OnHidden = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState)>;
        using F_OnVisible = TF_Event<void(const F_WindowState& CurrentState, const F_WindowState& NewState)>;
    }
    struct F_WindowEvents
    {
        WindowEvents::F_OnDestroy OnDestroy;
        WindowEvents::F_OnRequestDestroy OnRequestDestroy;
        
        WindowEvents::F_OnTitleChanged OnTitleChanged;
        WindowEvents::F_OnMove OnMove;
        WindowEvents::F_OnResize OnResize;
        WindowEvents::F_OnSizeModeChanged OnSizeModeChanged;
        WindowEvents::F_OnLostFocus OnLostFocus;
        WindowEvents::F_OnFocus OnFocus;
        WindowEvents::F_OnHidden OnHidden;
        WindowEvents::F_OnVisible OnVisible;
    };
}