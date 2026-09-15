#include "Abytek/DirectX12/RHIWorkGraphPass.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIWorkGraphPass::Build(const F_RHIWorkGraphPassBuildParams& BuildParams)
    {
        A_RHIWorkGraphPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        A_DirectX12RHIHasGeneralPipeline::Build(BuildParams);
    }   
    void F_DirectX12RHIWorkGraphPass::Release()
    {
        A_DirectX12RHIHasGeneralPipeline::Release();
        A_DirectX12RHIPassExtension::Release();
        A_RHIWorkGraphPass::Release();
    }

    void F_DirectX12RHIWorkGraphPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        A_DirectX12RHIHasGeneralPipeline::AppendSubresourceBindings(SubresourceBindingSet);
    }
}
#endif