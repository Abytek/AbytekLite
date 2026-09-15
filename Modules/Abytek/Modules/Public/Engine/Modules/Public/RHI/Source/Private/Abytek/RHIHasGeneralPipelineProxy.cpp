#include "Abytek/RHIHasGeneralPipelineProxy.hpp"
#include "Abytek/RHIHasGeneralPipeline.hpp"
#include "Abytek/RHIPipelineState.hpp"
#include "Abytek/RHIPipelineStateProxy.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntimeProxy.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIBindGroupProxy.hpp"
#include "Abytek/RHIPass.hpp"


namespace Abytek
{
    void A_RHIHasGeneralPipelineProxy::Build(const TW_Valid<A_RHIPass>& Pass)
    {
        auto HasGeneralPipeline = Pass.DynamicCast<A_RHIHasGeneralPipeline>();
        
        _PipelineStateProxy = HasGeneralPipeline->GetPipelineState()->GetProxy().FastCast<A_RHIPipelineStateProxy>();
        
        const auto& BindGroups = HasGeneralPipeline->GetBindGroups();
        _BindGroupProxies.reserve(BindGroups.size());
        for (const auto& BindGroup : BindGroups)
        {
            _BindGroupProxies.push_back(BindGroup->GetProxy().FastCast<A_RHIBindGroupProxy>());
        }
    }
    void A_RHIHasGeneralPipelineProxy::Release()
    {
        _BindGroupProxies = {};
        _PipelineStateProxy = {};
    }
}
