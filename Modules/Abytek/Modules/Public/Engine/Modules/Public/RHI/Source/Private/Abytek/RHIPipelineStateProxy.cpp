#include "Abytek/RHIPipelineStateProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHIPipelineState.hpp"
#include "Abytek/DirectX12/RHIPipelineStateProxy.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntimeProxy.hpp"


namespace Abytek
{
    /*ABYTEK_RA_BEGIN_OBJECT(A_RHIPipelineStateProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIPipelineStateProxy>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIPipelineStateProxy::LateBuildPipelineState()
    {
        auto PipelineState = GetPipelineState();
        _TemplateRuntimeProxy = PipelineState->GetTemplateRuntime()->GetProxy().FastCast<A_RHIPipelineStateTemplateRuntimeProxy>();
    }
    void A_RHIPipelineStateProxy::Release()
    {
        _TemplateRuntimeProxy = {};
        A_RHIContextChildProxy::Release();
    }*/
}
