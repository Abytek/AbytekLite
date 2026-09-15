#include "Abytek/RHIDeviceManager.hpp"
#include "Abytek/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIDeviceManager.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_RHIDeviceManager);
    
    ABYTEK_RA_BEGIN_OBJECT(A_RHIDeviceManager)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIDeviceManager>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIDeviceManager::Build()
    {
        A_RAObject::BuildMinimal();

        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }   
    void A_RHIDeviceManager::Release()
    {
        A_RAObject::Release();

        while (_Devices.GetSize() > 0)
        {
            DestroyDevice(_Devices.GetBegin()->Weak());
        }
    }

    TW_Valid<A_RHIDevice> A_RHIDeviceManager::CreateDevice(
        TF_Function<void(const TW_Valid<A_RHIDevice>& Device)>&& InitFunction
    )
    {
        auto OwnedSingletonDevice = RACreateAndBuild<A_RHIDevice>();
        auto Result = OwnedSingletonDevice.Weak();
        if (InitFunction)
        {
            InitFunction(Result);
        }
            
        if (GetDevices().GetSize() == 0)
        {
            _MainDevice = Result;
        }
        InjectDevices().Add(boost::move(OwnedSingletonDevice));

        B8 Approve = true;
        _OnRequestConnectEvent.BroadcastCustom(
            [&Result, &Approve](auto& Function) -> B8
            {
                Function(Result, Approve);
                return Approve;
            }
        );

        if (Approve)
        {
            Result->Activate();
            Result->_FeatureSet = Result->GenerateFeatureSet();
            
            _OnConnectedEvent.Broadcast(Result);
        }
        return Result;
    }
    void A_RHIDeviceManager::DestroyDevice(const TW_Valid<A_RHIDevice>& Device)
    {
        if (Device->IsActivated())
        {
            _OnDisconnectedEvent.Broadcast(Device);
            Device->Deactivate();
        }
        _Devices.Remove(
            _Devices.Find(Device)
        );
    }
}
