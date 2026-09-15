#include "Abytek/Windows/WindowSubsystemProxy.hpp"
#include "Abytek/Windows/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/LLEvents/Windows/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsWindowSubsystemProxy::Build()
    {
        A_WindowSubsystemProxy::Build();
    }
    void F_WindowsWindowSubsystemProxy::Release()
    {
        A_WindowSubsystemProxy::Release();
    }

    E_WindowAPI F_WindowsWindowSubsystemProxy::GetAPI()
    {
        return E_WindowAPI::WINDOWS;
    }
}
#endif