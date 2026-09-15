#include "Abytek/LLEvents/LLEvents.hpp"
#ifdef ABYTEK_PLATFORM_WINDOWS
#include "Abytek/LLEvents/Windows/LLEvents.hpp"
#elif defined(ABYTEK_PLATFORM_ANDROID)
#include "Abytek/LLEvents/Android/LLEvents.hpp"
#endif


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_LLEvents);
    
    A_LLEvents::A_LLEvents()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    A_LLEvents::~A_LLEvents()
    {
    }

    B8 A_LLEvents::_NewFrame()
    {
        return GetLiveCounter() != 0;
    }

    B8 A_LLEvents::Tick()
    {
        if (!_NewFrame())
        {
            return false;
        }
        ABYTEK_PROFILER_FRAME();
        ABYTEK_PROFILER_EVENT();
        
        OnTick.Broadcast();
        FinalizeFrame();

        H_TaskUtilities::Switch();
        return true;
    }

    void A_LLEvents::Start()
    {
        while(Tick());
    }
    void A_LLEvents::FinalizeFrame()
    {
    }

    TU<A_LLEvents> A_LLEvents::Create()
    {
#ifdef ABYTEK_PLATFORM_WINDOWS
        return TU<F_WindowsLLEvents>()();
#elif defined(ABYTEK_PLATFORM_ANDROID)
        return TU<F_AndroidLLEvents>()();
#else 
#error "Unknown platform"
#endif
    }
}
