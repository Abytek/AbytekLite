#include "Abytek/Android/WindowSubsystemProxy.hpp"
#include "Abytek/Android/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/LLEvents/Android/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidWindowSubsystemProxy::Build()
    {
        A_WindowSubsystemProxy::Build();
    }
    void F_AndroidWindowSubsystemProxy::Release()
    {
        A_WindowSubsystemProxy::Release();
    }

    E_WindowAPI F_AndroidWindowSubsystemProxy::GetAPI()
    {
        return E_WindowAPI::ANDROID_;
    }
}
#endif