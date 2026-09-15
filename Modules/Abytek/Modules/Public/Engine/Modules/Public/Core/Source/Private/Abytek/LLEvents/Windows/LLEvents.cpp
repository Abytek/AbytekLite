#include "Abytek/LLEvents/Windows/LLEvents.hpp"
#include "Abytek/Profiling.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    const wchar_t F_WindowsLLEvents::_WindowClassNameCStr[] = ABYTEK_TEXT("Abytek::WindowClass");

	LRESULT CALLBACK LLEventsWindowProc(HWND WinAPIHandle, UINT UMsg, WPARAM WParam, LPARAM LParam)
	{
		auto LLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();

		TF_Optional<HRESULT> ResultOpt;
		
		auto& OnWinAPIProcessMsgEvent = LLEvents->GetOnWinAPIProcessMsgEvent();
		OnWinAPIProcessMsgEvent.BroadcastCustom(
			[&](auto& Listener)
			{
				Listener(WinAPIHandle, UMsg, WParam, LParam, ResultOpt);
				if (ResultOpt)
				{
					return false;
				}
				return true;
			}
		);
		if (ResultOpt)
		{
			return *ResultOpt;
		}

		if (UMsg == WM_DESTROY)
		{
			return 0;
		}
		return DefWindowProc(WinAPIHandle, UMsg, WParam, LParam);
	}
	
    F_WindowsLLEvents::F_WindowsLLEvents()
    {
		WNDCLASS WC = { };
		WC.lpfnWndProc = LLEventsWindowProc;
		WC.lpszClassName = _WindowClassNameCStr;
		RegisterClass(&WC);
    }
    F_WindowsLLEvents::~F_WindowsLLEvents()
    {
    	UnregisterClass(_WindowClassNameCStr, 0);
    }

    void F_WindowsLLEvents::_WinAPIProcessMessages()
    {
		ABYTEK_PROFILER_EVENT();
    	while(true)
    	{
    		MSG Msg = {};

    		if(!PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
    		{
    			break;
    		}

    		TranslateMessage(&Msg);
    		DispatchMessage(&Msg);

    		_OnWinAPIPostProcessMsgEvent.Broadcast(Msg);
    	}
    }

    void F_WindowsLLEvents::Start()
    {
		A_LLEvents::Start();
    }
	void F_WindowsLLEvents::FinalizeFrame()
	{
		_WinAPIProcessMessages();
	}
}
#endif