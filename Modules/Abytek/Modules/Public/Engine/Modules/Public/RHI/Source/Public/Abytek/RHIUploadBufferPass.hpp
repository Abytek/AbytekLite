#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIResource.hpp"


namespace Abytek
{
    class A_RHIResource;
    class A_RHISubmissionList;
    
    struct F_RHIUploadBufferPassBuildParams : F_RHIPassBuildParams
    {
        F_RHIBufferDataView BufferDataView;
        TS<A_RHIResource> Buffer;
        U64 OffsetInBytes = 0;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIUploadBufferPass : public A_RHIPass
    {
    private:
        F_RHIBufferDataView _BufferDataView;
        TS<A_RHIResource> _Buffer;
        U64 _OffsetInBytes = 0;

    public:
        ABYTEK_FORCE_INLINE const auto& GetBufferDataView() const noexcept
        {
            return _BufferDataView;
        }
        ABYTEK_FORCE_INLINE auto& InjectBufferDataView() noexcept
        {
            return _BufferDataView;
        }
        ABYTEK_FORCE_INLINE const auto& GetBuffer() const noexcept
        {
            return _Buffer;
        }
        ABYTEK_FORCE_INLINE auto& InjectBuffer() noexcept
        {
            return _Buffer;
        }
        ABYTEK_FORCE_INLINE const auto& GetOffsetInBytes() const noexcept
        {
            return _OffsetInBytes;
        }
        ABYTEK_FORCE_INLINE auto& InjectOffsetInBytes() noexcept
        {
            return _OffsetInBytes;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIUploadBufferPass);
        virtual void Build(const F_RHIUploadBufferPassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() override
        {
            return E_RHIPassClass::BLIT;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
        
    public:
        virtual B8 CanDetachCopyPass();
        virtual void DetachCopyPass(I_RHISubmissionItemContainer& SubmissionItemContainer);
    };
}