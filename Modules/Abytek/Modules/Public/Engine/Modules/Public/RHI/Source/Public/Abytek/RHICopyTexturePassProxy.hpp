#pragma once

#include "Abytek/RHICopyTexturePass.hpp"


namespace Abytek
{
    class A_RHIResourceProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHICopyTexturePassProxy : public A_RHIPassProxy
    {
    private:
        TS<A_RHIResourceProxy> _SrcTextureProxy;
        TS<A_RHIResourceProxy> _DestTextureProxy;
        F_RHITextureElementSet _SrcElementSet;
        F_RHITextureElementSet _DstElementSet;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetSrcTextureProxy() const noexcept
        {
            return _SrcTextureProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectSrcTextureProxy() noexcept
        {
            return _SrcTextureProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetDestTextureProxy() const noexcept
        {
            return _DestTextureProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectDestTextureProxy() noexcept
        {
            return _DestTextureProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetSrcElementSet() const noexcept
        {
            return _SrcElementSet;
        }
        ABYTEK_FORCE_INLINE auto& InjectSrcElementSet() noexcept
        {
            return _SrcElementSet;
        }
        ABYTEK_FORCE_INLINE const auto& GetDstElementSet() const noexcept
        {
            return _DstElementSet;
        }
        ABYTEK_FORCE_INLINE auto& InjectDstElementSet() noexcept
        {
            return _DstElementSet;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHICopyTexturePassProxy);
        virtual void Build(const TW_Valid<A_RHICopyTexturePass>& Pass);
        virtual void Release() override;
    };
}