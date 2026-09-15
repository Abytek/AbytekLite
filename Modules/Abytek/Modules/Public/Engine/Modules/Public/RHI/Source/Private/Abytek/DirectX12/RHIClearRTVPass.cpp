#include "Abytek/DirectX12/RHIClearRTVPass.hpp"
#include "Abytek/DirectX12/RHIResourceView.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIClearRTVPass::Build(const F_RHIClearRTVPassBuildParams& BuildParams)
    {
        A_RHIClearRTVPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
    }   
    void F_DirectX12RHIClearRTVPass::Release()
    {
        A_DirectX12RHIPassExtension::Release();
        A_RHIClearRTVPass::Release();
    }

    void F_DirectX12RHIClearRTVPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        GetRTV().FastCast<F_DirectX12RHIResourceView>()->AppendSubresourceBindingSet(SubresourceBindingSet);
    }
}
#endif