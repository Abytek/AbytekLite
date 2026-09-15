#pragma once

#include "Abytek/WindowManager.hpp"
#include "Abytek/LLEvents/Windows/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API F_WindowsWindowManager : public A_WindowManager
    {
    private:
        F_WinAPIOnProcessMsgEvent::F_ListenerHandle _OnProcessMsgEvent;

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_WindowsWindowManager);
        virtual void Build() override;
        virtual void Release() override;
        
    protected:
        virtual void OnUpdate() override;
        virtual void OnPostUpdate() override;
        
    private:
        void _ProcessMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, TF_Optional<HRESULT>& ResultOpt);
    };
}
#endif