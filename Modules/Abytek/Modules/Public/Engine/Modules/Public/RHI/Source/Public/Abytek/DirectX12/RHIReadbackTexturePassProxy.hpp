#pragma once

#include "Abytek/RHIReadbackTexturePassProxy.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHICopyReadbackTexturePass;
    class F_DirectX12RHIReadbackTexturePassProxy;
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHICopyReadbackTexturePassProxy : public A_RHIPassProxy, public A_DirectX12RHIPassProxyExtension
    {
    private:
        TS<A_RHIResourceProxy> _TextureProxy;
        TS<A_RHIResourceProxy> _ReadbackBufferProxy;
        U32 _NumSubresources = 0;
        TF_SmallVector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT, 3> _SubImages_ReadbackFootprint;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetTextureProxy() const noexcept
        {
            return _TextureProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetReadbackBufferProxy() const noexcept
        {
            return _ReadbackBufferProxy;
        }
        ABYTEK_FORCE_INLINE auto GetNumSubresources() const noexcept
        {
            return _NumSubresources;
        }
        ABYTEK_FORCE_INLINE const auto& GetSubImages_ReadbackFootprint() const noexcept
        {
            return _SubImages_ReadbackFootprint;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHICopyReadbackTexturePassProxy);
        void Build(const TW_Valid<F_DirectX12RHICopyReadbackTexturePass>& Pass);
        void Release() override;
        
    public:
        void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams) override;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIReadbackTexturePassProxy : public A_RHIReadbackTexturePassProxy, public A_DirectX12RHIPassProxyExtension
    {
    private:
        TS<A_RHIResourceProxy> _ReadbackBufferProxy;
        U32 _NumSubresources = 0;
        TF_SmallVector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT, 3> _SubImages_ReadbackFootprint;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetReadbackBufferProxy() const noexcept
        {
            return _ReadbackBufferProxy;
        }
        ABYTEK_FORCE_INLINE auto GetNumSubresources() const noexcept
        {
            return _NumSubresources;
        }
        ABYTEK_FORCE_INLINE const auto& GetSubImages_ReadbackFootprint() const noexcept
        {
            return _SubImages_ReadbackFootprint;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIReadbackTexturePassProxy);
        void Build(const TW_Valid<A_RHIReadbackTexturePass>& Pass) override;
        void Release() override;
        
    public:
        void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams) override;
    };
}
#endif