#pragma once

#include "Abytek/RHIReadbackTexturePass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/RHITransientReadbackBufferManager.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIReadbackTexturePass;

    struct F_DirectX12RHICopyReadbackTexturePassBuildParams : F_RHIPassBuildParams
    {
        TW<F_DirectX12RHIReadbackTexturePass> ReadbackPass;
    };
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHICopyReadbackTexturePass : public A_RHIPass, public A_DirectX12RHIPassExtension
    {
    private:
        TW<F_DirectX12RHIReadbackTexturePass> _ReadbackPass;

    public:
        ABYTEK_FORCE_INLINE const auto& GetReadbackPass() const noexcept
        {
            return _ReadbackPass;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHICopyReadbackTexturePass);
        virtual void Build(const F_DirectX12RHICopyReadbackTexturePassBuildParams& BuildParams);
        void Release() override;
        
    public:
        E_RHIPassClass GetPassClass() override
        {
            return E_RHIPassClass::BLIT;
        }
        
    public:
        TS_Valid<A_RHIPassProxy> CreateProxy() override;
        
    protected:
        void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    };

    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIReadbackTexturePass : public A_RHIReadbackTexturePass, public A_DirectX12RHIPassExtension
    {
    private:
        TF_RHIImageView<true> _ImageView;
        F_RHITransientReadbackBufferRange _TransientReadbackBufferRange;
        U32 _NumSubresources = 0;
        TF_SmallVector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT, 3> _SubImages_ReadbackFootprint;
        TF_SmallVector<U32, 6> _SubImages_NumRow;
        TS<F_DirectX12RHICopyReadbackTexturePass> _CopyPass;

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
        ABYTEK_FORCE_INLINE const auto& GetCopyPass() const noexcept
        {
            return _CopyPass;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIReadbackTexturePass);
        virtual void Build(const F_RHIReadbackTexturePassBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    
    public:
        E_DirectX12RHIPassBatchType GetPassBatchType() override;
        
    protected:
        void OnAddItemsBefore(I_RHISubmissionItemContainer& Container) override;
        
    public:
        B8 CanDetachCopyPass() override;
        void DetachCopyPass(I_RHISubmissionItemContainer& SubmissionItemContainer) override;
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        void SetDebugName(const F_DebugName& Value) noexcept override;
#endif
    };
}
#endif