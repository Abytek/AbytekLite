#include "Abytek/DirectX12/RHIResourceSyncPass.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIResourceSyncPass::Build(const F_RHIResourceSyncPassBuildParams& BuildParams)
    {
        A_RHIResourceSyncPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
    }   
    void F_DirectX12RHIResourceSyncPass::Release()
    {
        A_DirectX12RHIPassExtension::Release();
        A_RHIResourceSyncPass::Release();
    }

    void F_DirectX12RHIResourceSyncPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        auto Resource = GetResource();
        auto Access = GetAccess();
        const auto& SubresourceSet = Resource.StaticCast<F_DirectX12RHIResource>()->GetSubresourceSet();
        for (const auto& Subresource : SubresourceSet)
        {
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(Resource.Weak(), Subresource.Index),
                    Access
                )  
            );
        }
    }

    E_DirectX12RHIPassBatchType F_DirectX12RHIResourceSyncPass::GetPassBatchType()
    {
        if (GetAccess().CPU != E_RHIResourceCPUAccess::NONE)
        {
            return E_DirectX12RHIPassBatchType::CPU_SYNC;
        }
        return E_DirectX12RHIPassBatchType::GPU;
    }
}
#endif