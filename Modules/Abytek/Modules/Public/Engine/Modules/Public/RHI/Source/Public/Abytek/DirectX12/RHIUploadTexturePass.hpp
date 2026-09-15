#pragma once

#include "Abytek/RHIUploadTexturePass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/RHITransientUploadBufferManager.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIUploadTexturePass;

    struct F_DirectX12RHICopyUploadTexturePassBuildParams : F_RHIPassBuildParams
    {
        TW<F_DirectX12RHIUploadTexturePass> UploadPass;
    };
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHICopyUploadTexturePass : public A_RHIPass, public A_DirectX12RHIPassExtension
    {
    private:
        TW<F_DirectX12RHIUploadTexturePass> _UploadPass;

    public:
        ABYTEK_FORCE_INLINE const auto& GetUploadPass() const noexcept
        {
            return _UploadPass;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHICopyUploadTexturePass);
        virtual void Build(const F_DirectX12RHICopyUploadTexturePassBuildParams& BuildParams);
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

    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIUploadTexturePass : public A_RHIUploadTexturePass, public A_DirectX12RHIPassExtension
    {
    private:
        F_RHITransientUploadBufferRange _TransientUploadBufferRange;
        U32 _NumSubresources = 0;
        TF_SmallVector<TF_Span<const U8>, 3> _SubImages_Bytes;
        TF_SmallVector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT, 3> _SubImages_UploadFootprint;
        TS<F_DirectX12RHICopyUploadTexturePass> _CopyPass;

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
        ABYTEK_FORCE_INLINE const auto& GetCopyPass() const noexcept
        {
            return _CopyPass;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIUploadTexturePass);
        virtual void Build(const F_RHIUploadTexturePassBuildParams& BuildParams);
        void Release() override;
        
    protected:
        void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    
    public:
        E_DirectX12RHIPassBatchType GetPassBatchType() override;
        
    protected:
        void OnAddItemsAfter(I_RHISubmissionItemContainer& Container) override;
        
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