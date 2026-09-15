#include "Abytek/IHISubsystemProxy.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/Windows/IHISubsystemProxy.hpp"
#include "Abytek/Android/IHISubsystemProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_IHISubsystemProxy)
        for (auto API : F_IHISubsystem::GetInstance()->GetAPIs())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
            if (API == E_IHIAPI::WINDOWS)
            {
                if (auto SubsystemProxy = F_WindowsIHISubsystemProxy::TryCreate())
                {
                    return SubsystemProxy;
                }
            }
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
            if (API == E_IHIAPI::ANDROID_)
            {
                if (auto SubsystemProxy = F_AndroidIHISubsystemProxy::TryCreate())
                {
                    return SubsystemProxy;
                }
            }
#endif
        }
    ABYTEK_RA_END_OBJECT()
    void A_IHISubsystemProxy::Build()
    {
        A_RAObject::BuildMinimal();
    }
    void A_IHISubsystemProxy::Release()
    {
        A_RAObject::Release();
    }
}
