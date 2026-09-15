#pragma once

#include "Abytek/LLEvents/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    using F_WinAPIOnProcessMsgEvent = TF_Event<
        void(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, TF_Optional<HRESULT>& ResultOpt)
    >;    
    using F_WinAPIOnPostProcessMsgEvent = TF_Event<void(const MSG& Msg)>;
    
    class ABYTEK_ENGINE_CORE_API F_WindowsLLEvents : public A_LLEvents
    {
    private:
        static const wchar_t _WindowClassNameCStr[];

    public:
        static ABYTEK_FORCE_INLINE const wchar_t* GetWindowClassNameCStr() noexcept { return _WindowClassNameCStr; };

    private:
        F_WinAPIOnProcessMsgEvent _OnWinAPIProcessMsgEvent;
        F_WinAPIOnPostProcessMsgEvent _OnWinAPIPostProcessMsgEvent;

    public:
        ABYTEK_FORCE_INLINE auto& GetOnWinAPIProcessMsgEvent() noexcept
        {
            return _OnWinAPIProcessMsgEvent;
        }
        ABYTEK_FORCE_INLINE auto& GetOnWinAPIPostProcessMsgEvent() noexcept
        {
            return _OnWinAPIPostProcessMsgEvent;
        }

    public:
        F_WindowsLLEvents();
        ~F_WindowsLLEvents() override;
        
    private:
        void _WinAPIProcessMessages();

    public:
        virtual void Start() override;
        virtual void FinalizeFrame() override;
    };
}
#endif