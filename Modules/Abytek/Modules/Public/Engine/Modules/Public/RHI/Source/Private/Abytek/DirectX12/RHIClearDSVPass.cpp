#include "Abytek/DirectX12/RHIClearDSVPass.hpp"
#include "Abytek/DirectX12/RHIResourceView.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIClearDSVPass::Build(const F_RHIClearDSVPassBuildParams& BuildParams)
    {
        A_RHIClearDSVPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
    }   
    void F_DirectX12RHIClearDSVPass::Release()
    {
        A_DirectX12RHIPassExtension::Release();
        A_RHIClearDSVPass::Release();
    }

    void F_DirectX12RHIClearDSVPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        GetDSV().FastCast<F_DirectX12RHIResourceView>()->AppendSubresourceBindingSet(SubresourceBindingSet);
    }
}
#endif