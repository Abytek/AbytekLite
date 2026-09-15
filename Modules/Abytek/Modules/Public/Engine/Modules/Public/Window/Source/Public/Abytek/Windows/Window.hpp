#pragma once

#include "Abytek/Window.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    struct ABYTEK_ENGINE_WINDOW_API H_WindowsWindow
    {
        static int ParseShowCommand(B8 IsVisible, E_WindowSizeMode SizeMode);
    };
    
    class ABYTEK_ENGINE_WINDOW_API F_WindowsWindow : public A_Window
    {
    private:
        HWND _WinAPIHandle = 0;

    public:
        ABYTEK_FORCE_INLINE auto GetWinAPIHandle() const noexcept
        {
            return _WinAPIHandle; 
        }
        ABYTEK_FORCE_INLINE auto& InjectWinAPIHandle() noexcept
        {
            return _WinAPIHandle;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_WindowsWindow);
        virtual void Build(const F_WindowBuildParams& Params) override;
        virtual void Release() override;

    private:
        void _CreateWindow();
        void _DestroyWindow();
        
    public:
        void WindowsDestroyWindow();
        void WindowsShowWindow(B8 InIsVisible, E_WindowSizeMode InSizeMode);
        void WindowsSetFocus(B8 InValue);
        void WindowsMove(const F_Vector2_I32& InValue);
        void WindowsResize(const F_Vector2_U32& InValue);
        
    public:
        virtual void FinalizeNextState() override;
    };
}
#endif