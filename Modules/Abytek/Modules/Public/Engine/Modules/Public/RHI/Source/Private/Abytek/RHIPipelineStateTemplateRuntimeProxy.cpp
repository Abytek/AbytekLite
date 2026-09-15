#include "Abytek/RHIPipelineStateTemplateRuntimeProxy.hpp"
#include "Abytek/RHIBindGroupTemplateRuntimeProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/DirectX12/RHIPipelineStateTemplateRuntimeProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIPipelineStateTemplateRuntimeProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIPipelineStateTemplateRuntimeProxy>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()

    void A_RHIPipelineStateTemplateRuntimeProxy::LateBuildPipelineStateTemplateRuntime()
    {
        auto PipelineStateTemplateRuntime = GetPipelineStateTemplateRuntime();
        for (const auto& BindGroupTemplateRuntime : PipelineStateTemplateRuntime->GetBindGroupTemplateRuntimes())
        {
            _BindGroupTemplateRuntimeProxies.push_back(
                BindGroupTemplateRuntime->GetProxy().FastCast<A_RHIBindGroupTemplateRuntimeProxy>()
            );
        }
    }
    void A_RHIPipelineStateTemplateRuntimeProxy::Release()
    {
        _BindGroupTemplateRuntimeProxies = {};
        A_RHITemplateRuntimeProxy::Release();
    }
}
