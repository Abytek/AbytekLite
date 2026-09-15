#include "Abytek/DirectX12/RHIRayTracePass.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIRayTracePass::Build(const F_RHIRayTracePassBuildParams& BuildParams)
    {
        A_RHIRayTracePass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        A_DirectX12RHIHasGeneralPipeline::Build(BuildParams);
    }   
    void F_DirectX12RHIRayTracePass::Release()
    {
        A_DirectX12RHIHasGeneralPipeline::Release();
        A_DirectX12RHIPassExtension::Release();
        A_RHIRayTracePass::Release();
    }

    void F_DirectX12RHIRayTracePass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        A_DirectX12RHIHasGeneralPipeline::AppendSubresourceBindings(SubresourceBindingSet);
    }
}
#endif