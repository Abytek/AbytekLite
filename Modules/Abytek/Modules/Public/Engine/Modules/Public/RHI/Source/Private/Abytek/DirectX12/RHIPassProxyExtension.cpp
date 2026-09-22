#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"

#include "Abytek/DirectX12/RHIPassExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    A_DirectX12RHIPassProxyExtension::A_DirectX12RHIPassProxyExtension()
    {
    }
    A_DirectX12RHIPassProxyExtension::~A_DirectX12RHIPassProxyExtension()
    {
    }
    
    void A_DirectX12RHIPassProxyExtension::Build(const TW_Valid<A_RHIPass>& Pass)
    {
        _PassProxy = ABYTEK_WTHIS().DynamicCast<A_RHIPassProxy>();
        auto PassExtension = Pass.DynamicCast<A_DirectX12RHIPassExtension>();
        if (PassExtension->GetPassBatchType() == E_DirectX12RHIPassBatchType::GPU)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            _ResourceBarrierProxies_Before = ProcessData_PassExtension->D3D12ResourceBarrierProxies_Before;
            _ResourceBarrierProxies_After = ProcessData_PassExtension->D3D12ResourceBarrierProxies_After;
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
            _CaptureEventStatesToBegin = ProcessData_PassExtension->CaptureEventStatesToBegin;
            _CaptureEventStatesToEnd = ProcessData_PassExtension->CaptureEventStatesToEnd;
#endif
        }
    }
    void A_DirectX12RHIPassProxyExtension::Release()
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        _CaptureEventStatesToEnd = {};
        _CaptureEventStatesToBegin = {};
#endif
        _ResourceBarrierProxies_After = {};
        _ResourceBarrierProxies_Before = {};
        _PassProxy = {};
    }

    void A_DirectX12RHIPassProxyExtension::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
    }
}
#endif