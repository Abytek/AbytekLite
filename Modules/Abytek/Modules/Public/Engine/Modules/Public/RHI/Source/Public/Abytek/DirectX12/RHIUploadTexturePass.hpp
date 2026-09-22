#pragma once

#include "Abytek/RHIUploadTexturePass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/RHITransientUploadBuffer.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIUploadTexturePass;

    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIUploadTexturePass : public A_RHIUploadTexturePass, public A_DirectX12RHIPassExtension
    {
    private:
        F_RHITransientUploadBufferRange_V2 _TransientUploadBufferRange;
        U32 _NumSubresources = 0;
        TF_SmallVector<TF_Span<const U8>, 3> _SubImages_Bytes;
        TF_SmallVector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT, 3> _SubImages_UploadFootprint;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTransientUploadBufferRange() const noexcept
        {
            return _TransientUploadBufferRange;
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
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIUploadTexturePass);
        virtual void Build(const F_RHIUploadTexturePassBuildParams& BuildParams);
        void Release() override;
        
    protected:
        void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    
    public:
        E_DirectX12RHIPassBatchType GetPassBatchType() override;
    };
}
#endif