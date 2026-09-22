#pragma once

#include "Abytek/RHIReadbackTexturePass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/RHITransientReadbackBuffer.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIReadbackTexturePass : public A_RHIReadbackTexturePass, public A_DirectX12RHIPassExtension
    {
    private:
        TF_RHIImageView<true> _ImageView;
        F_RHITransientReadbackBufferRange_V2 _TransientReadbackBufferRange;
        U32 _NumSubresources = 0;
        TF_SmallVector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT, 3> _SubImages_ReadbackFootprint;
        TF_SmallVector<U32, 6> _SubImages_NumRow;

    public:
        ABYTEK_FORCE_INLINE auto& GetImageView() const noexcept
        {
            return _ImageView;
        }
        ABYTEK_FORCE_INLINE const auto& GetTransientReadbackBufferRange() const noexcept
        {
            return _TransientReadbackBufferRange;
        }
        ABYTEK_FORCE_INLINE const auto& GetNumSubresources() const noexcept
        {
            return _NumSubresources;
        }
        ABYTEK_FORCE_INLINE const auto& GetSubImages_ReadbackFootprint() const noexcept
        {
            return _SubImages_ReadbackFootprint;
        }
        ABYTEK_FORCE_INLINE const auto& GetSubImages_NumRow() const noexcept
        {
            return _SubImages_NumRow;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIReadbackTexturePass);
        virtual void Build(const F_RHIReadbackTexturePassBuildParams& BuildParams);
        void Release() override;
        
    protected:
        void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    
    public:
        E_DirectX12RHIPassBatchType GetPassBatchType() override;
    };
}
#endif