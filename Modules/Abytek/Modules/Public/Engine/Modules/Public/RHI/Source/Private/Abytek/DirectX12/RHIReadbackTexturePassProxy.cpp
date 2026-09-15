#include "Abytek/DirectX12/RHIReadbackTexturePassProxy.hpp"
#include "Abytek/DirectX12/RHIReadbackTexturePass.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHICopyReadbackTexturePassProxy);
    void F_DirectX12RHICopyReadbackTexturePassProxy::Build(const TW_Valid<F_DirectX12RHICopyReadbackTexturePass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        
        auto CastedPass = Pass.FastCast<F_DirectX12RHICopyReadbackTexturePass>();
        auto CastedReadbackPass = CastedPass->GetReadbackPass();
        const auto& TransientReadbackBufferRange = CastedReadbackPass->GetTransientReadbackBufferRange();
        _TextureProxy = CastedReadbackPass->GetTexture()->GetProxy().FastCast<A_RHIResourceProxy>();
        _ReadbackBufferProxy = TransientReadbackBufferRange.GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        _NumSubresources = CastedReadbackPass->GetNumSubresources();
        _SubImages_ReadbackFootprint = CastedReadbackPass->GetSubImages_ReadbackFootprint();
    }   
    void F_DirectX12RHICopyReadbackTexturePassProxy::Release()
    {
        _SubImages_ReadbackFootprint = {};
        _NumSubresources = 0;
        _ReadbackBufferProxy = {};
        _TextureProxy = {};
        
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIPassProxy::Release();
    }

    void F_DirectX12RHICopyReadbackTexturePassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        auto TextureD3D12Resource = _TextureProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
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
    
    void F_DirectX12RHIReadbackTexturePassProxy::Build(const TW_Valid<A_RHIReadbackTexturePass>& Pass)
    {
        A_RHIReadbackTexturePassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        
        auto CastedPass = Pass.FastCast<F_DirectX12RHIReadbackTexturePass>();
        const auto& TransientReadbackBufferRange = CastedPass->GetTransientReadbackBufferRange();
        _ReadbackBufferProxy = TransientReadbackBufferRange.GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        _ReadbackBufferOffsetInBytes = TransientReadbackBufferRange.BeginOffsetInBytes;
        _ReadbackBufferSizeInBytes = TransientReadbackBufferRange.GetSizeInBytes();
        _ImageView = CastedPass->GetImageView();
    }   
    void F_DirectX12RHIReadbackTexturePassProxy::Release()
    {
        _ImageView = {};
        _ReadbackBufferSizeInBytes = 0;
        _ReadbackBufferOffsetInBytes = 0;
        _ReadbackBufferProxy = {};
        
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIReadbackTexturePassProxy::Release();
    }

    void F_DirectX12RHIReadbackTexturePassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        auto D3D12Resource = _ReadbackBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
    
        D3D12_RANGE D3D12ReadRange = { _ReadbackBufferOffsetInBytes, _ReadbackBufferOffsetInBytes + _ReadbackBufferSizeInBytes };
        U8* DataPtr = nullptr;
        HRESULT HR = D3D12Resource->Map(0, &D3D12ReadRange, (void**)&DataPtr);
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot map resource";
        
        memcpy(
            _ImageView.Payload.Bytes.data(),
            DataPtr,
            _ReadbackBufferSizeInBytes
            );
        
        GetCallback()(_ImageView.GetReadOnly());
        
        D3D12Resource->Unmap(0, nullptr);
    }
}
#endif