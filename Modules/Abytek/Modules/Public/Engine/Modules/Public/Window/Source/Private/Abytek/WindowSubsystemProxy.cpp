#include "Abytek/WindowSubsystemProxy.hpp"
#include "Abytek/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/Windows/WindowSubsystemProxy.hpp"
#include "Abytek/Android/WindowSubsystemProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_WindowSubsystemProxy)
        for (auto API : F_WindowSubsystem::GetInstance()->GetAPIs())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
            if (API == E_WindowAPI::WINDOWS)
            {
                return TU<F_WindowsWindowSubsystemProxy>()();
            }
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
            if (API == E_WindowAPI::ANDROID_)
            {
                return TU<F_AndroidWindowSubsystemProxy>()();
            }
#endif
    }
    ABYTEK_RA_END_OBJECT()
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_WindowSubsystemProxy);
    void A_WindowSubsystemProxy::Build()
    {
        A_RAObject::BuildMinimal();
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    void A_WindowSubsystemProxy::Release()
    {
        A_RAObject::Release();
    }
}
