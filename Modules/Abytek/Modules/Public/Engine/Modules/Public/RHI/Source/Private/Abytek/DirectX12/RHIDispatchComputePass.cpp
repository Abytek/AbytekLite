#include "Abytek/DirectX12/RHIDispatchComputePass.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIDispatchComputePass::Build(const F_RHIDispatchComputePassBuildParams& BuildParams)
    {
        A_RHIDispatchComputePass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        A_DirectX12RHIHasGeneralPipeline::Build(BuildParams);
    }   
    void F_DirectX12RHIDispatchComputePass::Release()
    {
        A_DirectX12RHIHasGeneralPipeline::Release();
        A_DirectX12RHIPassExtension::Release();
        A_RHIDispatchComputePass::Release();
    }

    void F_DirectX12RHIDispatchComputePass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        A_DirectX12RHIHasGeneralPipeline::AppendSubresourceBindings(SubresourceBindingSet);
    }
}
#endif