#include "Abytek/DirectX12/RHIDeviceManager.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_DirectX12RHIDeviceManager::Build()
    {
        A_RHIDeviceManager::Build();

        _CreateDevices();
    }
    void F_DirectX12RHIDeviceManager::Release()
    {
        A_RHIDeviceManager::Release();
    }

    void F_DirectX12RHIDeviceManager::_CreateDevices()
    {
        auto RHISubsystemProxy = A_RHISubsystemProxy::GetInstance().FastCast<F_DirectX12RHISubsystemProxy>();

        auto DXGIFactory = RHISubsystemProxy->GetDXGIFactory();
        
        TF_Set<U32> DeviceIds;
        
        IDXGIAdapter* DXGIAdapter = 0;
        for (UINT i = 0; DXGIFactory->EnumAdapters(i, &DXGIAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            IDXGIAdapter1* DXGIAdapter1 = 0;
            DXGIAdapter->QueryInterface(IID_PPV_ARGS(&DXGIAdapter1));

            DXGI_ADAPTER_DESC1 DXGIDeviceInfo;
            DXGIAdapter1->GetDesc1(&DXGIDeviceInfo);
            
            DXGIAdapter1->Release();

            if (DXGIDeviceInfo.Flags& DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }    

            if (DeviceIds.find(DXGIDeviceInfo.DeviceId) != DeviceIds.end())
            {
                DXGIAdapter->Release();
                continue;
            }
            DeviceIds.insert(DXGIDeviceInfo.DeviceId);
            
            F_RHIDeviceInfo DeviceInfo;
            DeviceInfo.Id = (
                ToText(DXGIDeviceInfo.AdapterLuid.HighPart)
                + ToText(DXGIDeviceInfo.AdapterLuid.LowPart)
            );
            DeviceInfo.Description = ToText(DXGIDeviceInfo.Description);
            DeviceInfo.VendorId = DXGIDeviceInfo.VendorId;
            DeviceInfo.DeviceId = DXGIDeviceInfo.DeviceId;
            DeviceInfo.SubsystemId = DXGIDeviceInfo.SubSysId;
            DeviceInfo.Revision = DXGIDeviceInfo.Revision;
            DeviceInfo.DedicatedVideoMemory = DXGIDeviceInfo.DedicatedVideoMemory;
            DeviceInfo.DedicatedSystemMemory = DXGIDeviceInfo.DedicatedSystemMemory;
            DeviceInfo.SharedSystemMemory = DXGIDeviceInfo.SharedSystemMemory;
            
            CreateDevice(
                [&](const auto& Device)
                {
                    Device->InjectInfo() = DeviceInfo;

                    auto CastedDevice = Device.template FastCast<F_DirectX12RHIDevice>();
                    CastedDevice->InjectDXGIAdapter() = DXGIAdapter;
                }
            );
        }
    }
}
#endif