#include "Abytek/PathManager.hpp"
#include "Abytek/Windows/PathManager.hpp"
#include "Abytek/Android/PathManager.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_PathManager);

    A_PathManager::A_PathManager()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    A_PathManager::~A_PathManager()
    {
    }

    TU<A_PathManager> A_PathManager::Create()
    {
#ifdef ABYTEK_PLATFORM_WINDOWS
        return TU<F_WindowsPathManager>()();
#elif defined(ABYTEK_PLATFORM_ANDROID)
        return TU<F_AndroidPathManager>()();
#else
#error "Unknown platform"
        return {};
#endif
    }
}
