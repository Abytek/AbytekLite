#pragma once

#include "Abytek/RHIUploadBufferPass.hpp"


namespace Abytek
{
    class A_RHIResourceProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIUploadBufferPassProxy : public A_RHIPassProxy
    {
    private:
        F_RHIBufferDataView _BufferDataView;
        TS<A_RHIResourceProxy> _BufferProxy;
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
        ABYTEK_FORCE_INLINE const auto& GetBufferProxy() const noexcept
        {
            return _BufferProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectBufferProxy() noexcept
        {
            return _BufferProxy;
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIUploadBufferPassProxy);
        virtual void Build(const TW_Valid<A_RHIUploadBufferPass>& Pass);
        virtual void Release() override;
    };
}