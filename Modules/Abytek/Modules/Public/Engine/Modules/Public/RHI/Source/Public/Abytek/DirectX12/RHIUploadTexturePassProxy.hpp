#pragma once

#include "Abytek/RHIUploadTexturePassProxy.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHICopyUploadTexturePass;
    class F_DirectX12RHIUploadTexturePassProxy;
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIUploadTexturePassProxy : public A_RHIUploadTexturePassProxy, public A_DirectX12RHIPassProxyExtension
    {
    private:
        TS<A_RHIResourceProxy> _UploadBufferProxy;
        U64 _UploadBufferOffsetInBytes = 0;
        U64 _UploadBufferSizeInBytes = 0;
        U32 _NumSubresources = 0;
        TF_SmallVector<TF_Span<const U8>, 3> _SubImages_Bytes;
        TF_SmallVector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT, 3> _SubImages_UploadFootprint;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetUploadBufferProxy() const noexcept
        {
            return _UploadBufferProxy;
        }
        ABYTEK_FORCE_INLINE auto GetUploadBufferOffsetInBytes() const noexcept
        {
            return _UploadBufferOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetUploadBufferSizeInBytes() const noexcept
        {
            return _UploadBufferSizeInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetNumSubresources() const noexcept
        {
            return _NumSubresources;
        }
        ABYTEK_FORCE_INLINE const auto& GetSubImages_Bytes() const noexcept
        {
            return _SubImages_Bytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetSubImages_UploadFootprint() const noexcept
        {
            return _SubImages_UploadFootprint;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIUploadTexturePassProxy);
        void Build(const TW_Valid<A_RHIUploadTexturePass>& Pass) override;
        void Release() override;
        
    public:
        void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams) override;
    };
}
#endif