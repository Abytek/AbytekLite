#include "Abytek/DirectX12/RHICopyTexturePassProxy.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHICopyTexturePassProxy::Build(const TW_Valid<A_RHICopyTexturePass>& Pass)
    {
        A_RHICopyTexturePassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
    }   
    void F_DirectX12RHICopyTexturePassProxy::Release()
    {
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHICopyTexturePassProxy::Release();
    }

    void F_DirectX12RHICopyTexturePassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        const auto& SrcTextureProxy = GetSrcTextureProxy();
        const auto& DestTextureProxy = GetDestTextureProxy();
        
        const auto& SrcElementSet = GetSrcElementSet();
        const auto& DstElementSet = GetDstElementSet();
        
        U32 NumSrcElements = SrcElementSet.size();
        U32 NumDstElements = DstElementSet.size();
        
        ABYTEK_ENGINE_RHI_ASSERT(NumSrcElements == NumDstElements)
            << "Mismatched elements ("
            << "src: " << NumSrcElements
            << ", dest: " << NumDstElements
            << ")";
        
        auto SrcD3D12Texture = SrcTextureProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        auto DestD3D12Texture = DestTextureProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        
        const auto& SrcTextureAspect = SrcTextureProxy->GetTextureAspect();
        const auto& DestTextureAspect = DestTextureProxy->GetTextureAspect();
        
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
            
            D3D12_TEXTURE_COPY_LOCATION SrcLocation = {};
            SrcLocation.pResource = SrcD3D12Texture.Get();
            SrcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            SrcLocation.SubresourceIndex = SrcSubresource;

            D3D12_TEXTURE_COPY_LOCATION DestLocation = {};
            DestLocation.pResource = DestD3D12Texture.Get();
            DestLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            DestLocation.SubresourceIndex = DestSubresource;
            
            ExecuteParams.D3D12CommandList->CopyTextureRegion(
                &DestLocation, 
                0, 0, 0,
                &SrcLocation, 
                nullptr 
            );
        }
    }
}
#endif