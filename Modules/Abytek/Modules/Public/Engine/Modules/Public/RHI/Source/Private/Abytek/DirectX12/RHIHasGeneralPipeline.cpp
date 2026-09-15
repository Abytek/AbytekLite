#include "Abytek/DirectX12/RHIHasGeneralPipeline.hpp"
#include "Abytek/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/DirectX12/RHIDescriptorManager.hpp"
#include "Abytek/DirectX12/RHIPipelineState.hpp"
#include "Abytek/DirectX12/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/DirectX12/RHIBindGroup.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntime.hpp"
#include "Abytek/DirectX12Shared/RHIBindGroupTemplate.hpp"
#include "Abytek/DirectX12Shared/RHIPipelineStateTemplate.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void A_DirectX12RHIHasGeneralPipeline::Build(const F_RHIHasGeneralPipelineBuildParams& BuildParams)
    {
        _Pass = ABYTEK_WTHIS().DynamicCast<A_RHIPass>();
        _PassExtension = ABYTEK_WTHIS().DynamicCast<A_DirectX12RHIPassExtension>();
        _Base = ABYTEK_WTHIS().DynamicCast<A_RHIHasGeneralPipeline>();
        
        _PassClass = _Pass->GetPassClass();
    }
    void A_DirectX12RHIHasGeneralPipeline::Release()
    {
        _PassClass = E_RHIPassClass::NONE;
        
        _Base = {};
        _PassExtension = {};
        _Pass = {};
    }
        
    void A_DirectX12RHIHasGeneralPipeline::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        for (const auto& BindGroup : _Base->GetBindGroups())
        {
            BindGroup.FastCast<F_DirectX12RHIBindGroup>()->AppendSubresourceBindingSet(SubresourceBindingSet);
        }
    }
}
#endif