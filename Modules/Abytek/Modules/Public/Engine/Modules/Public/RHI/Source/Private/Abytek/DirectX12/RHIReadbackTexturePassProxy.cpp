#include "Abytek/DirectX12/RHIReadbackTexturePassProxy.hpp"
#include "Abytek/DirectX12/RHIReadbackTexturePass.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIReadbackTexturePassProxy::Build(const TW_Valid<A_RHIReadbackTexturePass>& Pass)
    {
        A_RHIReadbackTexturePassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        
        auto CastedPass = Pass.FastCast<F_DirectX12RHIReadbackTexturePass>();
        const auto& TransientReadbackBufferRange = CastedPass->GetTransientReadbackBufferRange();
        _ReadbackBufferProxy = TransientReadbackBufferRange.GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        _NumSubresources = CastedPass->GetNumSubresources();
        _SubImages_ReadbackFootprint = CastedPass->GetSubImages_ReadbackFootprint();
    }   
    void F_DirectX12RHIReadbackTexturePassProxy::Release()
    {
        _SubImages_ReadbackFootprint = {};
        _NumSubresources = 0;
        _ReadbackBufferProxy = {};
        
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIReadbackTexturePassProxy::Release();
    }

    void F_DirectX12RHIReadbackTexturePassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        auto TextureD3D12Resource = GetTextureProxy().FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        auto ReadbackD3D12Resource = _ReadbackBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            D3D12_TEXTURE_COPY_LOCATION DstCopyLocation;
            DstCopyLocation.pResource = ReadbackD3D12Resource.Get();
            DstCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            DstCopyLocation.PlacedFootprint = _SubImages_ReadbackFootprint[SubresourceIndex];
            
            D3D12_TEXTURE_COPY_LOCATION SrcCopyLocation;
            SrcCopyLocation.pResource = TextureD3D12Resource.Get();
            SrcCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            SrcCopyLocation.SubresourceIndex = SubresourceIndex;
                
            ExecuteParams.D3D12CommandList->CopyTextureRegion(
                &DstCopyLocation, 
                0, 0, 0,
                &SrcCopyLocation, 
                nullptr 
            );
        }
    }
}
#endif