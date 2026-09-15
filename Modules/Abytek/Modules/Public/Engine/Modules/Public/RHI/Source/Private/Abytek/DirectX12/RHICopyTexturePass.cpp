#include "Abytek/DirectX12/RHICopyTexturePass.hpp"
#include "Abytek/DirectX12/RHIResourceView.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHICopyTexturePass::Build(const F_RHICopyTexturePassBuildParams& BuildParams)
    {
        A_RHICopyTexturePass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
    }   
    void F_DirectX12RHICopyTexturePass::Release()
    {
        A_DirectX12RHIPassExtension::Release();
        A_RHICopyTexturePass::Release();
    }

    void F_DirectX12RHICopyTexturePass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        const auto& SrcTexture = GetSrcTexture();
        const auto& DestTexture = GetDestTexture();
        
        const auto& SrcTextureAspect = SrcTexture->GetTextureAspect();
        const auto& DestTextureAspect = DestTexture->GetTextureAspect();
        
        const auto& SrcElementSet = GetSrcElementSet();
        const auto& DstElementSet = GetDstElementSet();
        
        U32 NumSrcElements = SrcElementSet.size();
        U32 NumDstElements = DstElementSet.size();
        
        ABYTEK_ENGINE_RHI_ASSERT(NumSrcElements == NumDstElements)
            << "Mismatched elements ("
            << "src: " << NumSrcElements
            << ", dest: " << NumDstElements
            << ")";
        
        for (U32 Idx = 0; Idx < NumSrcElements; ++Idx)
        {
            const auto& SrcElement = SrcElementSet[Idx];
            const auto& DstElement = DstElementSet[Idx];
            
            ABYTEK_ENGINE_RHI_ASSERT(
                (SrcElement.MipLevel < SrcTextureAspect.MipLevelCount)    
                && (SrcElement.ArraySlice < SrcTextureAspect.ArraySize)    
            );
            ABYTEK_ENGINE_RHI_ASSERT(
                (DstElement.MipLevel < DestTextureAspect.MipLevelCount)    
                && (DstElement.ArraySlice < DestTextureAspect.ArraySize)    
            );
            
            UINT SrcSubresource = D3D12CalcSubresource(
                SrcElement.MipLevel,
                SrcElement.ArraySlice,
                0,
                SrcTextureAspect.MipLevelCount,
                SrcTextureAspect.ArraySize
            );

            UINT DestSubresource = D3D12CalcSubresource(
                DstElement.MipLevel,
                DstElement.ArraySlice,
                0,
                DestTextureAspect.MipLevelCount,
                DestTextureAspect.ArraySize
            );
        
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        SrcTexture.Weak(),
                        SrcSubresource
                    ),
                    F_RHIResourceAccess::MakeCopySrc()
                )
            );
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        DestTexture.Weak(),
                        DestSubresource
                    ),
                    F_RHIResourceAccess::MakeCopyDest()
                )
            );
        }
    }
}
#endif