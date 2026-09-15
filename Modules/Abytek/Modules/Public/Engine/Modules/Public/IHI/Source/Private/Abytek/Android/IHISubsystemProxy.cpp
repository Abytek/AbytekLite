#include "Abytek/Android/IHISubsystemProxy.hpp"
#include "Abytek/IHISubsystem.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidIHISubsystemProxy::Build()
    {
        A_IHISubsystemProxy::Build();
    }
    void F_AndroidIHISubsystemProxy::Release()
    {
        A_IHISubsystemProxy::Release();
    }

    E_IHIAPI F_AndroidIHISubsystemProxy::GetAPI()
    {
        return E_IHIAPI::ANDROID_;
    }

    TU<A_IHISubsystemProxy> F_AndroidIHISubsystemProxy::TryCreate()
    {
        return TU<F_AndroidIHISubsystemProxy>()();
    }
}
#endif