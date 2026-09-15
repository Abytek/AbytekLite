#pragma once

#include "Abytek/RHICopyBufferPass.hpp"


namespace Abytek
{
    class A_RHIResourceProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHICopyBufferPassProxy : public A_RHIPassProxy
    {
    private:
        TS<A_RHIResourceProxy> _SrcBufferProxy;
        U64 _SrcOffsetInBytes = 0;
        TS<A_RHIResourceProxy> _DstBufferProxy;
        U64 _DstOffsetInBytes = 0;
        U64 _SizeInBytes = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetSrcBufferProxy() const noexcept
        {
            return _SrcBufferProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectSrcBufferProxy() noexcept
        {
            return _SrcBufferProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetSrcOffsetInBytes() const noexcept
        {
            return _SrcOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE auto& InjectSrcOffsetInBytes() noexcept
        {
            return _SrcOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetDstBufferProxy() const noexcept
        {
            return _DstBufferProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectDstBufferProxy() noexcept
        {
            return _DstBufferProxy;
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHICopyBufferPassProxy);
        virtual void Build(const TW_Valid<A_RHICopyBufferPass>& Pass);
        virtual void Release() override;
    };
}