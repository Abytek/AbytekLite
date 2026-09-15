#pragma once

#include "Abytek/RHIUploadBufferPass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/RHITransientUploadBufferManager.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class A_RHICopyBufferPass;

    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIUploadBufferPass : public A_RHIUploadBufferPass, public A_DirectX12RHIPassExtension
    {
    private:
        F_RHITransientUploadBufferRange _TransientUploadBufferRange;
        TS<A_RHICopyBufferPass> _CopyPass;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTransientUploadBufferRange() const noexcept
        {
            return _TransientUploadBufferRange;
        }
        ABYTEK_FORCE_INLINE const auto& GetCopyPass() const noexcept
        {
            return _CopyPass;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIUploadBufferPass);
        virtual void Build(const F_RHIUploadBufferPassBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    
    public:
        virtual E_DirectX12RHIPassBatchType GetPassBatchType() override;
        
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