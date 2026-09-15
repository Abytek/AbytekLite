#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/RHIPipelineStateTemplate.hpp"
#include "Abytek/RHITemplateRuntimeDatabase.hpp"
#include "Abytek/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntimeProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIPipelineStateTemplateRuntime)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIPipelineStateTemplateRuntime>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT();
    void A_RHIPipelineStateTemplateRuntime::Build(const F_RHIPipelineStateTemplateRuntimeBuildParams& BuildParams)
    {
        A_RHITemplateRuntime::Build(BuildParams);
    }
    void A_RHIPipelineStateTemplateRuntime::Release()
    {
        _BindGroupTemplateRuntimes = {};
        A_RHITemplateRuntime::Release();
    }

    void A_RHIPipelineStateTemplateRuntime::FinalizeActivation()
    {
        A_RHITemplateRuntime::FinalizeActivation();
        
        auto RuntimeDatabase = GetDatabase();
        auto Template = GetTemplate().FastCast<A_RHIPipelineStateTemplate>();
        
        for (const auto& BindGroupTemplate : Template->GetBindGroupTemplates())
        {
            _BindGroupTemplateRuntimes.push_back(
                RuntimeDatabase->GetOrActivateRuntime(BindGroupTemplate)
                .FastCast<A_RHIBindGroupTemplateRuntime>()
            );
        }
        
        GetProxy().FastCast<A_RHIPipelineStateTemplateRuntimeProxy>()->LateBuildPipelineStateTemplateRuntime();
    }

    TS<A_RHIContextChildProxy> A_RHIPipelineStateTemplateRuntime::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIPipelineStateTemplateRuntimeProxy>(ABYTEK_WTHIS());
    }
}
