#include "Abytek/Windows/IHISubsystemProxy.hpp"
#include "Abytek/IHISubsystem.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsIHISubsystemProxy::Build()
    {
        A_IHISubsystemProxy::Build();
        
        RAWINPUTDEVICE RawInputDevice;
        RawInputDevice.usUsagePage = 0x1 /* HID_USAGE_PAGE_GENERIC */;
        RawInputDevice.usUsage = 0x2 /* HID_USAGE_GENERIC_MOUSE */;
        RawInputDevice.dwFlags = 0;
        RawInputDevice.hwndTarget = 0;
        B8 Result = RegisterRawInputDevices(&RawInputDevice, 1, sizeof(RAWINPUTDEVICE));
        ABYTEK_BASE_PLATFORM_ASSERT(Result) << "can't register raw input device for mouse";
    }
    void F_WindowsIHISubsystemProxy::Release()
    {
        A_IHISubsystemProxy::Release();
    }

    E_IHIAPI F_WindowsIHISubsystemProxy::GetAPI()
    {
        return E_IHIAPI::WINDOWS;
    }

    TU<A_IHISubsystemProxy> F_WindowsIHISubsystemProxy::TryCreate()
    {
        return TU<F_WindowsIHISubsystemProxy>()();
    }
}
#endif