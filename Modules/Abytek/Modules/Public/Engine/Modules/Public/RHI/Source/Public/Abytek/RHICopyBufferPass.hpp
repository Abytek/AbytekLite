#pragma once

#include "Abytek/RHIPass.hpp"


namespace Abytek
{
    class A_RHIResource;
    
    struct F_RHICopyBufferPassBuildParams : public F_RHIPassBuildParams
    {
        TS<A_RHIResource> SrcBuffer;
        U64 SrcOffsetInBytes = 0;
        TS<A_RHIResource> DstBuffer;
        U64 DstOffsetInBytes = 0;
        U64 SizeInBytes = 0;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHICopyBufferPass : public A_RHIPass
    {
    private:
        TS<A_RHIResource> _SrcBuffer;
        U64 _SrcOffsetInBytes = 0;
        TS<A_RHIResource> _DstBuffer;
        U64 _DstOffsetInBytes = 0;
        U64 _SizeInBytes = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetSrcBuffer() const noexcept
        {
            return _SrcBuffer;
        }
        ABYTEK_FORCE_INLINE auto& InjectSrcBuffer() noexcept
        {
            return _SrcBuffer;
        }
        ABYTEK_FORCE_INLINE const auto& GetSrcOffsetInBytes() const noexcept
        {
            return _SrcOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE auto& InjectSrcOffsetInBytes() noexcept
        {
            return _SrcOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetDstBuffer() const noexcept
        {
            return _DstBuffer;
        }
        ABYTEK_FORCE_INLINE auto& InjectDstBuffer() noexcept
        {
            return _DstBuffer;
        }
        ABYTEK_FORCE_INLINE const auto& GetDstOffsetInBytes() const noexcept
        {
            return _DstOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE auto& InjectDstOffsetInBytes() noexcept
        {
            return _DstOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetSizeInBytes() const noexcept
        {
            return _SizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto& InjectSizeInBytes() noexcept
        {
            return _SizeInBytes;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHICopyBufferPass);
        virtual void Build(const F_RHICopyBufferPassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() const override
        {
            return E_RHIPassClass::BLIT;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
    };
}