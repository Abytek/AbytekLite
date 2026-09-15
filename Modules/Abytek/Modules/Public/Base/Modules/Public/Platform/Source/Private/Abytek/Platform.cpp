#include "Abytek/Platform.hpp"
#include "Abytek/FSAdapter_Standard.hpp"
#include "Abytek/PathManager.hpp"
#include "Abytek/Windows/Platform.hpp"
#include "Abytek/Android/Platform.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_Platform);

    A_Platform::A_Platform()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    A_Platform::~A_Platform()
    {
    }

    TU<A_Platform> A_Platform::Create()
    {
#ifdef ABYTEK_PLATFORM_WINDOWS
        return TU<F_WindowsPlatform>()();
#elif defined(ABYTEK_PLATFORM_ANDROID)
        return TU<F_AndroidPlatform>()();
#else
#error "Unknown platform"
        return {};
#endif
    }

    void A_Platform::OnInit()
    {
        _FSAdapter_Standard = A_FSAdapter_Standard::Create();
        _PathManager = A_PathManager::Create();
    }
    void A_Platform::OnPostInit()
    {
    }
    void A_Platform::OnPreRelease()
    {
    }
    void A_Platform::OnRelease()
    {
        _PathManager = {};
        _FSAdapter_Standard = {};
    }

    void A_Platform::Startup()
    {
        OnInit();
        OnPostInit();
    }
    void A_Platform::Shutdown()
    {
        OnPreRelease();
        OnRelease();
    }

    void A_Platform::OnBeginCurrentThread()
    {
    }
    void A_Platform::OnEndCurrentThread()
    {
    }
}
