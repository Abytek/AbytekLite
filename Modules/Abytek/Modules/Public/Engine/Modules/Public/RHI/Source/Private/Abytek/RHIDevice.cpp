#include "Abytek/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHIDeviceManager.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIDevice)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIDevice>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIDevice::Build()
    {
        A_RAObject::BuildMinimal();
    }
    void A_RHIDevice::Release()
    {
        A_RAObject::Release();

        ABYTEK_ENGINE_RHI_ASSERT(!IsActivated());
        _IsActivated = false;
        _Info = {};
    }

    void A_RHIDevice::Activate()
    {
        ABYTEK_ENGINE_RHI_ASSERT(!_IsActivated) << "RHI device was already activated";
        _IsActivated = true;
    }
    void A_RHIDevice::Deactivate()
    {
        ABYTEK_ENGINE_RHI_ASSERT(_IsActivated) << "RHI device was not activated";
        _IsActivated = false;
    }
}
