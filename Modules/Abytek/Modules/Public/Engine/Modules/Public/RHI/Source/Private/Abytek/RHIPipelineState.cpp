#include "Abytek/RHIPipelineState.hpp"
#include "Abytek/RHIPipelineStateProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIPipelineState.hpp"


namespace Abytek
{
    /*ABYTEK_RA_BEGIN_OBJECT(A_RHIPipelineState)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIPipelineState>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIPipelineState::Build(const F_RHIPipelineStateBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);

        _TemplateRuntime = BuildParams.TemplateRuntime;

        ABYTEK_ENGINE_RHI_ASSERT(_TemplateRuntime) << "RHIPipelineState requires a template runtime";
            
        GetProxy().FastCast<A_RHIPipelineStateProxy>()->LateBuildPipelineState();
    }
    void A_RHIPipelineState::Release()
    {
        _TemplateRuntime = {};
            
        A_RHIContextChild::Release();
    }

    TS<A_RHIContextChildProxy> A_RHIPipelineState::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIPipelineStateProxy>(ABYTEK_WTHIS());
    }*/
}
