#include "Abytek/DirectX12/RHIClearUAVFloatPass.hpp"
#include "Abytek/DirectX12/RHIResourceView.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIClearUAVFloatPass::Build(const F_RHIClearUAVFloatPassBuildParams& BuildParams)
    {
        A_RHIClearUAVFloatPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
    }   
    void F_DirectX12RHIClearUAVFloatPass::Release()
    {
        A_DirectX12RHIPassExtension::Release();
        A_RHIClearUAVFloatPass::Release();
    }

    void F_DirectX12RHIClearUAVFloatPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        GetUAV().FastCast<F_DirectX12RHIResourceView>()->AppendSubresourceBindingSet(SubresourceBindingSet);
    }
}
#endif