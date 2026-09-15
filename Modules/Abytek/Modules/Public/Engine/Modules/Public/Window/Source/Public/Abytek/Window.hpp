#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"
#include "Abytek/WindowCommon.hpp"


namespace Abytek
{
    struct F_WindowBuildParams
    {
        F_Text Title;

        F_Vector2_I32 Offset = F_Vector2_I32::Zero();
        F_Vector2_U32 Size = F_Vector2_U32 { 1200, 700 };

        B8 IsVisible = true;
        B8 Focus = true;

        E_WindowSizeMode SizeMode = E_WindowSizeMode::DEFAULT;
    };
    
    class ABYTEK_ENGINE_WINDOW_API A_Window : public A_RAObject
    {
    public:
        friend class A_WindowManager;
        
    private:
        F_WindowState _CurrentState;
        F_WindowState _NextState;

    public:
        F_WindowEvents Events;
        
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
        
        ABYTEK_FORCE_INLINE B8 IsRequestedToDestroy() const noexcept
        {
            return _CurrentState.IsRequestedToDestroy;
        }
        ABYTEK_FORCE_INLINE B8 IsAcceptedToDestroy() const noexcept
        {
            return _CurrentState.IsAcceptedToDestroy;
        }
        ABYTEK_FORCE_INLINE const auto& GetTitle() const noexcept
        {
            return _CurrentState.Title;
        }
        ABYTEK_FORCE_INLINE const auto& GetOffset() const noexcept
        {
            return _CurrentState.Offset;
        }
        ABYTEK_FORCE_INLINE const auto& GetSize() const noexcept
        {
            return _CurrentState.Size;
        }
        ABYTEK_FORCE_INLINE const auto& GetClientOffset() const noexcept
        {
            return _CurrentState.ClientOffset;
        }
        ABYTEK_FORCE_INLINE const auto& GetClientSize() const noexcept
        {
            return _CurrentState.ClientSize;
        }
        ABYTEK_FORCE_INLINE auto GetScaledOffset() const noexcept
        {
            return _CurrentState.GetScaledOffset();
        }
        ABYTEK_FORCE_INLINE auto GetScaledSize() const noexcept
        {
            return _CurrentState.GetScaledSize();
        }
        ABYTEK_FORCE_INLINE auto GetScaledClientOffset() const noexcept
        {
            return _CurrentState.GetScaledClientOffset();
        }
        ABYTEK_FORCE_INLINE auto GetScaledClientSize() const noexcept
        {
            return _CurrentState.GetScaledClientSize();
        }
        ABYTEK_FORCE_INLINE auto GetSizeMode() const noexcept
        {
            return _CurrentState.SizeMode;
        }
        ABYTEK_FORCE_INLINE auto GetFocus() const noexcept
        {
            return _CurrentState.Focus;
        }
        ABYTEK_FORCE_INLINE auto IsVisible() const noexcept
        {
            return _CurrentState.IsVisible;
        }
        ABYTEK_FORCE_INLINE const auto& GetDPI() const noexcept
        {
            return _CurrentState.DPI;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_Window);
        virtual void Build(const F_WindowBuildParams& Params);
        virtual void Release() override;
        
    public:
        virtual void RequestToDestroy();
        virtual void AcceptToDestroy();
        virtual void SetTitle(const F_Text& Value);
        virtual void Move(const F_Vector2_I32& NewOffset);
        virtual void Resize(const F_Vector2_U32& Value);
        virtual void SetVisibility(B8 Value);
        virtual void SetFocus(B8 Value);
        virtual void SetSizeMode(E_WindowSizeMode Value);
        
    public:
        virtual void FinalizeNextState();
        virtual void UpdateCurrentState();
    };
}
