#include "Abytek/DirectX12/RHICopyBufferPass.hpp"
#include "Abytek/DirectX12/RHIResourceView.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHICopyBufferPass::Build(const F_RHICopyBufferPassBuildParams& BuildParams)
    {
        A_RHICopyBufferPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
    }   
    void F_DirectX12RHICopyBufferPass::Release()
    {
        A_DirectX12RHIPassExtension::Release();
        A_RHICopyBufferPass::Release();
    }

    void F_DirectX12RHICopyBufferPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        SubresourceBindingSet.push_back(
            F_DirectX12RHISubresourceBinding::MakeCore(
                F_DirectX12RHISubresourceReference::Make(
                    GetSrcBuffer().Weak(),
                    0
                ),
                F_RHIResourceAccess::MakeCopySrc()
            )
        );
        SubresourceBindingSet.push_back(
            F_DirectX12RHISubresourceBinding::MakeCore(
                F_DirectX12RHISubresourceReference::Make(
                    GetDstBuffer().Weak(),
                    0
                ),
                F_RHIResourceAccess::MakeCopyDest()
            )
        );
    }
}
#endif