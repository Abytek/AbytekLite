#include "Abytek/DirectX12/RHIUploadTexturePassProxy.hpp"
#include "Abytek/DirectX12/RHIUploadTexturePass.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHICopyUploadTexturePassProxy);
    void F_DirectX12RHICopyUploadTexturePassProxy::Build(const TW_Valid<F_DirectX12RHICopyUploadTexturePass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        
        auto CastedPass = Pass.FastCast<F_DirectX12RHICopyUploadTexturePass>();
        auto CastedUploadPass = CastedPass->GetUploadPass();
        const auto& TransientUploadBufferRange = CastedUploadPass->GetTransientUploadBufferRange();
        _TextureProxy = CastedUploadPass->GetTexture()->GetProxy().FastCast<A_RHIResourceProxy>();
        _UploadBufferProxy = TransientUploadBufferRange.GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        _NumSubresources = CastedUploadPass->GetNumSubresources();
        _SubImages_UploadFootprint = CastedUploadPass->GetSubImages_UploadFootprint();
    }   
    void F_DirectX12RHICopyUploadTexturePassProxy::Release()
    {
        _SubImages_UploadFootprint = {};
        _NumSubresources = 0;
        _UploadBufferProxy = {};
        _TextureProxy = {};
        
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIPassProxy::Release();
    }

    void F_DirectX12RHICopyUploadTexturePassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        auto TextureD3D12Resource = _TextureProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        auto UploadD3D12Resource = _UploadBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            D3D12_TEXTURE_COPY_LOCATION DstCopyLocation;
            DstCopyLocation.pResource = TextureD3D12Resource.Get();
            DstCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            DstCopyLocation.SubresourceIndex = SubresourceIndex;
                
            D3D12_TEXTURE_COPY_LOCATION SrcCopyLocation;
            SrcCopyLocation.pResource = UploadD3D12Resource.Get();
            SrcCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            SrcCopyLocation.PlacedFootprint = _SubImages_UploadFootprint[SubresourceIndex];
            
            ExecuteParams.D3D12CommandList->CopyTextureRegion(
                &DstCopyLocation, 
                0, 0, 0,
                &SrcCopyLocation, 
                nullptr 
            );
        }
    }
    
    void F_DirectX12RHIUploadTexturePassProxy::Build(const TW_Valid<A_RHIUploadTexturePass>& Pass)
    {
        A_RHIUploadTexturePassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        
        auto CastedPass = Pass.FastCast<F_DirectX12RHIUploadTexturePass>();
        const auto& TransientUploadBufferRange = CastedPass->GetTransientUploadBufferRange();
        _UploadBufferProxy = TransientUploadBufferRange.GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        _UploadBufferOffsetInBytes = TransientUploadBufferRange.BeginOffsetInBytes;
        _UploadBufferSizeInBytes = TransientUploadBufferRange.GetSizeInBytes();
        _NumSubresources = CastedPass->GetNumSubresources();
        _SubImages_Bytes = CastedPass->GetSubImages_Bytes();
        _SubImages_UploadFootprint = CastedPass->GetSubImages_UploadFootprint();
    }   
    void F_DirectX12RHIUploadTexturePassProxy::Release()
    {
        _SubImages_UploadFootprint = {};
        _SubImages_Bytes = {};
        _NumSubresources = 0;
        _UploadBufferSizeInBytes = 0;
        _UploadBufferOffsetInBytes = 0;
        _UploadBufferProxy = {};
        
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIUploadTexturePassProxy::Release();
    }

    void F_DirectX12RHIUploadTexturePassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        auto D3D12Resource = _UploadBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
    
        auto NumSubresources = GetNumSubresources();
        auto SubImages_Bytes = GetSubImages_Bytes();
        auto SubImages_UploadFootprint = GetSubImages_UploadFootprint();
        
        U8* DataPtr = nullptr;
        HRESULT HR = D3D12Resource->Map(0, nullptr, (void**)&DataPtr);
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot map resource";
        
        for (U32 SubresourceIndex = 0; SubresourceIndex < NumSubresources; ++SubresourceIndex)
        {
            const auto& SubImage_Bytes = SubImages_Bytes[SubresourceIndex];
            const auto& SubImage_UploadFootprint = SubImages_UploadFootprint[SubresourceIndex];
            memcpy(
                DataPtr + SubImage_UploadFootprint.Offset,
                SubImage_Bytes.data(),
                SubImage_Bytes.size()
            );
        }
        
        D3D12_RANGE WrittenRange = { _UploadBufferOffsetInBytes, _UploadBufferOffsetInBytes + _UploadBufferSizeInBytes };
        D3D12Resource->Unmap(0, &WrittenRange);
    }
}
#endif