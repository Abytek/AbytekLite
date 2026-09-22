#pragma once

#include "Abytek/RHIReadbackBufferPass.hpp"


namespace Abytek
{
    class A_RHIResourceProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIReadbackBufferPassProxy : public A_RHIPassProxy
    {
    private:
        TS<A_RHIResourceProxy> _BufferProxy;
        U64 _SizeInBytes = 0;
        U64 _OffsetInBytes = 0;
        F_RHIReadbackBufferCallback _Callback;

    public:
        ABYTEK_FORCE_INLINE const auto& GetBufferProxy() const noexcept
        {
            return _BufferProxy;
        }
        ABYTEK_FORCE_INLINE auto GetSizeInBytes() const noexcept
        {
            return _SizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetOffsetInBytes() const noexcept
        {
            return _OffsetInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetCallback() const noexcept
        {
            return _Callback;
        }
        ABYTEK_FORCE_INLINE auto HasWork() const noexcept
        {
            return _SizeInBytes > 0;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIReadbackBufferPassProxy);
        virtual void Build(const TW_Valid<A_RHIReadbackBufferPass>& Pass);
        virtual void Release() override;
    };
}