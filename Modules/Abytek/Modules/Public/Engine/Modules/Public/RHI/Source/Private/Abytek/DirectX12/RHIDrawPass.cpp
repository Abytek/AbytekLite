#include "Abytek/DirectX12/RHIDrawPass.hpp"
#include "Abytek/DirectX12/RHIBindGroup.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIDrawPass::Build(const F_RHIDrawPassBuildParams& BuildParams)
    {
        A_RHIDrawPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        A_DirectX12RHIHasGeneralPipeline::Build(BuildParams);
    }   
    void F_DirectX12RHIDrawPass::Release()
    {
        A_DirectX12RHIHasGeneralPipeline::Release();
        A_DirectX12RHIPassExtension::Release();
        A_RHIDrawPass::Release();
    }

    void F_DirectX12RHIDrawPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        A_DirectX12RHIHasGeneralPipeline::AppendSubresourceBindings(SubresourceBindingSet);
    }
}
#endif